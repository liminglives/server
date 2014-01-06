#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

#include "TcpServer.h"
#include "Acceptor.h"

TcpServer::TcpServer():epollfd(0), listenfd(0)
{}

TcpServer::~TcpServer()
{}

void TcpServer::epoll_server()
{
    struct epoll_event  revents[MAX_EPOLL_EVENTS];
    
    epollfd = epoll_create(MAX_EPOLL_FD);
    if (epollfd < 0)
    {
        cout << "epoll_create failed" << endl;
        return;
    }

    Acceptor *pAcceptor = new Acceptor(epollfd);
    pAcceptor->setCallBack(this);
    pAcceptor->start();
    
    while(1)
    {
        int nready = epoll_wait(epollfd, revents, MAX_EPOLL_EVENTS, -1);
        if (nready == -1)
        {
            std::cout << "epoll wait error:" << strerror(errno) <<std::endl;
            break;
        }
        vector<Channel *> vecChannel;
        for (int i=0; i<nready; ++i)
        {
            Channel *pTmpChannel = static_cast<Channel *>(revents[i].data.ptr);
            pTmpChannel->setREvent(revents[i].events);
            vecChannel.push_back(pTmpChannel);
        }

        for (vector<Channel *>::iterator itChannel = vecChannel.begin(); itChannel != vecChannel.end(); ++itChannel)
        {
            (*itChannel)->handleEvent();
        }
    }
    close(listenfd);
    close(epollfd);
}

void TcpServer::newTcpConnection(int sockfd)
{
    TcpConnection *pTcpConnection = new TcpConnection(epollfd, sockfd); //memory leak
    
    mapSocketTcpConnection[sockfd] = pTcpConnection;
}

void TcpServer::start()
{
    epoll_server();
    //select_server();
}