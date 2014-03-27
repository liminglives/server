#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Declear.h"
#include "IFChannelCallBack.h"
#include "Timestamp.h"
#include "IFRun.h"

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

    long runAt(Timestamp when, IFRun* pRun);
    long runAfter(double delay, IFRun* pRun);
    long runEvery(double interval, IFRun* pRun);
    void cancelTimer(long timerId);
    
private:
    Epoll *pPoller;
    int ieventfd;
    Channel *pWakeupChannel;
    std::vector<Runner> vecRun;
    TimerQueue *pTimerQueue;

    int createEventfd();
};

#endif
