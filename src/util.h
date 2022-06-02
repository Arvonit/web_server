#ifndef UTIL_H
#define UTIL_H

#include <fmt/core.h>

namespace util
{

/**
 * A convience function to print format strings with a new line at the end. This simply adds `\n`
 * to the end of a `fmt::print` call.
 */
template <typename... T>
inline void println(fmt::format_string<T...> &&fmt_str, T &&...args)
{
    fmt::print("{}\n",
               fmt::format(std::forward<fmt::format_string<T...>>(fmt_str),
                           std::forward<T>(args)...));
}

template <typename... T>
inline void eprintln(fmt::format_string<T...> &&fmt_str, T &&...args)
{
    fmt::print(stderr,
               "{}\n",
               fmt::format(std::forward<fmt::format_string<T...>>(fmt_str),
                           std::forward<T>(args)...));
}

} // namespace util

#endif
