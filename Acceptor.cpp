#include "Acceptor.h"

#include "IFAcceptorCallBack.h"
#include "Channel.h"
#include "EventLoop.h"

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

Acceptor:: Acceptor(EventLoop *loop)
:pLoop(loop),
listenfd(0),
pChannel(0),
pCallBack(0)
{}

Acceptor::~Acceptor()
{
    close(listenfd);
    delete pChannel;
}

void Acceptor::start()
{
    listenfd = tcp_listen();
    
    //pChannel = new Channel(epollfd, listenfd);
    pChannel = new Channel(pLoop, listenfd);
    pChannel->setCallBack(this);
    pChannel->enableReading();
}

void Acceptor::setCallBack(IFAcceptorCallBack *_pCallBack)
{
    this->pCallBack = _pCallBack;
}

void Acceptor::handleRead()
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(struct sockaddr_in);
    int connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (connfd < 0)
    {
        std::cout<<"accept error:"<<strerror(errno)<<std::endl;
        return;
    }
    fcntl(connfd, F_SETFL, O_NONBLOCK);
    
    pCallBack->newTcpConnection(connfd);

    std::cout << "new connection from " 
    << "[" << inet_ntoa(cli_addr.sin_addr) 
    << ":" << ntohs(cli_addr.sin_port) << "]" 
    << " accept socket fd:" << connfd 
    << std::endl;
}

void Acceptor::handleWrite()
{}

int Acceptor::tcp_listen()
{
    int socket_ret = 0; 
    int on = 1;
    struct sockaddr_in server_addr;
    
    bzero(&server_addr, sizeof(server_addr));    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);
    
    socket_ret = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ret < 0)
    {
        std::cout << "create socket error!" << std::endl;
        exit(1);
    }
    fcntl(socket_ret, F_SETFL, O_NONBLOCK);
    setsockopt(socket_ret, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (-1 == bind(socket_ret, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cout << "bind error!" << std::endl;
        exit(1);
    }
    
    if (-1 == listen(socket_ret, MAX_LISTEN_CONN))
    {
        std::cout << "listen error!" << std::endl;
        exit(1);
    }
    
    return socket_ret;
}