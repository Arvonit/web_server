#ifndef WEB_HTTP_REQUEST_H
#define WEB_HTTP_REQUEST_H

#include <string>

/**
 * A structure containing the fields of an HTTP request. For the purposes of this basic server, it
 * only parses the request line.
 */
struct http_request {
    std::string method;
    std::string target;
    std::string version;

    http_request(std::string raw);
};

#endif
