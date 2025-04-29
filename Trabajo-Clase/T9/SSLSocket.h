#ifndef SSLSOCKET_H
#define SSLSOCKET_H

#include "Socket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

class SSLSocket : public Socket
{
private:
   SSL_CTX *context;
   SSL *ssl;

public:
   SSLSocket(char t = 's', bool IPv6 = false); // Igual que Socket
   SSLSocket(int id);                          // Para sockets aceptados
   virtual ~SSLSocket();

   void InitContext(); // TLS_client_method
   void Init();        // SSL_new con el contexto
   void LoadCertificates(const char *certFile, const char *keyFile);

   void InitServerContext(); // TLS_server_method
   void InitServer(const char *certFile, const char *keyFile);

   void Accept();           // SSL_accept
   const char *GetCipher(); // SSL_get_cipher
   void ShowCerts();        // mostrar certificado

   void CopyContext(SSLSocket *original); // Copiar contexto
   size_t SSLWrite(const void *buffer, size_t size);
   size_t SSLRead(void *buffer, size_t size);
   void Close();
};

#endif
