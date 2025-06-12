#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "Socket.h"

#define BROADCAST_PORT 5353

int main()
{
    Socket s('d'); // Datagram socket (UDP)
    s.BuildSocket('d');

    sockaddr_in recvAddr{};
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(BROADCAST_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    s.Bind(BROADCAST_PORT);

    char buffer[512];
    sockaddr_in senderAddr;
    for (int i = 0; i < 3; i++)
    {
        size_t len = s.recvFrom(buffer, sizeof(buffer) - 1, &senderAddr);
        buffer[len] = '\0';

        std::cout << "Mensaje recibido: " << buffer << std::endl;
    }
    std::cout<< "Servidor finalizó la recepción de mensajes.\n";
    s.Close();

    return 0;
}
