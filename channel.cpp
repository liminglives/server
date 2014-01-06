#include "Channel.h"
#include <sys/epoll.h>

Channel::Channel(int epollfd, int socketfd) 
:epollfd(epollfd), 
sockfd(socketfd), 
events(0), 
revents(0), 
callBack(0)
{}

void Channel::setCallBack(ChannelCallBack *callBack)
{
    this->callBack = callBack;
}

void Channel::registerEvent()
{
    struct epoll_event ev;
    events |= EPOLLIN;
    ev.data.ptr = this;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);    
    
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
