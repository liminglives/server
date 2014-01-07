#include "Channel.h"

#include "IFChannelCallBack.h"
#include "EventLoop.h"

#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int socketfd)
:sockfd(socketfd), 
events(0), 
revents(0), 
callBack(0),
pLoop(loop)
{}

void Channel::setCallBack(IFChannelCallBack *callBack)
{
    this->callBack = callBack;
}

void Channel::registerEvent()
{
    events |= EPOLLIN;
    update();
}

void Channel::update()
{
   pLoop->update(this);
}

void Channel::setREvent(int event)
{
    revents = event;
}

void Channel::handleEvent()
{
    if (revents & EPOLLIN)
        callBack->handle(sockfd);
}

int Channel::getEvents()
{
    return events;
}

int Channel::getSockfd()
{
    return sockfd;
}