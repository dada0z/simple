#ifndef __HTTP_H__
#define __HTTP_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus
#include "http_request.h"
#include "http_response.h"
void do_request(http_request_t request, http_response_t response);
#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__HTTP_H__