#include "EventLoop.h"

#include "Channel.h"
#include "Epoll.h"

#include <vector>

EventLoop::EventLoop()
:pPoller(new Epoll)
{}

EventLoop::~EventLoop()
{
    delete pPoller;
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
    }
    
}