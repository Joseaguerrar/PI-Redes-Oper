#ifndef Socket_h
#define Socket_h

#include "VSocket.h"
#include <cstddef>

class Socket : public VSocket
{ // Socket Class, inherits from VSocket

public:
    // Constructors
    // Default constructor
    Socket();
    Socket(char type, bool IPv6 = false);
    Socket(int id);
    ~Socket();

    // Socket connection methods
    int MakeConnection(const char *host, int port);
    int MakeConnection(const char *host, const char *service);
    int getSocketDescriptor() const;

    // Read and write methods
    size_t Read(void *buffer, size_t size);
    size_t Write(void *buffer, size_t size);
    size_t Write(char *message);

    // Accept method for server socket
    Socket *Accept();

    VSocket *AcceptConnection() override;

protected:
    // Add any private members for basic socket here, if needed.
};

#endif