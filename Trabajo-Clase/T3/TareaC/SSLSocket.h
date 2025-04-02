/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
  *
  *   SSL Socket class interface
  *
  * (Fedora version)
  *
 **/

#ifndef SSLSocket_h
#define SSLSocket_h

#include "VSocket.h"
#include <openssl/types.h>


class SSLSocket : public VSocket {

   public:
      SSLSocket( bool IPv6 = false );				// Not possible to create with UDP, client constructor
      SSLSocket( char *, char *, bool = false );		// For server connections
      SSLSocket( int );
      ~SSLSocket();
      int MakeConnection( const char *, int );
      int MakeConnection( const char *, const char * );
      size_t Write( const char * );
      size_t Write( const void *, size_t );
      size_t Read( void *, size_t );
      void ShowCerts();
      const char * GetCipher();
      
      void Init( bool = false );		// Defaults to create a client context, true if server context needed
      void LoadCertificates( const char *, const char * );

      SSL * GetSSLStruct() {
         return reinterpret_cast<SSL *>(SSLStruct);
      }

   private:
      void InitContext( bool );

// Instance variables      
      void * SSLContext;				// SSL context
      void * SSLStruct;					// SSL BIO (Basic Input/Output)

};

#endif

