#ifndef __EVENT_H__
#define __EVENT_H__
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

#include <stddef.h>

typedef struct epoll_event* event_t;

event_t get_event(size_t index);
int create_events();
void add_event(int epoll_fd, int fd, int state);
void delete_event(int epoll_fd, int fd, int state);
void modify_event(int epoll_fd, int fd, int state);
int wait_events(int epoll_fd);

#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  //__EVENT_H__