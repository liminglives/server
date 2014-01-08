#include "TcpConnection.h"

#include "EventLoop.h"
#include "Channel.h"
#include "IFUser.h"

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>

TcpConnection::TcpConnection(EventLoop *_pLoop, int _sockfd)
:connfd(_sockfd),
pLoop(_pLoop),
pUser(0)
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
    if (nread < 0)
    {
        std::cout<<"read error:" <<strerror(errno)<<std::endl;
        close(sockfd);
        return;
    }
    else if (nread == 0)
    {
        std::cout << "read 0 closed fd" <<std::endl;
        close(sockfd);
        return;
    }
    //if (write(sockfd, line, nread) != nread)
        //std::cout<<"write error:"<<strerror(errno)<<std::endl;
    std::string mes(line, nread);
    pUser->onMessage(this, mes);
    
}

void TcpConnection::sendData(const std::string & data)
{
    unsigned int nwrite = write(connfd, data.c_str(), data.size());
    if (nwrite != data.size())
        std::cout<<"write error:"<<strerror(errno)<<std::endl;
}

void TcpConnection::setUser(IFUser * _pUser)
{
    pUser = _pUser;
}

void TcpConnection::enableConnection()
{
    if (pUser)
        pUser->onConnection(this);
}

