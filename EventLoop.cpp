#include "EventLoop.h"

#include "Channel.h"
#include "Epoll.h"
#include "IFRun.h"
#include "TimerQueue.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>


using namespace std;

EventLoop::EventLoop()
:pPoller(new Epoll),
pWakeupChannel(new Channel(this, ieventfd)),
pTimerQueue(new TimerQueue())
{
    ieventfd = createEventfd();
    //pWakeupChannel = new Channel(this, ieventfd);
    pWakeupChannel->setCallBack(this);
    pWakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    delete pPoller;
    delete pWakeupChannel;
    
}

void EventLoop::update(Channel *_pChannel)
{
    pPoller->update(_pChannel);
}

void EventLoop::loop()
{
    while(1)
    {
        std::vector<Channel*> vecChannel;
        pPoller->poll(vecChannel);

        for (std::vector<Channel *>::iterator itChannel = vecChannel.begin(); itChannel != vecChannel.end(); ++itChannel)
        {
            (*itChannel)->handleEvent();
        }

        handlePendingRuns();
    }
    
}

int EventLoop::createEventfd()
{
    int ret = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (ret < 0)
        std::cout << "create eventfd failed" << std::endl;
    return ret;
}

void EventLoop::queueLoop(IFRun * pRun, void *param)
{
    Runner runner(pRun, param);
    vecRun.push_back(runner);
    wakeup();
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(ieventfd, &one, sizeof(one));
    if (n != sizeof(one))
    {
        std::cout << "write data into eventfd error" << std::endl;
    }
}

void EventLoop::handleRead()
{
    uint64_t rdata = 0;
    ssize_t n = read(ieventfd, &rdata, sizeof(rdata));
    if (n != sizeof(rdata))
    {
        std::cout << "read data from eventfd error" << std::endl;
    }
}

void EventLoop::handleWrite()
{
}

void EventLoop::handlePendingRuns()
{
    vector<Runner> tempRuns;
    tempRuns.swap(vecRun);
    vector<Runner>::iterator it;
    for(it = tempRuns.begin(); it != tempRuns.end(); ++it)
    {
        (*it).doRun();
    }
}

int EventLoop::runAt(Timestamp when, IFRun* pRun)
{
    return pTimerQueue->addTimer( pRun, when, 0.0);
}
int EventLoop::runAfter(double delay, IRun* pRun)
{
    return pTimerQueue->addTimer(pRun, Timestamp::nowAfter(delay), 0.0);
}
int EventLoop::runEvery(double interval, IRun* pRun)
{
    return pTimerQueue->addTimer(pRun, Timestamp::nowAfter(interval), interval);
}
void EventLoop::cancelTimer(int timerId)
{
    pTimerQueue->cancelTimer(timerId);
}