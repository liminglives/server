#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "Declear.h"
#include "IFUser.h"
#include "TcpServer.h"
#include "IFRun.h"

class EchoServer : public IFUser , public IFRun
{
public:
    EchoServer(EventLoop *pLoop);
    ~EchoServer();
    virtual void onConnection(TcpConnection * pTcpConn);
    virtual void onMessage(TcpConnection * pTcpConn, Buffer &data);
    virtual void onCompleteWrite();
    void start();
    virtual void run(void * param);

private:
    EventLoop *pLoop;
    TcpServer pServer;
    long timer;
    int times;
    
};
#endif