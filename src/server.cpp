#include "server.h"
#include "client.h"
#include "util.h"
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <stdexcept>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

server_socket server_socket::listen(std::string address, int port) {
    const int backlog = 10;
    int fd, yes;
    struct addrinfo hints, *server_info, *p;

    memset(&hints, 0, sizeof(hints)); // Initialize addrinfo to all zeros
    hints.ai_family = AF_UNSPEC;      // We don't care if it's IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP socket
    hints.ai_flags = AI_PASSIVE;      // Fill in IP for us

    // Call getaddrinfo to populate results into `server_info`
    if (int status =
            getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &server_info);
        status != 0) {
        throw std::runtime_error(fmt::format("gai error: {}", gai_strerror(status)));
    }

    // Search for the correct server info in the linked list and bind to it
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            eprintln("[warning] socket error: {}\n", strerror(errno));
            continue;
        }

        // Clear the pesky "Address already in use" error message
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
            // fprintf(stderr, "[warning] setsockopt error: %s\n", strerror(errno));
            // exit(EXIT_FAILURE);
            throw std::runtime_error(fmt::format("setsockopt error: {}", strerror(errno)));
        }

        if (bind(fd, p->ai_addr, p->ai_addrlen) < 0) {
            close(fd);
            eprintln("[warning] bind error: {}", strerror(errno));
            continue;
        }

        break;
    }

    // If we're at the end of the list, we failed to bind
    if (p == NULL) {
        throw std::runtime_error("Failed to bind socket");
    }

    freeaddrinfo(server_info); // We're done with this struct now

    // Listen for incoming connections
    if (::listen(fd, backlog) < 0) { // :: calls global namespace
        close(fd);
        throw std::runtime_error(fmt::format("listen error: {}", strerror(errno)));
    }

    return server_socket(fd);
}

server_socket server_socket::from_fd(int fd) {
    if (!is_valid_fd(fd)) {
        throw std::runtime_error("File descriptor does not correspond to a valid socket");
    }
    return server_socket(fd);
}

client_socket server_socket::accept() {
    struct sockaddr_storage client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_fd = ::accept(fd, (struct sockaddr *)&client_address, &client_address_size);
    if (client_fd < 0) {
        throw std::runtime_error(fmt::format("accept error: {}", strerror(errno)));
    }

    return client_socket::from_fd(client_fd);
}

server_socket::~server_socket() {
    if (is_valid_fd(fd)) {
        close(fd);
    }
}
