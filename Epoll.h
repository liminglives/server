#ifndef EPOLL_H
#define EPOLL_H

#include "Declear.h"


#include <sys/epoll.h>
#include <vector>

#define MAX_EPOLL_FD          2
#define MAX_EPOLL_EVENTS  100

class Epoll
{
public:
    Epoll();
    ~Epoll();
    void update(Channel *pChannel);
    void poll(std::vector<Channel * > &vecChannel);
private:
    int epollfd;
    struct epoll_event  revents[MAX_EPOLL_EVENTS];
};

#endif
