#ifndef WEB_UTIL_H
#define WEB_UTIL_H

#include <fmt/core.h>
#include <sys/fcntl.h>

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
 * A convenience function to print a format string to standard error and aborts the program.
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

#endif
