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
   const char * interface = (argc >= 2) ? argv[1] : "eth0"; // Permitir pasar interfaz como argumento
   char buffer[MAXBUF];
   VSocket * client;
   struct sockaddr_in6 server_addr;
   const char * message = "Mensaje desde el cliente TDP";

   // Limpiar buffer
memset(buffer, 0, MAXBUF);

// Crear socket SSL/TCP IPv6
client = new SSLSocket(true);

// IP sin el %interfaz (que no lo acepta inet_pton internamente)
const char * ip_clean = "fe80::215:5dff:feb9:6c72";

// Establecer conexión
if (client->MakeConnection(ip_clean, PORT) != 0) {
    fprintf(stderr, "Error al conectar vía SSL\n");
    exit(EXIT_FAILURE);
}

// Enviar mensaje al servidor
client->Write(message);
printf("Mensaje enviado al servidor SSL.\n");

// Leer respuesta
client->Read(buffer, MAXBUF);
printf("Respuesta del servidor: %s\n", buffer);

// Cerrar conexión
client->Close();
   return 0;
}
