#include "TcpConnection.h"

#include "EventLoop.h"
#include "Channel.h"

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>

TcpConnection::TcpConnection(EventLoop *_pLoop, int _sockfd)
:connfd(_sockfd),
pLoop(_pLoop)
{
    pChannel = new Channel(pLoop, connfd);
    pChannel->setCallBack(this);
    pChannel->registerEvent();
}

TcpConnection::~TcpConnection()
{
    delete pChannel;
}

void TcpConnection::handle(int sockfd)
{
    if (sockfd < 0)
    {
        std::cout<<"EPOLLIN sockfd < 0"<<std::endl;
        return;
    }
    char line[MAX_LINE_SIZE] = {0};
    int nread = read(sockfd, line, sizeof(line));
    if (nread <= 0)
    {
        std::cout<<"read error:" <<strerror(errno)<<std::endl;
        close(sockfd);
        return;
    }
    if (write(sockfd, line, nread) != nread)
        std::cout<<"write error:"<<strerror(errno)<<std::endl;
}

