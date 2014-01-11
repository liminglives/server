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
    pChannel->enableReading();
}

TcpConnection::~TcpConnection()
{
    
    delete pChannel;
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
        std::cout << "read 0 , closed fd" <<std::endl;
        close(connfd);
        return;
    }
    
    inBuf.append(line, nread);
    pUser->onMessage(this, inBuf);
}

void TcpConnection::handleWrite()
{
    if (!outBuf.empty())
    {
        int nwrite = write(connfd, outBuf.toCString(), outBuf.readableBytes());
        if (nwrite < 0)
            std::cout<<"write error:"<<strerror(errno)<<std::endl;
        else
        {
            outBuf.retrive(nwrite);
            if (outBuf.empty())
            {
                pChannel->disableWriting();
                pLoop->queueLoop(this);
            }
        }
    }
}

void TcpConnection::sendData(const std::string & data)
{
    int nwrite = 0;
    if (outBuf.empty())
    {
        nwrite = write(connfd, data.c_str(), data.size());
        if (nwrite < 0)
        {
            cout << "write error" << endl;
            return;
        }
    }
    
    if (nwrite < (static_cast<int>(data.size())))
    {
        outBuf.append(data.substr(nwrite, data.size()));
        if (!(pChannel->isWriting()))
            pChannel->enableWriting();
    }
    else //if (nwrite == (static_cast<int>(data.size())))
    {
        pLoop->queueLoop(this);
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

void TcpConnection::run()
{
    pUser->onCompleteWrite();
}