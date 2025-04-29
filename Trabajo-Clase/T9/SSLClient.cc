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

#include <cstdlib>
#include <cstring> // strlen
#include <cstdio>

#include "SSLSocket.h" // incluir SSLSocket

/**
 * Cliente SSL
 **/
int main(int cuantos, char *argumentos[])
{
   SSLSocket *client; // Ahora es un SSLSocket
   char userName[16] = {0};
   char password[16] = {0};
   const char *requestMessage = "\n<Body>\n\
\t<UserName>%s</UserName>\n\
\t<Password>%s</Password>\n\
</Body>\n";

   char buf[1024];
   char clientRequest[1024] = {0};
   int bytes;
   char *hostname, *portnum;

   if (cuantos != 3)
   {
      printf("usage: %s <hostname> <portnum>\n", argumentos[0]);
      exit(0);
   }

   hostname = argumentos[1];
   portnum = argumentos[2];

   client = new SSLSocket('s');
   client->Init();                                  // Inicializar contexto SSL y SSL_new
   client->MakeConnection(hostname, atoi(portnum)); // Usar MakeConnection normal TCP
   client->Accept();                                // Realizar el handshake SSL

   printf("Enter the User Name : ");
   scanf("%s", userName);
   printf("\nEnter the Password : ");
   scanf("%s", password);

   sprintf(clientRequest, requestMessage, userName, password); // construir mensaje
   printf("\n\nConnected with %s encryption\n", client->GetCipher());
   client->ShowCerts();                                    // mostrar certificados
   client->SSLWrite(clientRequest, strlen(clientRequest)); // escribir datos
   bytes = client->SSLRead(buf, sizeof(buf));              // leer respuesta
   buf[bytes] = 0;
   printf("Received: \"%s\"\n", buf);

   client->Close();
   delete client;

   return 0;
}
