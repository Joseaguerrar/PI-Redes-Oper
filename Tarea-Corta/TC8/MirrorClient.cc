/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example
 *
 *   Deben determinar la dirección IP del equipo donde van a correr el servidor
 *   para hacer la conexión en ese punto (ip addr)
 *
 **/

 #include <stdio.h>
 #include <cstring>
 #include "Socket.h"
 
 #define PORT 1234
 #define BUFSIZE 512
 
 int main( int argc, char ** argv ) {
    VSocket * s;
    char buffer[ BUFSIZE ];
 
    s = new Socket( 's', true );     // Creaite a new stream IPv6 socket
    memset( buffer, 0, BUFSIZE );	// Zero fill buffer
 
    s->MakeConnection( "fe80::215:5dff:fefd:80e9%eth0", PORT ); // Same port as server, en este caso mi dirección, también se puede loopback (igual que 127.0.0.1 pero para IPv6 sería: "::1")
    if ( argc > 1 ) {
       s->Write( argv[1] );		// If provided, send first program argument to server
    } else {
       s->Write( "Hello world 2025 ..." );
    }
    s->Read( buffer, BUFSIZE );	// Read answer sent back from server
    printf( "%s", buffer );	// Print received string, mirror example this will print same sent string
 
 }