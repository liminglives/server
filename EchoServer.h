#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "Declear.h"
#include "IFUser.h"
#include "TcpServer.h"

class EchoServer : public IFUser
{
public:
    EchoServer(EventLoop *pLoop);
    ~EchoServer();
    virtual void onConnection(TcpConnection * pTcpConn);
    virtual void onMessage(TcpConnection * pTcpConn, Buffer &data);
    virtual void onCompleteWrite();
    void start();

private:
    EventLoop *pLoop;
    TcpServer pServer;
    
};
#endif