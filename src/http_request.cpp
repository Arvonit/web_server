#include "http_request.h"
#include "util.h"
#include <algorithm>

http_request::http_request(std::string raw) {
    constexpr auto delimiter = "\r\n"; // Browsers follow CRLF
    auto request_vector = split_str(raw, delimiter);
    auto request_line = request_vector[0];

    auto request_line_tokens = split_str(request_line, " ");
    auto method = request_line_tokens[0];
    auto target = request_line_tokens[1];
    replace(target, "%20", " "); // Browser treats a space as `%20`
    auto version = request_line_tokens[2];

    this->method = method;
    this->target = target;
    this->version = version;
}
