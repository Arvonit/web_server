#ifndef WEB_CLIENT_H
#define WEB_CLIENT_H

#include <string>

/**
 * A C++ wrapper around a TCP client socket.
 */
class client_socket {
private:
    int fd;
    client_socket(int fd); // Use named constructors

public:
    /**
     * Move constructor
     */
    client_socket(client_socket&& rhs);

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
    int send(std::string message);

    /**
     * Receives a message of specified size from the remote socket.
     */
    std::pair<std::string, int> recv(int size = 8192);

    /**
     * Closes the socket's file descriptor when it is freed from memory.
     */
    ~client_socket();
};

#endif
