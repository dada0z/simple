#include "http_response.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  size_t status_code;
  const char* reason_phrase;
} http_status_t;
http_status_t statusMap[] = {{100, "Continue"},
                             {101, "Switching Protocols"},
                             {200, "OK"},
                             {201, "Created"},
                             {202, "Accepted"},
                             {203, "Non-Authoritative Information"},
                             {204, "No Content"},
                             {205, "Reset Content"},
                             {206, "Partial Content"},
                             {300, "Multiple Choices"},
                             {301, "Moved Permanently"},
                             {302, "Found"},
                             {303, "See Other"},
                             {304, "Not Modified"},
                             {305, "Use Proxy"},
                             {307, "Temporary Redirect"},
                             {400, "Bad Request"},
                             {401, "Unauthorized"},
                             {402, "Payment Required"},
                             {403, "Forbidden"},
                             {404, "Not Found"},
                             {405, "Method Not Allowed"},
                             {406, "Not Acceptable"},
                             {407, "Proxy Authentication Required"},
                             {408, "Request Time-out"},
                             {409, "Conflict"},
                             {410, "Gone"},
                             {411, "Length Required"},
                             {412, "Precondition Failed"},
                             {413, "Request Entity Too Large"},
                             {414, "Request-URI Too Large"},
                             {415, "Unsupported Media Type"},
                             {416, "Requested range not satisfiable"},
                             {417, "Expectation Failed"},
                             {500, "Internal Server Error"},
                             {501, "Not Implemented"},
                             {502, "Bad Gateway"},
                             {503, "Service Unavailable"},
                             {504, "Gateway Time-out"},
                             {505, "HTTP Version not supported"}};
const char* get_reason_phrase_from_status_code(size_t status_code) {
  int i;
  size_t len = sizeof(statusMap) / sizeof(statusMap[0]);
  for (i = 0; i < len; ++i) {
    if (status_code == statusMap[i].status_code)
      return statusMap[i].reason_phrase;
  }
  return NULL;
}

http_response_t create_http_response(int fd) {
  http_response_t response = (http_response_t)malloc(sizeof(http_response));
  memset(response, 0, sizeof(http_response));
  response->fd = fd;
  response->headers = hashmap_new();
  return response;
}

ssize_t free_http_response(http_response_t response) {
  if (response->fd != -1) {
    close(response->fd);
    response->fd = -1;
  }

  if (response->headers) {
    hashmap_free(response->headers);
  }

  if (response) {
    free(response);
  }

  return 0;
}