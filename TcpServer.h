#ifndef TCPSERVER_H#define TCPSERVER_H

#include <map>#include "Declear.h"#include "IFAcceptorCallBack.h"

class TcpServer : public IFAcceptorCallBack
{
public:      TcpServer();
     ~TcpServer();      TcpServer(EventLoop *loop);
    void start();
    //virtual void handle(int sockfd);       virtual void newTcpConnection(int sockfd);       void setCallBack(IFUser *_pUser);    
private:
    //int tcp_listen();
    void epoll_server();       std::map<int, TcpConnection *> mapSocketTcpConnection;       EventLoop *pLoop;       Acceptor *pAcceptor;       IFUser *pUser;};#endif