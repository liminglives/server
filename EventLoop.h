#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"
#include "IFChannelCallBack.h"

#include <vector>

class EventLoop : public IFChannelCallBack
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel *_pChannel);
    virtual void handleRead();
    virtual void handleWrite();
    void queueLoop(IFRun *pRun);
    void wakeup();
    void handlePendingRuns();
    
private:
    Epoll *pPoller;
    int ieventfd;
    int createEventfd();
    Channel *pWakeupChannel;
    std::vector<IFRun *> vecRun;
};

#endif
