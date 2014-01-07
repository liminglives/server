#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

#include "TcpServer.h"

#include "EventLoop.h"#include "TcpConnection.h"#include "EventLoop.h"
#include "Acceptor.h"

TcpServer::TcpServer()
{}

TcpServer:: TcpServer(EventLoop *loop)
:pLoop(loop),
pAcceptor(0)
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
    
    mapSocketTcpConnection[sockfd] = pTcpConnection;
}

void TcpServer::start()
{
    epoll_server();
    //select_server();
}