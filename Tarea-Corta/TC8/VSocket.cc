/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
  *
  ****** VSocket base class implementation
  *
  * (Fedora version)
  *
 **/

 #include <sys/socket.h>
 #include <arpa/inet.h>		// ntohs, htons
 #include <stdexcept>            // runtime_error
 #include <cstring>		// memset
 #include <netdb.h>		// getaddrinfo, freeaddrinfo
 #include <unistd.h>		// close
 /*
 #include <cstddef>
 #include <cstdio>
 
 //#include <sys/types.h>
 */
 #include "VSocket.h"
 
 
 /**
   *  Class creator (constructor)
   *     use Unix socket system call
   *
   *  @param     char t: socket type to define
   *     's' for stream
   *     'd' for datagram
   *  @param     bool ipv6: if we need a IPv6 socket
   *
  **/
 void VSocket::BuildSocket( char t, bool IPv6 ){
 
   int domain, type;

   // Dominio: solo IPv4 por ahora
   domain = AF_INET;

   // Dominio: IPv4 o IPv6
   domain = IPv6 ? AF_INET6 : AF_INET;

   // Tipo: stream o datagram
   switch ( t ) {
       case 's': type = SOCK_STREAM; break;
       case 'd': type = SOCK_DGRAM;  break;
       default:
           throw std::runtime_error( "VSocket::BuildSocket, invalid type" );
   }

   // Crear el socket
   idSocket = socket( domain, type, 0 );
   if ( idSocket == -1 ) {
       throw std::runtime_error( "VSocket::BuildSocket, socket creation failed" );
   }

   // Guardar atributos
   this->IPv6 = IPv6;
   this->type = t;
 
 }
 
 
 /**
   * Class destructor
   *
  **/
 VSocket::~VSocket() {
 
    this->Close();
 
 }
 
 
 /**
   * Close method
   *    use Unix close system call (once opened a socket is managed like a file in Unix)
   *
  **/
 void VSocket::Close(){
   if ( idSocket != -1 ) {
      int st = close( idSocket );
      if ( st == -1 ) {
          throw std::runtime_error( "VSocket::Close(), close failed" );
      }
      idSocket = -1; // Marcar como cerrado
  }
 
 }
 
 
 /**
   * EstablishConnection method
   *   use "connect" Unix system call
   *
   * @param      char * host: host address in dot notation, example "10.84.166.62"
   * @param      int port: process address, example 80
   *
  **/
 int VSocket::EstablishConnection( const char * hostip, int port ) {
 
  if ( IPv6 ) {
    struct sockaddr_in6 server_addr;
    memset( &server_addr, 0, sizeof( server_addr ) );

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons( port );

    int st = inet_pton( AF_INET6, hostip, &server_addr.sin6_addr );
    if ( st <= 0 ) {
        throw std::runtime_error( "VSocket::EstablishConnection (IPv6), invalid IP address" );
    }

    st = connect( idSocket, (struct sockaddr *) &server_addr, sizeof( server_addr ) );
    if ( st == -1 ) {
        throw std::runtime_error( "VSocket::EstablishConnection (IPv6), connect failed" );
    }

    return st;

 } else {  // IPv4
    struct sockaddr_in server_addr;
    memset( &server_addr, 0, sizeof( server_addr ) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port );

    int st = inet_pton( AF_INET, hostip, &server_addr.sin_addr );
    if ( st <= 0 ) {
        throw std::runtime_error( "VSocket::EstablishConnection (IPv4), invalid IP address" );
    }

    st = connect( idSocket, (struct sockaddr *) &server_addr, sizeof( server_addr ) );
    if ( st == -1 ) {
        throw std::runtime_error( "VSocket::EstablishConnection (IPv4), connect failed" );
    }

    return st;
 }
 
 }
 
 
 /**
   * EstablishConnection method
   *   use "connect" Unix system call
   *
   * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
   * @param      char * service: process address, example "http"
   *
  **/
 int VSocket::EstablishConnection( const char *host, const char *service ) {
  struct addrinfo hints, *res;
  memset( &hints, 0, sizeof( hints ) );

  hints.ai_family = IPv6 ? AF_INET6 : AF_INET;  // Seleccionar IPv4 o IPv6
  hints.ai_socktype = ( this->type == 's' ) ? SOCK_STREAM : SOCK_DGRAM;  // Stream o Datagram

  int st = getaddrinfo( host, service, &hints, &res );
  if ( st != 0 ) {
      throw std::runtime_error( "VSocket::EstablishConnection, getaddrinfo failed" );
  }

  st = connect( idSocket, res->ai_addr, res->ai_addrlen );
  freeaddrinfo( res );  // Liberar la memoria

  if ( st == -1 ) {
      throw std::runtime_error( "VSocket::EstablishConnection, connect failed" );
  }

  return st;
 
 }
 
 
 /**
   * Bind method
   *    use "bind" Unix system call (man 3 bind) (server mode)
   *
   * @param      int port: bind a unamed socket to a port defined in sockaddr structure
   *
   *  Links the calling process to a service at port
   *
  **/
 int VSocket::Bind( int port ) {
  if ( IPv6 ) {
    struct sockaddr_in6 server_addr;
    memset( &server_addr, 0, sizeof( server_addr ) );

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;  // Aceptar conexiones desde cualquier dirección IPv6
    server_addr.sin6_port = htons( port );

    int st = bind( idSocket, (struct sockaddr *) &server_addr, sizeof( server_addr ) );
    if ( st == -1 ) {
        throw std::runtime_error( "VSocket::Bind (IPv6), bind failed" );
    }

 } else {  // IPv4
    struct sockaddr_in server_addr;
    memset( &server_addr, 0, sizeof( server_addr ) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );  // Aceptar conexiones desde cualquier dirección IPv4
    server_addr.sin_port = htons( port );

    int st = bind( idSocket, (struct sockaddr *) &server_addr, sizeof( server_addr ) );
    if ( st == -1 ) {
        throw std::runtime_error( "VSocket::Bind (IPv4), bind failed" );
    }
 }

 this->port = port;  // Guardar el puerto en el atributo

 return 0;
 
 }
 
 
 /**
   * MarkPassive method
   *    use "listen" Unix system call (man listen) (server mode)
   *
   * @param      int backlog: defines the maximum length to which the queue of pending connections for this socket may grow
   *
   *  Establish socket queue length
   *
  **/
 int VSocket::MarkPassive( int backlog ) { //Listen...
   int st = listen( idSocket, backlog );
   if ( st == -1 ) {
       throw std::runtime_error( "VSocket::MarkPassive, listen failed" );
   }
   return st;
 
 }
 
 
 /**
   * WaitForConnection method
   *    use "accept" Unix system call (man 3 accept) (server mode)
   *
   *
   *  Waits for a peer connections, return a sockfd of the connecting peer
   *
  **/
 int VSocket::WaitForConnection( void ) {
  if ( IPv6 ) {
    struct sockaddr_in6 client_addr;
    socklen_t addr_len = sizeof( client_addr );

    int client_fd = accept( idSocket, (struct sockaddr *) &client_addr, &addr_len );
    if ( client_fd == -1 ) {
        throw std::runtime_error( "VSocket::WaitForConnection (IPv6), accept failed" );
    }

    return client_fd;

 } else {  // IPv4
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof( client_addr );

    int client_fd = accept( idSocket, (struct sockaddr *) &client_addr, &addr_len );
    if ( client_fd == -1 ) {
        throw std::runtime_error( "VSocket::WaitForConnection (IPv4), accept failed" );
    }

    return client_fd;
 }
 
 }
 
 
 /**
   * Shutdown method
   *    use "shutdown" Unix system call (man 3 shutdown) (server mode)
   *
   *
   *  cause all or part of a full-duplex connection on the socket associated with the file descriptor socket to be shut down
   *
  **/
 int VSocket::Shutdown( int mode ) {
   int st = shutdown( idSocket, mode );
   if ( st == -1 ) {
       throw std::runtime_error( "VSocket::Shutdown, shutdown failed" );
   }
   return st;
 
 }
 
 
 // UDP methods 2025
 
 /**
   *  sendTo method
   *
   *  @param	const void * buffer: data to send
   *  @param	size_t size data size to send
   *  @param	void * addr address to send data
   *
   *  Send data to another network point (addr) without connection (Datagram)
   *
  **/
 size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
  socklen_t addr_len = IPv6 ? sizeof( struct sockaddr_in6 ) : sizeof( struct sockaddr_in );
  ssize_t st = sendto( idSocket, buffer, size, 0, (struct sockaddr *) addr, addr_len );
  if ( st == -1 ) {
      throw std::runtime_error( "VSocket::sendTo, sendto failed" );
  }
  return st;
 
 }
 
 
 /**
   *  recvFrom method
   *
   *  @param	const void * buffer: data to send
   *  @param	size_t size data size to send
   *  @param	void * addr address to receive from data
   *
   *  @return	size_t bytes received
   *
   *  Receive data from another network point (addr) without connection (Datagram)
   *
  **/
 size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
  socklen_t addr_len = IPv6 ? sizeof( struct sockaddr_in6 ) : sizeof( struct sockaddr_in );
  ssize_t st = recvfrom( idSocket, buffer, size, 0, (struct sockaddr *) addr, &addr_len );
  if ( st == -1 ) {
      throw std::runtime_error( "VSocket::recvFrom, recvfrom failed" );
  }
  return st;
 
 }