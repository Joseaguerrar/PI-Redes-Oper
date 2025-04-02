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

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <sys/socket.h>
 #include <openssl/ssl.h>
 
 #include "Socket.h"
 #include "SSLSocket.h"
 
 #define PORT 1234
 #define MAXBUF 1024
 
 int main() {
   // Paso 1: Crear socket TCP y hacer bind+listen
   Socket listener('s', true);  // TCP/IPv6
   listener.Bind(PORT);
   listen(listener.GetIdSocket(), 5);  // Aceptar hasta 5 conexiones

   printf("Esperando conexiones SSL en IPv6...\n");

   struct sockaddr_storage client_addr;
   socklen_t client_len = sizeof(client_addr);

   int client_fd = accept(listener.GetIdSocket(), (struct sockaddr *)&client_addr, &client_len);
   if (client_fd < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
   }

   // Paso 2: Wrappear el descriptor en un SSLSocket
   SSLSocket sslClient(client_fd);
   sslClient.Init(true);  // Inicializa como servidor
   sslClient.LoadCertificates("cert.pem", "key.pem");

   // Paso 3: Hacer handshake SSL
   SSL *ssl = sslClient.GetSSLStruct();
   SSL_set_fd(ssl, client_fd);
   if (SSL_accept(ssl) <= 0) {
      perror("SSL_accept");
      exit(EXIT_FAILURE);
   }

   // Paso 4: Leer mensaje del cliente
   char buffer[MAXBUF] = {0};
   sslClient.Read(buffer, MAXBUF);
   printf("Mensaje del cliente: %s\n", buffer);

   // Paso 5: Responder
   const char * response = "Respuesta del servidor SSL";
   sslClient.Write(response);

   sslClient.Close();
   return 0;
}
 

