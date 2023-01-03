#ifndef WEB_UTIL_H
#define WEB_UTIL_H

#include <fmt/core.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <sys/fcntl.h>
#include <vector>

// Include the fmt::print function in the current namespace
using fmt::print;

/**
 * A convenience function to print format strings with a new line at the end. This simply adds
 * `\n` to the end of a `fmt::print` call.
 */
template <typename... T>
inline void println(fmt::format_string<T...>&& fmt, T&&... args) {
    print("{}\n",
          fmt::format(std::forward<fmt::format_string<T...>>(fmt), std::forward<T>(args)...));
}

/**
 * A convenience function to print a format string to standard error.
 */
template <typename... T>
inline void eprintln(fmt::format_string<T...>&& fmt, T&&... args) {
    print(stderr,
          "{}\n",
          fmt::format(std::forward<fmt::format_string<T...>>(fmt), std::forward<T>(args)...));
}

inline bool is_valid_fd(int fd) {
    return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}

inline std::vector<std::string> split_str(std::string s, std::string delimiter) {
    std::vector<std::string> out;
    size_t last = 0;
    size_t next = 0;

    while ((next = s.find(delimiter, last)) != std::string::npos) {
        out.push_back(s.substr(last, next - last));
        last = next + 1;
    }
    out.push_back(s.substr(last));

    return out;
}

inline std::string read_file(std::string path) {
    // Throw exception if file does not exist
    if (!std::filesystem::exists(path)) {
        throw std::invalid_argument("File not found");
    }

    constexpr size_t read_size = 4096;
    std::ifstream file_stream(path);
    file_stream.exceptions(std::ios_base::badbit); // Throw exception if there is an error reading

    // Read file 4096 bytes at a time into `buf` string and append that to `out` string
    std::string out;
    std::string buf(read_size, '\0');
    while (file_stream.read(buf.data(), read_size)) {
        out.append(buf, 0, file_stream.gcount()); // `gcount()` returns number of chars read
    }
    out.append(buf, 0, file_stream.gcount());

    return out;
}

// Create an HTML response page similar to nginx
inline std::string create_html_error_page(std::string error) {
    return fmt::format(R"(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>{}</title
  </head>
  <body>
    <h1 style="text-align: center">{}</h1>
    <hr>
    <p style="text-align:center">web_server 1.0</p>
  </body>
</html>)",
                       error,
                       error);
}

inline std::string create_http_response(std::string status,
                                        std::optional<std::string> content = std::nullopt) {
    return fmt::format("HTTP/1.1 {}\r\nserver: web_server\r\n\r\n{}",
                       status,
                       content.value_or(create_html_error_page(status)));
}

#endif
