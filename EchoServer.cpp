#include "EchoServer.h"
#include "TcpConnection.h"


#include <iostream>

EchoServer::EchoServer(EventLoop *_pLoop)
:pLoop(_pLoop),
pServer(_pLoop)
{
    pServer.setCallBack(this);
}

EchoServer::~EchoServer()
{}

void EchoServer::onConnection(TcpConnection * pTcpConn)
{
    std::cout << "new connection" << std::endl;
}

void EchoServer::onMessage(TcpConnection * pTcpConn, const std::string & mes)
{
    if (pTcpConn)
        pTcpConn->sendData(mes);
}

void EchoServer::start()
{
    pServer.start();
}