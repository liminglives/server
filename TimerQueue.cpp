#include "TimerQueue.h"
#include <sys/timerfd.h>
#include <iostream>
#include <string.h>

#define UINTPTR_MAX 0xffffffff

TimerQueue::TimerQueue(EventLoop * _pEventLoop)
:pEventLoop(_pEventLoop)
,timerfd(createTimerfd())
,pChannle(new Channel(pEventLoop, timerfd))
,pAddTimerWrapper(new AddTimerWrapper(this))
,pCancelTimerWrapper(new CancelTimerWrapper(this))
{
    pChannle->setCallBack(this);
    pChannle->enableReading();
}

int TimerQueue::createTimerfd()
{
    int timerfd = timerfd_create(CLOCK_MONOTONIC,
            TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        std::cout<< "create timerfd failed" <<std::endl;
    }
    return timerfd;
}

int TimerQueue::addTimer(IFRun *pRun, Timestamp time, double interval)
{
    Timer *pTimer = new Timer(time, pRun, interval);
    pEventLoop->queueLoop(pAddTimerWrapper, pTimer);
    return (int)pTimer;
}

void TimerQueue::cancelTimer(int timerId)
{
    pEventLoop->queueLoop(pCancelTimerWrapper, static_cast<void *>timerId);
}

void TimerQueue::doAddTimer(void *param)
{
    Timer *pTimer = static_cast<Timer*>(param);
    bool earliestChanged = insert(pTimer);
    if (earliestChanged)
    {
        resetTimerfd(timerfd, pTimer->getTimestamp());
    }
    
}

void TimerQueue::doCancelTimer(void * param)
{
    Timer *pTimer = static_cast<Timer*>(param);
    Entry e(pTimer->getTimestamp(), pTimer);
    
    for (TimerList::iterator it = timerlist.begin(); it != timerlist.end(); ++it)
    {
        if (it->second == pTimer)
        {
            timerlist.erase(it);
            break;
        }
    }
}

void TimerQueue::handleRead()
{
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd, now);

    std::vector<Entry> expired = getExpired(now);
    for (std::vector<Entry> it = expired.begin(); it != expired.end(); ++it)
    {
        it->second->run();
    }
    reset(expired, now);
}

void TimerQueue::handleWrite()
{}

void TimerQueue::reset(const vector < Entry > & expired, Timestamp now)
{
    for (std::vector<Entry>::const_iterator it=expired.begin(); it != expired.end(); ++it)
    {
        if (it->second->isRepeat())
        {
            it->second->moveToNext();
            insert(it->second);
        }
    }

    Timestamp nextExpire;
    if  (!timerlist.empty())
    {
        nextExpire = timerlist.begin()->second->getTimestamp();
    }
    if (nextExpire.valid())
    {
        resetTimerfd(timerfd, nextExpire);
    }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry entry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator end = timerlist.lower_bound(entry);
    copy(timerlist.begin(), end, back_inserter(expired));
    timerlist.erase(timerlist.begin(), end);
    return expired;
}

void TimerQueue::readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    sszie_t n = read(timerfd, &howmany, sizeof(howmany));
    if (n != sizeof(howmany))
    {
        std::cout << "TimerQueue:readTimerfd read error" << std::endl;
    }
}

bool TimerQueue::insert(Timer* pTimer)
{
    bool earliestChanged = false;
    Timestamp when = pTimer->getStamp();
    TimerList::iterator it = _timers.begin();
    if(it == _timers.end() || when < it->first)
    {
        earliestChanged = true;
    }
    pair<TimerList::iterator, bool> result
       = _timers.insert(Entry(when, pTimer));
    if(!(result.second))
    {
        cout << "_timers.insert() error " << endl;
    }

    return earliestChanged;
}

void TimerQueue::resetTimerfd(int timerfd, Timestamp time)
{
    struct itimerspec newvalue, oldvalue;
    bzero(&newvalue, sizeof(newvalue));
    bzero(&oldvalue, sizeof(oldvalue));
    newvalue.it_value = howMuchTimeFromNow(time);
    ret = timerfd_settime(timerfd, 0, &newvalue, &oldvalue);
    if  (ret)
    {
        std::cout<< "timerfd_settime error" << std::endl;
    }
    return ret;
}

struct timespec TimerQueue::howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch()
        - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
            microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
            (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

