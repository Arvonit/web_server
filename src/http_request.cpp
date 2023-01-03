#include "http_request.h"
#include "util.h"
#include <algorithm>

/**
 * Replaces `%20` in target string with space.
 */
void sanitize(std::string& str) {
    std::string x = "%20";
    std::string y = " ";

    size_t pos = 0;
    while (pos += y.length()) {
        pos = str.find(x, pos);
        if (pos == std::string::npos) {
            break;
        }
        str.replace(pos, x.length(), y);
    }
}

http_request::http_request(std::string raw) {
    constexpr auto delimiter = "\r\n"; // Browsers follow CRLF
    auto request_vector = split_str(raw, delimiter);
    auto request_line = request_vector[0];

    auto request_line_tokens = split_str(request_line, " ");
    auto method = request_line_tokens[0];
    auto target = request_line_tokens[1];
    sanitize(target);
    auto version = request_line_tokens[2];

    this->method = method;
    this->target = target;
    this->version = version;
}
