#include <iostream>
#include <thread>
#include <map>
#include <mutex>
#include <sstream>
#include <chrono>
#include <cstring>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Socket.h"

// Constantes utilizadas
#define DISCOVERY_PORT 5353   // Puerto UDP para descubrimiento
#define TCP_SERVER_PORT 8081  // Puerto TCP del servidor de figuras
#define CLIENT_HTTP_PORT 8080 // Puerto donde el cliente hace peticiones HTTP
#define TIMEOUT_RESPONSE 5    // Tiempo de espera máximo para recibir respuestas UDP
#define BROADCAST_WAIT 120    // Espera entre rondas de descubrimiento (segundos)

using namespace std;

// Lista de direcciones broadcast para descubrimiento (solo localhost en este ejemplo)
vector<string> broadcast_ips = {
    "127.0.0.255" // broadcast local (para pruebas)
};

// Tabla de ruteo: figura -> IP del servidor que la contiene
map<string, string> tabla_ruteo;
mutex tabla_mutex; // Protege el acceso a la tabla

// ---------------------------------------------
// Hilo de descubrimiento: envía broadcast y recibe respuestas UDP
// ---------------------------------------------
void discovery_thread()
{
    Socket s('d'); // Socket tipo datagrama (UDP)
    s.BuildSocket('d');

    // Permitir broadcast en este socket
    int yes = 1;
    setsockopt(s.idSocket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DISCOVERY_PORT);

    while (true)
    {
        // Enviar broadcast a todas las IPs configuradas
        string mensaje = "GET /servers";
        for (const auto &ip : broadcast_ips)
        {
            inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
            s.sendTo(mensaje.c_str(), mensaje.size(), &addr);
            cout << "[BROADCAST] Enviado a " << ip << endl;
        }

        // Esperar respuestas durante TIMEOUT_RESPONSE segundos
        auto start = chrono::steady_clock::now();
        char buffer[512];
        sockaddr_in senderAddr{};

        while (true)
        {
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - start).count() > TIMEOUT_RESPONSE)
                break;

            // Recibir datos
            size_t len = s.recvFrom(buffer, sizeof(buffer) - 1, &senderAddr);
            buffer[len] = '\0';

            // Esperado: ServerName|ip|figura1,figura2,...
            string respuesta(buffer);
            istringstream iss(respuesta);
            string nombre, ip, lista;

            // Parsear respuesta y registrar figuras
            if (getline(iss, nombre, '|') &&
                getline(iss, ip, '|') &&
                getline(iss, lista))
            {
                // Limpiar espacios
                nombre.erase(remove(nombre.begin(), nombre.end(), ' '), nombre.end());
                ip.erase(remove(ip.begin(), ip.end(), ' '), ip.end());
                lista.erase(remove(lista.begin(), lista.end(), ' '), lista.end());

                istringstream figs_stream(lista);
                string figura;
                lock_guard<mutex> lock(tabla_mutex); // Protege acceso
                while (getline(figs_stream, figura, ','))
                {
                    tabla_ruteo[figura] = ip;
                    cout << "[RUTEO] Figura '" << figura << "' registrada con IP " << ip << endl;
                }
            }
        }

        // Esperar antes de hacer otra ronda
        this_thread::sleep_for(chrono::seconds(BROADCAST_WAIT));
    }

    s.Close();
}

// ---------------------------------------------
// Hilo HTTP: escucha en puerto 8080 y responde peticiones del cliente
// ---------------------------------------------
void atender_clientes_http()
{
    VSocket *servidor = new Socket('s');
    servidor->Bind(CLIENT_HTTP_PORT);
    servidor->MarkPassive(5); // Cola de conexiones
    cout << "[HTTP] Servidor escuchando en puerto " << CLIENT_HTTP_PORT << "\n";

    while (true)
    {
        VSocket *cliente = servidor->AcceptConnection(); // Espera cliente
        char buffer[512];
        cliente->Read(buffer, sizeof(buffer) - 1);
        buffer[511] = '\0';

        string request(buffer);
        string prefix = "GET /figure?name=";
        size_t pos = request.find(prefix);

        if (pos != string::npos)
        {
            // Extraer nombre de la figura desde la URL
            string nombre_figura = request.substr(pos + prefix.length());
            size_t fin = nombre_figura.find(' ');
            if (fin != string::npos){
                nombre_figura = nombre_figura.substr(0, fin);
            }
            // Filtrar caracteres inválidos (solo alfanuméricos, guión y guión bajo)
            nombre_figura.erase(remove_if(nombre_figura.begin(), nombre_figura.end(),
                                          [](char c)
                                          { return !isalnum(c) && c != '_' && c != '-'; }),
                                nombre_figura.end());

            string ip_destino;

            // Buscar en la tabla de ruteo
            {
                lock_guard<mutex> lock(tabla_mutex);
                if (tabla_ruteo.find(nombre_figura) != tabla_ruteo.end())
                {
                    ip_destino = tabla_ruteo[nombre_figura];
                }
            }

            if (!ip_destino.empty())
            {
                try
                {
                    // Contactar al servidor de figuras por TCP
                    Socket servidor_tcp('s');
                    servidor_tcp.BuildSocket('s');
                    servidor_tcp.MakeConnection(ip_destino.c_str(), TCP_SERVER_PORT);

                    string solicitud = "GET /figure/" + nombre_figura;
                    servidor_tcp.Write(solicitud.c_str(), solicitud.size());

                    // Leer respuesta con timeout
                    char respuesta[2048] = {0};
                    struct timeval timeout;
                    timeout.tv_sec = TIMEOUT_RESPONSE;
                    timeout.tv_usec = 0;
                    setsockopt(servidor_tcp.idSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

                    size_t bytes = servidor_tcp.Read(respuesta, sizeof(respuesta) - 1);
                    respuesta[bytes] = '\0';

                    string body = "<html><body><pre>\n" + string(respuesta) + "\n</pre></body></html>";
                    // Construir respuesta HTTP con la figura en <pre>
                    string http_response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html; charset=UTF-8\r\n"
                        "Content-Length: " +
                        to_string(body.size()) + "\r\n"
                                                            "\r\n<html><body><pre>\n" +
                        string(respuesta) + "\n</pre></body></html>";

                    cliente->Write(http_response.c_str(), http_response.size());
                    cout << "[HTTP] Figura '" << nombre_figura << "' enviada al cliente.\n";
                    servidor_tcp.Close();
                }
                catch (...)
                {
                    // Si falla la conexión TCP
                    string err = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                    cliente->Write(err.c_str(), err.size());
                    cout << "[HTTP] Timeout/error al contactar servidor de figura '" << nombre_figura << "'.\n";
                }
            }
            else
            {
                // Figura no encontrada en la tabla
                string err = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                cliente->Write(err.c_str(), err.size());
                cout << "[HTTP] Figura '" << nombre_figura << "' no registrada en tabla.\n";
            }
        }
        else
        {
            // Solicitud mal formada
            string err = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            cliente->Write(err.c_str(), err.size());
            cout << "[HTTP] Solicitud inválida.\n";
        }

        // Cierra conexión con el cliente
        cliente->Close();
        delete cliente;
    }

    delete servidor;
}

// ---------------------------------------------
// Función principal
// ---------------------------------------------
int main()
{
    // Iniciar hilos: descubrimiento y HTTP
    thread t1(discovery_thread);
    thread t2(atender_clientes_http);

    cout << "[INIT] Tenedor activo.\n";

    t1.join();
    t2.join();

    return 0;
}
