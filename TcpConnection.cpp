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
pUser(0),
inBuf(new string()),
outBuf(new string())
{
    pChannel = new Channel(pLoop, connfd);
    pChannel->setCallBack(this);
    pChannel->enableReading();
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

void TcpConnection::handleRead()
{
    if (connfd < 0)
    {
        std::cout<<"EPOLLIN sockfd < 0"<<std::endl;
        return;
    }
    char line[MAX_LINE_SIZE] = {0};
    int nread = read(connfd, line, sizeof(line));
    if (nread < 0)
    {
        std::cout<<"read error:" <<strerror(errno)<<std::endl;
        close(connfd);
        return;
    }
    else if (nread == 0)
    {
        std::cout << "read 0 closed fd" <<std::endl;
        close(connfd);
        return;
    }
    
    inBuf->append(line, nread);
    pUser->onMessage(this, *inBuf);
}

void TcpConnection::handleWrite()
{
    if (!outBuf->empty())
    {
        int nwrite = write(connfd, outBuf->c_str(), outBuf->size());
        if (nwrite < 0)
            std::cout<<"write error:"<<strerror(errno)<<std::endl;
        else
        {
            *outBuf = (outBuf->substr(nwrite, outBuf->size()));
            if (outBuf->empty())
                pChannel->disableWriting();
        }
    }
}

void TcpConnection::sendData(const std::string & data)
{
    int nwrite = 0;
    if (outBuf->empty())
    {
        nwrite = write(connfd, data.c_str(), data.size());
        if (nwrite < 0)
            cout << "write error" << endl;
    }
    if (nwrite < (static_cast<int>(data.size())))
    {
        outBuf->append(data.begin()+nwrite, data.end());
        if (!(pChannel->isWriting()))
            pChannel->enableWriting();
    }

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

