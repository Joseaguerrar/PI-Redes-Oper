/**
  *  Para IPv6 esta prueba solo funciona utilizando un equipo de la red interna de la ECCI,
  *  por lo que deberán realizarlo en la ECCI o conectarse por la VPN para completarla
  *  La dirección IPv6 provista es una dirección privada
  *
  *  También deben prestar atención al componente que esta luego del "%" en la dirección y que hace
  *  referencia a la interfaz de red utilizada para la conectividad: "enp0s31f6"
  *
  *  Puede obtener las direcciones de sus tarjetas de red con el comando "ip addr"
  *
  *  Uso:
  *
  *    ./tcp-cli.out ipVer[4|6] SSL[0|1]
  *
 **/
 
 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>  // para if_nametoindex

#include "VSocket.h"
#include "Socket.h"
#include "SSLSocket.h"

#define MAXBUF 1024
#define PORT 1234

int main(int argc, char *argv[]) {
   const char * whalev6 = "fe80::215:5dff:feb9:6c72";//%enp0s31f6";  // Dirección IPv6 del servidor
   const char * interface = "eth0";
   char buffer[MAXBUF];
   VSocket * client;
   struct sockaddr_in6 server_addr;
   const char * message = "Mensaje desde el cliente UDP";

   memset(buffer, 0, MAXBUF);

   client = new Socket('d', true);  // Crear socket UDP IPv6

   // Configurar la dirección del servidor
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin6_family = AF_INET6;
   server_addr.sin6_port = htons(PORT);
   
   if (inet_pton(AF_INET6, whalev6, &server_addr.sin6_addr) <= 0) {
      perror("inet_pton");
      exit(EXIT_FAILURE);
   }
   server_addr.sin6_scope_id = if_nametoindex(interface);
   if (server_addr.sin6_scope_id == 0) {
       perror("if_nametoindex");
       exit(EXIT_FAILURE);
   }
   // Enviar mensaje
   client->sendTo((const void *)message, strlen(message), (void *)&server_addr);
   printf("Mensaje enviado al servidor.\n");

   // Recibir respuesta
   client->recvFrom((void *)buffer, MAXBUF, (void *)&server_addr);
   printf("Respuesta del servidor: %s\n", buffer);

   client->Close();
   return 0;
}
