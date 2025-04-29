/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2025-i
 *  Grupos: 1 y 3
 *
 *  ******   Socket class implementation
 *
 *  (Fedora version)
 **/

#include <sys/socket.h> // socket, sockaddr_in
#include <arpa/inet.h>  // htons, inet_pton
#include <unistd.h>     // write, read, close
#include <cstring>      // strlen, memset
#include <stdexcept>    // runtime_error
#include <cstdio>       // printf
#include <netdb.h>      // getaddrinfo

#include "Socket.h" // Socket header

/**
 * Constructor principal
 *
 * @param char t: tipo ('s' = stream, 'd' = datagram)
 * @param bool IPv6: true si se quiere IPv6
 **/
Socket::Socket(char t, bool IPv6)
{
  this->BuildSocket(t, IPv6);
}

/**
 * Constructor con descriptor de socket ya aceptado
 *
 * @param int id: descriptor del socket
 **/
Socket::Socket(int id)
{
  this->idSocket = id;
  this->IPv6 = false; // Solo manejamos IPv4 por ahora
  this->type = 's';   // Tipo stream por defecto
}

/**
 * Destructor
 **/
Socket::~Socket()
{
  // No liberamos aquí explícitamente, lo maneja quien herede
}

/**
 * Crea el socket del sistema operativo
 *
 * @param char t: tipo ('s' = stream, 'd' = datagram)
 * @param bool IPv6: true si se quiere IPv6
 **/
void Socket::BuildSocket(char t, bool IPv6)
{
  this->IPv6 = IPv6;
  this->type = t;

  int domain = IPv6 ? AF_INET6 : AF_INET;
  int tipo = (t == 'd') ? SOCK_DGRAM : SOCK_STREAM;

  idSocket = socket(domain, tipo, 0);
  if (idSocket < 0)
  {
    throw std::runtime_error("Error creando socket");
  }
}

/**
 * Establece conexión usando dirección IP y puerto
 *
 * @param hostip dirección IP (ej. "192.168.1.1")
 * @param port número de puerto
 * @return 0 si la conexión fue exitosa
 **/
int Socket::EstablishConnection(const char *hostip, int port)
{
  struct sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);

  if (inet_pton(AF_INET, hostip, &serverAddress.sin_addr) <= 0)
  {
    throw std::runtime_error("Invalid IP address");
  }

  if (connect(idSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    throw std::runtime_error("Connection failed");
  }

  return 0;
}

/**
 * Establece conexión usando nombre de host y nombre de servicio
 *
 * @param host nombre DNS (ej. "os.ecci.ucr.ac.cr")
 * @param service nombre de servicio (ej. "http")
 * @return 0 si la conexión fue exitosa
 **/
int Socket::EstablishConnection(const char *host, const char *service)
{
  struct addrinfo hints{}, *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // IPv4 o IPv6
  hints.ai_socktype = (type == 'd') ? SOCK_DGRAM : SOCK_STREAM;

  int status = getaddrinfo(host, service, &hints, &res);
  if (status != 0)
  {
    throw std::runtime_error("getaddrinfo failed");
  }

  if (connect(idSocket, res->ai_addr, res->ai_addrlen) < 0)
  {
    freeaddrinfo(res);
    throw std::runtime_error("Connection failed");
  }

  freeaddrinfo(res);
  return 0;
}

/**
 * Método público para establecer conexión usando IP y puerto
 *
 * @param hostip dirección IP
 * @param port número de puerto
 * @return 0 si la conexión fue exitosa
 **/
int Socket::MakeConnection(const char *hostip, int port)
{
  return this->EstablishConnection(hostip, port);
}

/**
 * Método público para establecer conexión usando host y servicio
 *
 * @param host nombre DNS
 * @param service nombre del servicio
 * @return 0 si la conexión fue exitosa
 **/
int Socket::MakeConnection(const char *host, const char *service)
{
  return this->EstablishConnection(host, service);
}

/**
 * Lee datos desde el socket
 *
 * @param buffer puntero al búfer
 * @param size tamaño del búfer
 * @return número de bytes leídos
 **/
size_t Socket::Read(void *buffer, size_t size)
{
  ssize_t st = read(idSocket, buffer, size);
  if (st == -1)
  {
    throw std::runtime_error("Socket::Read, fallo en read()");
  }
  return st;
}

/**
 * Escribe datos al socket desde un búfer
 *
 * @param buffer datos a escribir
 * @param size tamaño de los datos
 * @return número de bytes escritos
 **/
size_t Socket::Write(const void *buffer, size_t size)
{
  ssize_t st = write(idSocket, buffer, size);
  if (st == -1)
  {
    throw std::runtime_error("Socket::Write, fallo en write()");
  }
  return st;
}

/**
 * Escribe una cadena de texto al socket
 *
 * @param text texto a escribir
 * @return número de bytes escritos
 **/
size_t Socket::Write(const char *text)
{
  return this->Write(text, strlen(text) + 1); // +1 para incluir el '\0'
}
