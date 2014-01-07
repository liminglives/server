#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel *_pChannel);
private:
    Epoll *pPoller;
};

#endif
