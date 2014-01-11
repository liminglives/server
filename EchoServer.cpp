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

void EchoServer::onMessage(TcpConnection * pTcpConn, std::string & mes)
{
    if (pTcpConn)
    {
        string data(mes);
        mes.clear();
        pTcpConn->sendData(data);
    }
}

void EchoServer::start()
{
    pServer.start();
}