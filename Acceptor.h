#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Declear.h"
#include "IFChannelCallBack.h"


#define SERVER_PORT       65533
#define MAX_LISTEN_CONN   5

class Acceptor : public IFChannelCallBack
{
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();
    virtual void handle(int sockfd);
    void start();
    void setCallBack(IFAcceptorCallBack *_pCallBack);

private:
    EventLoop *pLoop;
    int listenfd;
    Channel *pChannel;
    IFAcceptorCallBack *pCallBack;
    
    int tcp_listen();
};
#endif
