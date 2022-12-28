#ifndef WEB_CLIENT_H
#define WEB_CLIENT_H

#include <string>

/**
 * Client sockets are also implemented with move semantics so that they cannot be copied when
 * called to a function.
 */
class client_socket {
private:
    int fd;
    client_socket(int fd) : fd(fd) {
    }

public:
    /**
     * Move constructor
     */
    client_socket(client_socket&& r) : fd(r.fd) {
        // Invalidate previous socket's fd so that it does not close when destroyed
        r.fd = -1;
    }

    /**
     * Creates a socket to connect to the given address and port.
     */
    static client_socket connect(std::string address, int port);

    /**
     * Creates a socket from a given file descriptor.
     */
    static client_socket from_fd(int fd);

    /**
     * Sends a message to the remote socket.
     */
    void send(std::string message);

    /**
     * Receives a message of specified size from the remote socket.
     */
    std::string recv(int size = 8192);

    /**
     * Closes the socket's file descriptor when it is freed from memory.
     */
    ~client_socket();
};

#endif
