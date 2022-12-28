#include "client.h"
#include "fmt/format.h"
#include "util.h"
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

client_socket client_socket::connect(std::string address, int port) {
    int fd;
    struct addrinfo hints, *server_info, *p;

    // Create a socket
    memset(&hints, 0, sizeof(hints)); // Initialize the struct to all zeros
    hints.ai_family = AF_UNSPEC;      // We don't care if it's IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP socket

    // Call getaddrinfo to populate results into `server_info`
    if (int status =
            getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &server_info);
        status != 0) {
        throw std::runtime_error(fmt::format("{}", gai_strerror(status)));
    }

    // Search for correct server in linked list
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            eprintln("[warning] client socket error: {}", strerror(errno));
            continue;
        }

        if (::connect(fd, p->ai_addr, p->ai_addrlen) < 0) {
            close(fd);
            throw std::runtime_error(fmt::format("connect error: {}", strerror(errno)));
        }

        break;
    }

    // If we're at the end of the list, we failed to connect
    if (p == NULL) {
        throw std::runtime_error("Failed to connect");
    }

    freeaddrinfo(server_info); // We're done with this struct

    return client_socket(fd);
}

client_socket client_socket::from_fd(int fd) {
    if (!is_valid_fd(fd)) {
        throw std::runtime_error("File descriptor does not correspond to a valid socket");
    }
    return client_socket(fd);
}

void client_socket::send(std::string message) {
    if (::send(fd, message.c_str(), message.length(), 0) < 0) {
        throw std::runtime_error(fmt::format("send error: {}", strerror(errno)));
    }
}

std::string client_socket::recv(int size) {
    // char message[1024];
    // memset(message, 0, sizeof(message));
    std::vector<char> message(size, 0);

    if (int bytes = ::recv(fd, &message[0], 1024, 0); bytes < 0) {
        throw std::runtime_error(fmt::format("recv error: {}", strerror(errno)));
    }
    // return std::string(message);
    return std::string(message.begin(), message.end());
}

client_socket::~client_socket() {
    if (is_valid_fd(fd)) {
        close(fd);
        // println("socket (fd: {}) destroyed", fd);
    }
}
