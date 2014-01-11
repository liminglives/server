#include "Epoll.h"

#include "Channel.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <errno.h>


const int NewChannel = -1;
const int OldChannel = 1;

Epoll::Epoll()
{
    epollfd = epoll_create(MAX_EPOLL_FD);
    if (epollfd <= 0)
    {
        std::cout << "epoll_create failed" << std::endl;
    }
}

Epoll::~Epoll()
{
    close(epollfd);
}

void Epoll::update(Channel *_pChannel)
{
    struct epoll_event ev;
    //events |= EPOLLIN;
    int mode = 0;
    int index = _pChannel->getIndex();
    if (index== NewChannel)
    {
        mode |= EPOLL_CTL_ADD;
        _pChannel->setIndex(OldChannel);
    }
    else if (index == OldChannel)
        mode |= EPOLL_CTL_MOD;
         
    ev.data.ptr = _pChannel;
    ev.events = _pChannel->getEvents();
    epoll_ctl(epollfd, mode, _pChannel->getSockfd(), &ev); 
}

void Epoll::poll(std::vector < Channel * > &vecChannel)
{
    int nready = epoll_wait(epollfd, revents, MAX_EPOLL_EVENTS, -1);
    if (nready == -1)
    {
        std::cout << "epoll wait error:" << strerror(errno) <<std::endl;
    }
    
    for (int i=0; i<nready; ++i)
    {
        Channel *pTmpChannel = static_cast<Channel *>(revents[i].data.ptr);
        pTmpChannel->setREvent(revents[i].events);
        vecChannel.push_back(pTmpChannel);
    }
}