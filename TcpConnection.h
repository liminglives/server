#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "IFChannelCallBack.h"
#include "Channel.h"

#define MAX_LINE_SIZE     128

class TcpConnection : public IFChannelCallBack
{
public:
    TcpConnection(int _epollfd, int _connfd);
    virtual void handle(int sockfd);
    

private:
    Channel *pChannel;
    int connfd;
    int epollfd;
};
#endif