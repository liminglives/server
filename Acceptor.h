#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "IFChannelCallBack.h"
#include "Channel.h"
#include "IFAcceptorCallBack.h"

#define SERVER_PORT       65533

class Acceptor : public IFChannelCallBack
{
public:
    Acceptor(int _epollfd);
    virtual void handle(int sockfd);
    void start();
    void setCallBack(IFAcceptorCallBack *_pCallBack);

private:
    Channel *pChannel;
    IFAcceptorCallBack *pCallBack;
    int listenfd;
    int epollfd;
    int tcp_listen();
};
#endif
