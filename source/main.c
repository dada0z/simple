#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "http_request.h"

#define PORT (8080)
#define MAXEVENTS (1024)

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

int main(void) {
  int listen_fd = -1;
  listen_fd = openListenSocket(PORT);
  if (listen_fd == -1) {
    return -1;
  }
  if (make_socket_non_blocking(listen_fd) == -1) {
    return -1;
  }

  int epfd = epoll_create1(0);
  struct epoll_event events[MAXEVENTS];
  memset(events, 0, sizeof(events));
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = listen_fd;
  event.events = EPOLLIN | EPOLLET;
  epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &event);

  int n = -1, fd = -1, i = 0, infd = -1;
  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t inlen = 1;
  while (1) {
    n = epoll_wait(epfd, events, MAXEVENTS, -1);
    for (i = 0; i < n; i++) {
      fd = events[i].data.fd;
      if (listen_fd == fd) {
        /* we hava one or more incoming connections */
        while (1) {
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
          event.data.ptr = create_http_request(infd, epfd, "/home/dada/Code/");
          event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
          epoll_ctl(epfd, EPOLL_CTL_ADD, infd, &event);
        }

      } else {
        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
            (!(events[i].events & EPOLLIN))) {
          close(fd);
          continue;
        }
      }
    }
  }

  return 0;
}
