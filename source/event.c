#include "event.h"
#include <sys/epoll.h>
#include "http_request.h"

#define MAXEVENTS (1024)

static struct epoll_event events[MAXEVENTS] = {0};
event_t get_event(size_t index) {
  if (index >= MAXEVENTS) {
    return NULL;
  }
  return &events[index];
}
int create_events() { return epoll_create1(0); }
void add_event(int epoll_fd, int fd, int state) {
  struct epoll_event event;
  event.events = state;
  event.data.ptr = create_http_request(fd, epoll_fd);
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

void delete_event(int epoll_fd, int fd, int state) {
  struct epoll_event event;
  event.events = state;
  event.data.fd = fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
}

void modify_event(int epoll_fd, int fd, int state) {
  struct epoll_event event;
  event.events = state;
  event.data.fd = fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
}

int wait_events(int epoll_fd) {
  return epoll_wait(epoll_fd, events, MAXEVENTS, -1);
}