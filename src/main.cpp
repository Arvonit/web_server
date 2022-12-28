#include "client.h"
#include "server.h"
#include "util.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

void handle_connection(client_socket client) {
    try {
        auto message = client.recv();
        print("<client> {}", message);
        client.send(message);
    } catch (std::exception& e) {
        println("{}", e.what());
    }
}

/**
 * A basic HTTP web server that returns a requested file or returns an error if not found. Uses
 * the thread-per-connection model to handle requests and is not asynchoronous.
 *
 * TODO:
 * - Implement HTTP part of the server
 * - Consider using `std::expected` type instead of exceptions
 */
int main(int argc, char* argv[]) {
    if (argc > 2) {
        println("Usage: web [port]");
        std::exit(1);
    }

    // Check if CLI argument is valid
    int port = 80;
    if (argc == 2) {
        try {
            port = std::stoi(argv[1]);
        } catch (std::invalid_argument& e) {
            println("Usage: web [port]");
            std::exit(1);
        }
    }

    try {
        server_socket socket = server_socket::listen("localhost", port);

        while (true) {
            std::thread handler(handle_connection, socket.accept());
            handler.detach();
        }
    } catch (std::exception& e) {
        println("{}", e.what());
    }
}
