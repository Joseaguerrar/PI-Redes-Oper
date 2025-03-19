/*
 *  Esta prueba solo funciona utilizando un equipo de la red interna de la ECCI, por lo que
 *  deberan realizarlo en la ECCI o  conectarse por la VPN para completarla
 *  La direccion IPv6 provista es una direccion privada
 *  Tambien deben prestar atencion al componente que esta luego del "%" en la direccion y que hace
 *  referencia a la interfaz de red utilizada para la conectividad, en el ejemplo se presenta la interfaz "eno1"
 *  pero es posible que su equipo tenga otra interfaz
 *
 * Read.Me

Objetivo
   Completar las clase "VSocket" y "Socket" en C++ para lograr el intercambio de mensajes entre computadoras que no comparten memoria utilizando el protocolo TCP en IPv6

Tareas

   - Completar las clases C++ "VSocket" y "Socket" para poder intercambiar mensajes entre procesos que no comparten memoria.
   - Se facilita las interfaces "VSocket.h" y "Socket.h".  Para esta tarea deben completar la funcionalidad para IPv6.

   - Los ejemplos "ipv4-test.cc" y "ipv6-test.cc" deben funcionar correctamente

Descripción detallada:
   - Clase Socket (revisar el archivo Socket.h para conocer los detalles)
      - int MakeConnection( const char *, const char * );


   - Clase VSocket (revisar el archivo VSocket.h para conocer los detalles)

      virtual int EstablishConnection( const char *, int ) = 0;

      virtual int EstablishConnection( const char *, const char * ) = 0;


      VSocket::EstablishConnection( const char * hostip, int port );	// connect to server (char *), and port (int)
         // Para IPv6 
            struct sockaddr_in6  host6;
            struct sockaddr * ha;

            memset( &host6, 0, sizeof( host6 ) );
            host6.sin6_family = AF_INET6;
            st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
            if ( 0 <= st ) {	// 0 means invalid address, -1 means address error
               throw std::runtime_error( "Socket::Connect( const char *, int ) [inet_pton]" );
            }
            host6.sin6_port = htons( port );
            ha = (struct sockaddr *) &host6;
            len = sizeof( host6 );
            st = connect( this->id, ha, len );
            if ( -1 == st ) {
               throw std::runtime_error( "Socket::Connect( const char *, int ) [connect]" );
            }


      VSocket::MakeConnection( const char *, const char *);	// connect to server (char *), and service (char *)
         ...
         struct addrinfo hints, *result, *rp;

         memset(&hints, 0, sizeof(struct addrinfo));
         hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
         //hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    //     hints.ai_flags = 0;
  //       hints.ai_protocol = 0;          /* Any protocol */

//         st = getaddrinfo( host, service, &hints, &result );

         //for ( rp = result; rp; rp = rp->ai_next ) {
       //     st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
      // //     if ( 0 == st )
    //           break;
  //       }
//
    //     freeaddrinfo( result );
  //       ...
//

 
#include <stdio.h>
#include <string.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {
   const char * lab/*Server IPv6*/ = "fe80::8c8e:fdf2:ed09:60e7%enp0s31f6"; // Reemplazando con la interfaz correcta
   const char * request = "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', true ); // Crear socket IPv6
   char a[512];

   memset( a, 0, 512 );
   s.MakeConnection( lab, (char *) "8080" ); // Conectar al servidor con el puerto y no servicio http
   s.Write(  request );
   s.Read( a, 512 );
   printf( "%s\n", a);

}