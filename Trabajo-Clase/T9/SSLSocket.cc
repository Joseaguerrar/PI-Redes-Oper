#include "SSLSocket.h"
#include "Socket.h"
#include <stdexcept>
#include <iostream>
#include <cstring> // For memset
#include <openssl/ssl.h>

// Constructor
SSLSocket::SSLSocket() : ssl(nullptr), sslContext(nullptr), username(nullptr), password(nullptr) {}

SSLSocket::SSLSocket(char type, bool IPv6) : Socket(type, IPv6), ssl(nullptr), sslContext(nullptr)
{
    // You can initialize SSL-related members or setup here
}

// Destructor
SSLSocket::~SSLSocket()
{
    SSLCleanup(); // Cleanup SSL resources
}

// Initializes SSL context for server-side
void SSLSocket::SSLInitServer(const char *certFile, const char *keyFile)
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    sslContext = SSL_CTX_new(SSLv23_server_method());
    if (!sslContext)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to create SSL context");
    }

    // Load the certificate and private key files
    if (SSL_CTX_use_certificate_file(sslContext, certFile, SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(sslContext, keyFile, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Error setting up SSL certificate or private key");
    }
}

// Create SSL object for the server socket
void SSLSocket::SSLCreate(SSLSocket *serverSocket)
{
    ssl = SSL_new(serverSocket->sslContext);
    if (!ssl)
    {
        throw std::runtime_error("Failed to create SSL object");
    }
    SSL_set_fd(ssl, idSocket);
}

// Accept SSL connection
void SSLSocket::SSLAccept()
{
    if (SSL_accept(ssl) <= 0)
    {
        throw std::runtime_error("SSL handshake failed (SSL_accept).");
    }
}

// SSL read method
int SSLSocket::SSLRead(char *buffer, int bufferSize)
{
    int bytesRead = SSL_read(ssl, buffer, bufferSize);
    if (bytesRead <= 0)
    {
        throw std::runtime_error("SSL read operation failed.");
    }
    return bytesRead;
}

// SSL write method
int SSLSocket::SSLWrite(const char *message, int length)
{
    int bytesWritten = SSL_write(ssl, message, length);
    if (bytesWritten <= 0)
    {
        throw std::runtime_error("SSL write operation failed.");
    }
    return bytesWritten;
}

// Show SSL certificates
void SSLSocket::SSLShowCerts()
{
    X509 *cert = SSL_get_peer_certificate(ssl); // Get client certificate
    if (cert)
    {
        char *line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        std::cout << "Client certificate: " << line << std::endl;
        OPENSSL_free(line);

        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        std::cout << "Issuer: " << line << std::endl;
        OPENSSL_free(line);
        X509_free(cert);
    }
    else
    {
        std::cout << "Client did not provide a certificate." << std::endl;
    }
}

// Get SSL cipher in use
const char *SSLSocket::SSLGetCipher()
{
    if (!ssl)
    {
        throw std::runtime_error("SSL object is not initialized.");
    }
    return SSL_get_cipher(ssl);
}

// SSL Cleanup
void SSLSocket::SSLCleanup()
{
    if (ssl)
    {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = nullptr;
    }

    if (sslContext)
    {
        SSL_CTX_free(sslContext);
        sslContext = nullptr;
    }
}

// Initializes SSL for client-side connections
void SSLSocket::SSLInit()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    sslContext = SSL_CTX_new(TLS_client_method());
    if (!sslContext)
    {
        throw std::runtime_error("Failed to create SSL context.");
    }
}

// Connects to server with SSL
void SSLSocket::SSLConnect(char *hostname, int port)
{
    // First establish a TCP connection to the server
    if (this->MakeConnection(hostname, port) < 0)
    {
        throw std::runtime_error("Failed to connect to the server");
    }

    // Create a new SSL object for this connection
    ssl = SSL_new(sslContext);
    if (!ssl)
    {
        throw std::runtime_error("Failed to create SSL object");
    }

    // Bind the socket to the file descriptor
    SSL_set_fd(ssl, idSocket);

    // Perform the SSL connection (performs the SSL handshake)
    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL_connect failed");
    }
}

// Load certificates for SSL connection
void SSLSocket::LoadCertificates(const char *certFile, const char *keyFile)
{
    // Load certificate
    if (SSL_CTX_use_certificate_file(sslContext, certFile, SSL_FILETYPE_PEM) <= 0)
    {
        throw std::runtime_error("Failed to load SSL certificate.");
    }

    // Load private key
    if (SSL_CTX_use_PrivateKey_file(sslContext, keyFile, SSL_FILETYPE_PEM) <= 0)
    {
        throw std::runtime_error("Failed to load SSL private key.");
    }

    // Verify private key
    if (!SSL_CTX_check_private_key(sslContext))
    {
        throw std::runtime_error("Private key does not match the public certificate.");
    }
}