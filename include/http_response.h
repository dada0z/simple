#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus
#include <stddef.h>
#include <sys/types.h>
#include "hashmap.h"

typedef struct http_response {
  int fd;
  int status_code;
  hashmap_t headers;
} http_response;

typedef http_response* http_response_t;

http_response_t create_http_response(int fd);
ssize_t free_http_response(http_response_t response);
const char* get_reason_phrase_from_status_code(size_t status_code);
#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__HTTP_RESPONSE_H__