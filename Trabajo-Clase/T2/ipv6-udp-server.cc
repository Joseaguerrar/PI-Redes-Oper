/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
  *
  * (Fedora version)
  *
  *  Server-side implementation of UDP client-server model for IPv6	
  *
 **/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Socket.h" 

#define PORT	1234 
#define MAXLINE 1024 

int main() { 
   VSocket * server;
   int n; 
   char buffer[MAXLINE]; 
   char *hello = (char *) "Hello from CI0123 server"; 

   // Cambiar a false si se quiere usar IPv4
   bool usarIPv6 = false;

   if (!usarIPv6) {
      struct sockaddr_in other;
      server = new Socket('d', false);  // false → IPv4
      server->Bind(PORT);
   
      memset(&other, 0, sizeof(other));
      n = server->recvFrom((void *) buffer, MAXLINE, (void *) &other);
      buffer[n] = '\0'; 
      printf("Server (IPv4): message received: %s\n", buffer);
   
      server->sendTo((const void *) hello, strlen(hello), (void *) &other);
      printf("Server (IPv4): Hello message sent.\n");
   } else {
      struct sockaddr_in6 other;
      server = new Socket('d', true);  // true → IPv6
      server->Bind(PORT);
   
      memset(&other, 0, sizeof(other));
      n = server->recvFrom((void *) buffer, MAXLINE, (void *) &other);
      buffer[n] = '\0'; 
      printf("Server (IPv6): message received: %s\n", buffer);
   
      server->sendTo((const void *) hello, strlen(hello), (void *) &other);
      printf("Server (IPv6): Hello message sent.\n");
   }

   server->Close();
   return 0;
}

