#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "event.h"
#include "http.h"
#include "http_request.h"

#define PORT (8080)

ssize_t make_socket_non_blocking(int fd) {
  int flags = -1;
  flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }

  flags |= O_NONBLOCK;
  flags = fcntl(fd, F_SETFL, flags);
  if (flags == -1) {
    return -1;
  }

  return 0;
}

int openListenSocket(unsigned short port) {
  int fd = -1, optval = 1;
  struct sockaddr_in addr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
                 sizeof(int)) < 0) {
    return -1;
  }

  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    return -1;
  }

  if (listen(fd, SOMAXCONN) < 0) {
    return -1;
  }

  return fd;
}

static void handle_accept(int epollfd, int listen_fd) {
  int infd = 0;
  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t inlen = 1;
  /* we hava one or more incoming connections */
  while (true) {
    infd = accept(listen_fd, (struct sockaddr *)&client_addr, &inlen);
    if (infd < 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        /* we have processed all incoming connections */
        break;
      } else {
        break;
      }
    }

    make_socket_non_blocking(infd);
    add_event(epollfd, infd, EPOLLIN | EPOLLET | EPOLLONESHOT);
  }
}

static void handle_events(int epollfd, int listen_fd, int event_num) {
  http_request_t request = NULL;
  event_t event = NULL;
  for (size_t i = 0; i < event_num; i++) {
    event = get_event(i);
    request = (http_request_t)event->data.ptr;
    if (listen_fd == request->fd) {
      handle_accept(epollfd, listen_fd);
    } else {
      if ((event->events & EPOLLERR) || (event->events & EPOLLHUP) ||
          (!(event->events & EPOLLIN))) {
        close(request->fd);
        continue;
      }
      do_request(request, create_http_response(request->fd));
    }
  }
}

static void do_epoll(int listen_fd) {
  int epoll_fd = create_events();
  add_event(epoll_fd, listen_fd, EPOLLIN | EPOLLET);
  int event_num = -1;
  while (true) {
    event_num = wait_events(epoll_fd);
    handle_events(epoll_fd, listen_fd, event_num);
  }
}

int main(void) {
  int listen_fd = -1;
  listen_fd = openListenSocket(PORT);
  if (listen_fd == -1) {
    return -1;
  }
  if (make_socket_non_blocking(listen_fd) == -1) {
    return -1;
  }
  do_epoll(listen_fd);
  return 0;
}
