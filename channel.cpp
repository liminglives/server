#include "Channel.h"

#include "IFChannelCallBack.h"
#include "EventLoop.h"

#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int socketfd)
:sockfd(socketfd), 
events(0), 
revents(0), 
callBack(0),
pLoop(loop),
index(-1)
{}

void Channel::setCallBack(IFChannelCallBack *callBack)
{
    this->callBack = callBack;
}

void Channel::update()
{
   pLoop->update(this);
}

int Channel::getIndex()
{
    return index;
}

void Channel::setIndex(int _index)
{
    index = _index;
}

void Channel::setREvent(int event)
{
    revents = event;
}

void Channel::handleEvent()
{
    if (revents & EPOLLIN)
        callBack->handleRead();
    if (revents & EPOLLOUT)
        callBack->handleWrite();
}

void Channel::enableReading()
{
    events |= EPOLLIN;
    update();
}

void Channel::enableWriting()
{
    events |= EPOLLOUT;
    update();
}

void Channel::disableReading()
{
    events &= ~EPOLLIN;
    update();
}

void Channel::disableWriting()
{
    events &= ~EPOLLOUT;
    update();
}

int Channel::isWriting()
{
    return events & EPOLLOUT;
}

int Channel::getEvents()
{
    return events;
}

int Channel::getSockfd()
{
    return sockfd;
}