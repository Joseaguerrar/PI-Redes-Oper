/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2025-i
 *  Grupos: 1 y 3
 *
 ****** SSLSocket example
 *
 * (Fedora version)
 *
 **/

#include <cstdlib> // atoi, exit
#include <cstring> // strlen, snprintf
#include <cstdio>  // printf, scanf
#include "SSLSocket.h"

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      printf("usage: %s <hostname> <portnum>\n", argv[0]);
      return 1;
   }

   const char *hostname = argv[1];
   int port = std::atoi(argv[2]);

   // 1) Creamos un SSLSocket (no un Socket)
   SSLSocket *client = new SSLSocket('s');

   // 2) Inicializamos la librería SSL
   client->SSLInit();

   // 3) Conectamos (TCP + handshake SSL interno)
   try
   {
      client->SSLConnect(const_cast<char *>(hostname), port);
   }
   catch (const std::exception &e)
   {
      fprintf(stderr, "SSLConnect failed: %s\n", e.what());
      return 1;
   }

   // 4) Pedimos credenciales al usuario
   char userName[16] = {0}, password[16] = {0};
   printf("Enter the User Name: ");
   scanf("%15s", userName);
   printf("Enter the Password: ");
   scanf("%15s", password);

   // 5) Montamos el mensaje XML
   char clientRequest[1024];
   std::snprintf(clientRequest, sizeof(clientRequest),
                 "\n<Body>\n\t<UserName>%s</UserName>\n\t<Password>%s</Password>\n</Body>\n",
                 userName, password);

   // 6) Información de cifrado y certificados
   printf("\nConnected with %s encryption\n", client->SSLGetCipher());
   client->SSLShowCerts();

   // 7) Enviamos y recibimos
   client->SSLWrite(clientRequest, std::strlen(clientRequest));
   char buf[1024];
   int bytes = client->SSLRead(buf, sizeof(buf));
   buf[bytes] = '\0';
   printf("Received: \"%s\"\n", buf);

   // 8) Cerramos
   client->SSLCleanup();
   delete client;
   return 0;
}