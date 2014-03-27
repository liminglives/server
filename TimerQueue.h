#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "Declear.h"
#include "Timestamp.h"
#include "IFChannelCallBack.h"
#include "IFRun.h"

#include <vector>
#include <set>

class TimerQueue : public IFChannelCallBack
{
public:
    class Timer
    {
    public:
        Timer(Timestamp _time, IFRun *_pRun, double _interval):
            time(_time),
            id(_time),
            pRun(_pRun),
            interval(interval)
            {}
         Timestamp getTimestamp(){return time;}
         void run() { pRun->run(this); }
         bool isRepeat() {return interval > 0.0;}
         Timestamp getId() {return id;}
         void moveToNext() {time = Timestamp::nowAfter(interval);}
         
    private:
        Timestamp time;
        Timestamp id;
        IFRun *pRun;
        double interval;
    };

    class AddTimerWrapper : public IFRun
    {
    public:
        AddTimerWrapper(TimerQueue *_pTimerQueue):pTimerQueue(_pTimerQueue)
        {}
        virtual void run(void *param)
        {
            pTimerQueue->doAddTimer(param);
        }
    private:
        TimerQueue *pTimerQueue;
    };

    class CancelTimerWrapper : public IFRun
    {
    public:
        CancelTimerWrapper(TimerQueue *_pTimerQueue):pTimerQueue(_pTimerQueue)
        {}
        virtual void run(void *param)
        {
            pTimerQueue->doCancelTimer(param);
        }
    private:
        TimerQueue *pTimerQueue;
    };

    TimerQueue(EventLoop *_pEventLoop);

    
    long addTimer(IFRun *pRun, Timestamp time, double interval);
    void cancelTimer(long timerId);

    virtual void handleRead();
    virtual void handleWrite();

    

private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
        
    EventLoop *pEventLoop;
    int timerfd;
    Channel *pChannle;
    AddTimerWrapper *pAddTimerWrapper;
    CancelTimerWrapper *pCancelTimerWrapper;
    TimerList timerlist;

    int createTimerfd();
    bool insert(Timer *pTimer);
    void resetTimerfd(int timerfd, Timestamp time);
    struct timespec howMuchTimeFromNow(Timestamp when);
    void doAddTimer(void *param);
    void doCancelTimer(void *param);
    void readTimerfd(int timerfd, Timestamp now);
    std::vector<Entry> getExpired(Timestamp now); 
    void reset(const vector<Entry> &expired, Timestamp now);
    
};
#endif