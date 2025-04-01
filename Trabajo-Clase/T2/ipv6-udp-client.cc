/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
  *
  * (Fedora version)
  *
  *   Client side implementation of IPv6 UDP client-server model 
  *
 **/

#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "Socket.h"

#define PORT    1234 
#define MAXLINE 1024 

int main() {
   VSocket * client;
   int n;
   char buffer[MAXLINE]; 
   char *hello = (char *) "Hello from client"; 

   // Cambiar a false si se quiere usar IPv4
   bool usarIPv6 = false;

   if (!usarIPv6) {
      struct sockaddr_in other;
      client = new Socket('d', false);  // false → IPv4
      memset(&other, 0, sizeof(other));
      other.sin_family = AF_INET;
      other.sin_port = htons(PORT);
      other.sin_addr.s_addr = inet_addr("127.0.0.1");
   
      n = client->sendTo((void *) hello, strlen(hello), (void *) &other); 
      printf("Client (IPv4): Hello message sent.\n");
   
      n = client->recvFrom((void *) buffer, MAXLINE, (void *) &other);
   } else {
      struct sockaddr_in6 other;
      client = new Socket('d', true);  // true → IPv6
      memset(&other, 0, sizeof(other));
      other.sin6_family = AF_INET6;
      other.sin6_port = htons(PORT);
      inet_pton(AF_INET6, "::1", &other.sin6_addr);
   
      n = client->sendTo((void *) hello, strlen(hello), (void *) &other); 
      printf("Client (IPv6): Hello message sent.\n");
   
      n = client->recvFrom((void *) buffer, MAXLINE, (void *) &other);
   }

   buffer[n] = '\0'; 
   printf("Client message received: %s\n", buffer); 

   client->Close(); 

   return 0;
}

