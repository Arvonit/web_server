#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "client.h"
#include <string>

// TODO: Implement move semantics

/**
 * An object-oriented, C++ wrapper around a TCP socket. Handles the lifetime of a socket and
 * contains methods to listen and accept incoming connections. Interface is based on Python's
 * `socket`.
 */
class server_socket {
private:
    int fd;
    server_socket(int fd) : fd(fd) {
    }

public:
    /**
     * Move constructor
     */
    server_socket(server_socket&& r) : fd(r.fd) {
        // Invalid old socket's file descriptor so that it does not get closed when it is
        // destroyed
        r.fd = -1;
    }

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
