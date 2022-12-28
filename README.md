# web_server

A pretty basic HTTP server written in C++. Handles the most fundamental request of retrieving a
desired file for a client.

This server is architected to handle requests with a thread-per-connection model, which is 
obviously not ideal but good enough for a toy project. I also developed a C++ wrapper (with RAII
I think) around the Unix socket in order to make it easier to write the server's logic. I hope 
to extend this project in the future to use a thread pool or async IO, perhaps using 
`io_uring` or `libuv`.

To build the project, run `cmake -B build` to generate the build files in a folder called `build`,
followed by `cmake --build build` to compile the executable. You can run the server with 
`./build/web [port]`, which listens on port 80 by default.
