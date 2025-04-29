/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2025-i
 *  Grupos: 1 y 3
 *
 ****** Socket class interface
 *
 * (Fedora version)
 *
 **/

#ifndef Socket_h
#define Socket_h

#include <cstddef>

class Socket
{

public:
   Socket(char t = 's', bool IPv6 = false);
   Socket(int); // Constructor adicional para aceptar un descriptor
   virtual ~Socket();

   int MakeConnection(const char *, int);
   int MakeConnection(const char *, const char *);

   size_t Read(void *, size_t);
   size_t Write(const void *, size_t);
   size_t Write(const char *);

protected:
   int idSocket;
   bool IPv6;
   char type;

   void BuildSocket(char t, bool IPv6); // Crear el socket
   int EstablishConnection(const char *hostip, int port);
   int EstablishConnection(const char *host, const char *service);
};

#endif