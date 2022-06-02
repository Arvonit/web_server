// A simple echo server at port 80

#include "asio/io_context.hpp"
#include "asio/ip/address.hpp"
#include "util.h"
#include <asio.hpp>
#include <exception>
#include <iostream>
#include <thread>

#define MAX_LENGTH 1024

void handle_client(asio::ip::tcp::socket socket)
{
    try {
        while (true) {
            char data[MAX_LENGTH];
            asio::error_code error;
            auto length = socket.read_some(asio::buffer(data), error);

            // End connection if EOF is sent; if there is any other error, raise an exception
            if (error == asio::error::eof) {
                util::println("Connection closed.");
                break;
            } else if (error) {
                throw asio::system_error(error);
            }

            asio::write(socket, asio::buffer(data, length));
        }
    } catch (std::exception& e) {
        util::eprintln("Exception in thread: {}", e.what());
    }
}

int main()
{
    asio::io_context context;
    asio::ip::tcp::acceptor server(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 80));
    try {
        while (true) {
            std::thread(handle_client, server.accept()).detach();
        }
    } catch (std::exception& e) {
        util::eprintln("Exception: {}", e.what());
    }
}
