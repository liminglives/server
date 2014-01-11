#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "Declear.h"
#include "IFUser.h"
#include "TcpServer.h"
#include <string>

class EchoServer : public IFUser
{
public:
    EchoServer(EventLoop *pLoop);
    ~EchoServer();
    virtual void onConnection(TcpConnection * pTcpConn);
    virtual void onMessage(TcpConnection * pTcpConn, std::string & mes);
    void start();

private:
    EventLoop *pLoop;
    TcpServer pServer;
    
};
#endif