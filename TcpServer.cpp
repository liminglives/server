#include <iostream>
#include <string.h>
#include <unistd.h>


#include "TcpServer.h"

#include "EventLoop.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "IFUser.h"

TcpServer::TcpServer()
:pLoop(0),
pAcceptor(0),
pUser(0)
{}

TcpServer:: TcpServer(EventLoop *loop)
:pLoop(loop),
pAcceptor(0),
pUser(0)
{}

TcpServer::~TcpServer()
{
    delete pAcceptor;
}

void TcpServer::epoll_server()
{
    pAcceptor = new Acceptor(pLoop);
    pAcceptor->setCallBack(this);
    pAcceptor->start();
}

void TcpServer::newTcpConnection(int sockfd)
{
    TcpConnection *pTcpConnection = new TcpConnection(pLoop, sockfd); //memory leak
    pTcpConnection->setUser(pUser);
    pTcpConnection->enableConnection();
    mapSocketTcpConnection[sockfd] = pTcpConnection;
}

void TcpServer::start()
{
    epoll_server();
    //select_server();
}

void TcpServer::setCallBack(IFUser * _pUser)
{
    pUser = _pUser;
}