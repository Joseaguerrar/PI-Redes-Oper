#include <stdexcept>
#include <cstdio>
#include <unistd.h>

#include "SSLSocket.h"

SSLSocket::SSLSocket(char t, bool IPv6) : Socket(t, IPv6), context(nullptr), ssl(nullptr)
{
    // constructor
}

SSLSocket::SSLSocket(int id) : Socket(id), context(nullptr), ssl(nullptr)
{
    // constructor desde socket aceptado
}

SSLSocket::~SSLSocket()
{
    if (ssl)
        SSL_free(ssl);
    if (context)
        SSL_CTX_free(context);
}

void SSLSocket::InitContext()
{
    const SSL_METHOD *method = TLS_client_method();
    context = SSL_CTX_new(method);
    if (!context)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to create SSL context");
    }
}

void SSLSocket::Init()
{
    InitContext();
    ssl = SSL_new(context);
    if (!ssl)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to create SSL structure");
    }
    SSL_set_fd(ssl, idSocket);
}

void SSLSocket::LoadCertificates(const char *certFile, const char *keyFile)
{
    if (SSL_CTX_use_certificate_file(context, certFile, SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(context, keyFile, SSL_FILETYPE_PEM) <= 0 ||
        !SSL_CTX_check_private_key(context))
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to load SSL certificates");
    }
}

void SSLSocket::InitServerContext()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD *method = TLS_server_method();
    context = SSL_CTX_new(method);
    if (!context)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to create SSL server context");
    }
}

void SSLSocket::InitServer(const char *certFile, const char *keyFile)
{
    InitServerContext();
    ssl = SSL_new(context);
    if (!ssl)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Failed to create SSL structure (server)");
    }
    SSL_set_fd(ssl, idSocket);
    LoadCertificates(certFile, keyFile);
}

void SSLSocket::Accept()
{
    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL handshake failed");
    }
}

const char *SSLSocket::GetCipher()
{
    return SSL_get_cipher(ssl);
}

void SSLSocket::ShowCerts()
{
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (cert)
    {
        char *line;
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
    {
        printf("No certificates.\n");
    }
}

void SSLSocket::CopyContext(SSLSocket *original)
{
    context = original->context;
    ssl = SSL_new(context);
    if (!ssl)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL_new failed in CopyContext");
    }
    SSL_set_fd(ssl, idSocket);
}

size_t SSLSocket::SSLWrite(const void *buffer, size_t size)
{
    int st = SSL_write(ssl, buffer, size);
    if (st <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL_write failed");
    }
    return st;
}

size_t SSLSocket::SSLRead(void *buffer, size_t size)
{
    int st = SSL_read(ssl, buffer, size);
    if (st <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL_read failed");
    }
    return st;
}

void SSLSocket::Close()
{
    if (ssl)
        SSL_shutdown(ssl);
    Socket::Write("", 0); // dummy write to signal EOF
    close(idSocket);
}
