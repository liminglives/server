#include "EchoServer.h"
#include "TcpConnection.h"
#include "Buffer.h"

#include <iostream>

EchoServer::EchoServer(EventLoop *_pLoop)
:pLoop(_pLoop),
pServer(_pLoop),
timer(0),
times(0)
{
    pServer.setCallBack(this);
}

EchoServer::~EchoServer()
{}

void EchoServer::onConnection(TcpConnection * pTcpConn)
{
    std::cout << "new connection" << std::endl;
}

void EchoServer::onMessage(TcpConnection * pTcpConn, Buffer &data)
{
    if (pTcpConn)
    {
        std::string sdata(data.retriveAsAllString());
        pTcpConn->sendData(sdata);
    
    }
    timer = pLoop->runEvery(1, this);
}

void EchoServer::onCompleteWrite()
{
    std::cout << "write completed" << std::endl;
}

void EchoServer::start()
{
    pServer.start();
}

void EchoServer::run(void * param)
{
    std::cout << times <<std::endl;
    if (times++ == 3)
    {
        pLoop->cancelTimer(timer);
        times = 0;
    }
}
