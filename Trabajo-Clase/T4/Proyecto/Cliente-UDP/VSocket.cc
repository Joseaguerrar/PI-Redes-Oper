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
#include <netdb.h>			// getaddrinfo, freeaddrinfo
#include <unistd.h>			// close
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

   this->IPv6 = IPv6;
   this->type = t;
   
   int domain = (IPv6) ? AF_INET6 : AF_INET;
   int socketType = (t == 's') ? SOCK_STREAM : SOCK_DGRAM;

   this->idSocket = socket( domain, socketType, 0 );
   if ( -1 == this->idSocket ) {
      throw std::runtime_error( "VSocket::BuildSocket(), error al crear socket" );
   }
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
   if (close(this->idSocket) == -1) {
      throw std::runtime_error( "VSocket::Close(), error al cerrar socket" );
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
   if (this->type != 's') { // Checkear que no sea UDP
      throw std::runtime_error("VSocket::EstablishConnection(), no válido para UDP");
   }
   struct sockaddr_in server_addr; // Estructura para dirección del servidor
   memset( &server_addr, 0, sizeof(server_addr) ); // Limpiar estructura
   server_addr.sin_family = AF_INET; // IPv4
   server_addr.sin_port = htons( port ); 

   if ( inet_pton( AF_INET, hostip, &server_addr.sin_addr ) <= 0 ) {
      perror( "inet_pton error" ); 
      throw std::runtime_error( "VSocket::EstablishConnection(), error en inet_pton" ); // Error inet_pton
   }

   if ( connect( this->idSocket, (struct sockaddr *) &server_addr, sizeof(server_addr) ) == -1 ) {
      perror( "connect error" );
      throw std::runtime_error( "VSocket::EstablishConnection(), error al conectar" );  // Error connect
   }
   return 0;

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
   if (this->type != 's') { // Checkear que no sea UDP
      throw std::runtime_error("VSocket::EstablishConnection(), no válido para UDP");
   }
   struct addrinfo hints, *res, *rp;
    
    // Limpiar estructura hints y definir criterios de búsqueda
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = this->IPv6 ? AF_INET6 : AF_INET;  // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Obtener información de la dirección usando getaddrinfo
    int status = getaddrinfo(host, service, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1; // Retornar -1 si hubo un error en la resolución del host
    }

    // Iterar sobre todas las direcciones devueltas y tratar de conectar
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        if (connect(this->idSocket, rp->ai_addr, rp->ai_addrlen) == 0) {
            freeaddrinfo(res);
            return 0;  // Conexión exitosa
        }
    }

    // Si llegamos aquí, ninguna dirección funcionó
    freeaddrinfo(res);
    return -1; // Indicar error si no se pudo conectar a ninguna dirección

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
   int st = -1;  // Indicar error si no se pudo conectar a ninguna dirección
   if (this->IPv6) {
      struct sockaddr_in6 addr6;
      memset(&addr6, 0, sizeof(addr6));
      addr6.sin6_family = AF_INET6;
      addr6.sin6_port = htons(port);
      addr6.sin6_addr = in6addr_any;  // Cualquier interfaz IPv6

      if (bind(this->idSocket, (struct sockaddr*)&addr6, sizeof(addr6)) == -1) {
          perror("VSocket::Bind() IPv6");
          throw std::runtime_error("Error en bind() IPv6");
      } else {
          st = 0;
      }
   } else {
         struct sockaddr_in addr4;
         memset(&addr4, 0, sizeof(addr4));
         addr4.sin_family = AF_INET;
         addr4.sin_port = htons(port);
         addr4.sin_addr.s_addr = INADDR_ANY;  // Cualquier interfaz IPv4

         if (bind(this->idSocket, (struct sockaddr*)&addr4, sizeof(addr4)) == -1) {
            perror("VSocket::Bind() IPv4");
            throw std::runtime_error("Error en bind() IPv4");
         } else {
            st = 0;
         }
   }

   this->port = port;  // Guarda el puerto en la clase por si lo necesita después
   return st;

}


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
   if (addr == nullptr) { // No se puede enviar a una dirección nula
      throw std::runtime_error("VSocket::sendTo(), dirección destino nula");
   }
   ssize_t sentBytes = sendto(
      this->idSocket,                  // socket descriptor
      buffer,                          // datos a enviar
      size,                            // tamaño de los datos
      0,                               // flags (normalmente 0)
      (struct sockaddr *)addr,        // dirección destino
      (this->IPv6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in)) // tamaño dirección
  );

  if (sentBytes == -1) {
      perror("VSocket::sendTo()");
      throw std::runtime_error("Error en sendto()");
  }

  return static_cast<size_t>(sentBytes);

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
   if (addr == nullptr) {  // No se puede recibir desde una dirección nula
      throw std::runtime_error("VSocket::recvFrom(), dirección origen nula");
   }

   socklen_t addrLen = this->IPv6 ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);

   ssize_t receivedBytes = recvfrom(
      this->idSocket,                  // socket descriptor
      buffer,                          // buffer donde se guardará el mensaje
      size,                            // tamaño del buffer
      0,                               // flags
      (struct sockaddr *)addr,        // dirección origen (remitente)
      &addrLen                         // puntero al tamaño de la dirección
   );

   if (receivedBytes == -1) {
      perror("VSocket::recvFrom()");
      throw std::runtime_error("Error en recvfrom()");
   }

   return static_cast<size_t>(receivedBytes);

}

