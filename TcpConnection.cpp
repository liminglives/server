#include "TcpConnection.h"
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>

TcpConnection::TcpConnection(int _epollfd, int _sockfd)
:connfd(_sockfd),
epollfd(_epollfd)
{
    pChannel = new Channel(epollfd, connfd);
    pChannel->setCallBack(this);
    pChannel->registerEvent();
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

