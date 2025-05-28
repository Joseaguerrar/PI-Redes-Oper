#include "syscall.h"

int main()
{
    int sock;

    // IP como puntero a string literal (evita memcpy)
    const char *ip = "127.0.0.1";
    int puerto = 1234;

    // Crear socket
    sock = Socket(AF_INET_NachOS, SOCK_STREAM_NachOS);
    if (sock < 0)
    {
        Write("Error al crear socket\n", 23, ConsoleOutput);
        Exit(-1);
    }

    // Conectarse al servidor
    if (Connect(sock, ip, puerto) < 0)
    {
        Write("Error al conectar\n", 20, ConsoleOutput);
        Exit(-1);
    }

    // Request como puntero a string literal
    const char *request = "GET /figure?name=gato HTTP/1.1\r\n\r\n";
    Write(request, 36, sock); // 36 caracteres, sin incluir \0

    // Leer respuesta e imprimirla
    char buffer[128];
    int bytes;
    while ((bytes = Read(buffer, 127, sock)) > 0)
    {
        buffer[bytes] = '\0';
        Write(buffer, bytes, ConsoleOutput);
    }

    Exit(0);
}
