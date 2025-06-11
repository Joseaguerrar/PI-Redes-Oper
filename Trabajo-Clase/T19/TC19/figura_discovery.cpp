#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "Socket.h"

#define BROADCAST_IP "172.16.123.95"
#define BROADCAST_PORT 5353

int main()
{
    Socket s('d'); // Datagram socket (UDP)
    s.BuildSocket('d');

    // Habilita broadcast
    int yes = 1;
    setsockopt(s.idSocket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BROADCAST_PORT);
    inet_pton(AF_INET, BROADCAST_IP, &addr.sin_addr);

    std::string msg = "FIGURA_SERVER ServidorA 172.16.123.85 8081 gato,arbol,sombrilla";

    for (int i = 0; i < 3; i++)
    {
        s.sendTo(msg.c_str(), msg.size(), &addr);
        std::cout << "Servidor anunció su presencia.\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    std::cout << "Servidor finalizó el anuncio.\n";
    s.Close();

    return 0;
}
