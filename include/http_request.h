#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <sys/types.h>
#include "hashmap.h"
#define ZV_AGAIN EAGAIN
#define HTTP_PARSE_INVALID_METHOD 10
#define HTTP_PARSE_INVALID_REQUEST 11
#define HTTP_PARSE_INVALID_HEADER 12
#define CR '\r'
#define LF '\n'
#define CRLFCRLF "\r\n\r\n"
#define MAX_BUF 8124

#define zv_str3_cmp(m, c0, c1, c2, c3) \
  *(uint32_t *)m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define zv_str3Ocmp(m, c0, c1, c2, c3) \
  *(uint32_t *)m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define zv_str4cmp(m, c0, c1, c2, c3) \
  *(uint32_t *)m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

typedef enum { UNKNOWN, GET, POST, HEAD, PUT, DELETE } HTTP_METHOD;
typedef struct http_request {
  int fd;
  int epfd;
  char buf[MAX_BUF]; /* ring buffer */
  size_t pos, last;
  size_t state;
  void *request_start;
  void *method_end; /* not include method_end*/
  HTTP_METHOD method;
  void *uri_start;
  void *uri_end; /* not include uri_end*/
  void *path_start;
  void *path_end;
  void *query_start;
  void *query_end;
  int http_major;
  int http_minor;
  void *request_end;
  hashmap_t headers;
  void *cur_header_key_start;
  void *cur_header_key_end;
  void *cur_header_value_start;
  void *cur_header_value_end;
} http_request;
typedef http_request *http_request_t;

extern http_request_t create_http_request(int fd, int epfd);
extern ssize_t http_parse_request_line(http_request_t request);
extern ssize_t http_parse_request_body(http_request_t request);
extern ssize_t free_http_request(http_request_t request);

#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__HTTP_REQUEST_H__