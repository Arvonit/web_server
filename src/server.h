#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "client.h"
#include <string>

/**
 * An C++ wrapper around a TCP server socket.
 */
class server_socket {
private:
    int fd;
    server_socket(int fd); // Use named constructors

public:
    server_socket(server_socket&& rhs);            // Move constructor
    server_socket& operator=(server_socket&& rhs); // Move assignment

    server_socket(server_socket& rhs) = delete;            // Disable copy functionality
    server_socket& operator=(server_socket& rhs) = delete; //

    /**
     * Creates a socket to listen on the given address and port. Throws an exception if there was
     * a problem in creating the socket.
     */
    static server_socket listen(std::string address, int port);

    /**
     * Creates a socket from a given file descriptor.
     */
    static server_socket from_fd(int fd);

    /**
     * Accepts a new connection to the socket.
     */
    client_socket accept();

    /**
     * Closes the socket's file descriptor when it is freed from memory.
     */
    ~server_socket();
};

#endif
