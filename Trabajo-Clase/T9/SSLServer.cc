/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <thread>
#include <cstdlib> // atoi, exit
#include <cstdio>  // printf
#include <cstring> // strcmp, strlen
#include "SSLSocket.h"

#define PORT 4321

void Service(SSLSocket *client)
{
   char buf[1024] = {0};
   const char *ServerResponse = "\n<Body>\n\
\t<Server>os.ecci.ucr.ac.cr</Server>\n\
\t<dir>ci0123</dir>\n\
\t<Name>Proyecto Integrador Redes y sistemas Operativos</Name>\n\
\t<NickName>PIRO</NickName>\n\
\t<Description>Consolidar e integrar los conocimientos de redes y sistemas operativos</Description>\n\
\t<Author>profesores PIRO</Author>\n</Body>\n";
   const char *validMessage = "\n<Body>\n\
\t<UserName>piro</UserName>\n\
\t<Password>ci0123</Password>\n</Body>\n";

   // Mostrar certificados del cliente
   client->SSLShowCerts();

   // Leer petición del cliente
   int bytes = client->SSLRead(buf, sizeof(buf));
   buf[bytes] = '\0';
   printf("Client msg: \"%s\"\n", buf);

   // Validar y responder
   if (std::strcmp(validMessage, buf) == 0)
   {
      client->SSLWrite(ServerResponse, std::strlen(ServerResponse));
   }
   else
   {
      const char *err = "Invalid Message";
      client->SSLWrite(err, std::strlen(err));
   }

   // Cerrar conexión
   client->SSLCleanup();
   delete client;
}

int main(int argc, char **argv)
{
   int port = (argc > 1) ? std::atoi(argv[1]) : PORT;

   // Inicialización del servidor SSL
   SSLSocket *server = new SSLSocket('s', true);
   server->Bind(port);
   server->Listen(10);
   server->SSLInitServer("ci0123.pem", "ci0123.pem");

   for (;;)
   {
      // 1) Aceptar conexión TCP
      int client_fd = server->DoAccept();

      // 2) Construir SSLSocket para el cliente
      SSLSocket *client = new SSLSocket('s', true);
      client->BuildSocket(client_fd);

      // 3) Crear objeto SSL y asociarlo al contexto del servidor
      client->SSLCreate(server);

      // 4) Hacer handshake SSL
      try
      {
         client->SSLAccept();
      }
      catch (const std::exception &e)
      {
         // Mostrar error y descartar cliente
         ERR_print_errors_fp(stderr);
         delete client;
         continue;
      }

      // 5) Servir en hilo separado
      std::thread(Service, client).detach();
   }

   // Nunca llega aquí, limpiar si fuera necesario
   delete server;
   return 0;
}
