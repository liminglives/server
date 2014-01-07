#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IFChannelCallBack.h"


#define MAX_LINE_SIZE     128

class TcpConnection : public IFChannelCallBack
{
public:
    TcpConnection(EventLoop *_pLoop, int _connfd);
    ~TcpConnection();
    virtual void handle(int sockfd);
    

private:
    Channel *pChannel;
    int connfd;
    EventLoop *pLoop;
    
};
#endif