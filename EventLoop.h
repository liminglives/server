#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"
#include "IFChannelCallBack.h"
#include "Timestamp.h"

#include <vector>

class EventLoop : public IFChannelCallBack
{
public:
    class Runner
    {
    public:
        Runner(IFRun *_pRun, void *_param):
            pRun(_pRun),param(_param){}
        void doRun()
        {
            pRun->run(param);
        }
    private:
        IFRun *pRun;
        void *param;
    };

    
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel *_pChannel);
    virtual void handleRead();
    virtual void handleWrite();
    void queueLoop(IFRun *pRun, void *param);
    void wakeup();
    void handlePendingRuns();

    int runAt(Timestamp when, IRun* pRun);
    int runAfter(double delay, IRun* pRun);
    int runEvery(double interval, IRun* pRun);
    void cancelTimer(int timerfd);
    
private:
    Epoll *pPoller;
    int ieventfd;
    Channel *pWakeupChannel;
    std::vector<Runner> vecRun;
    TimerQueue *pTimerQueue;

    int createEventfd();
};

#endif
