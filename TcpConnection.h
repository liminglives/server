#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IFChannelCallBack.h"

#include <string>

using namespace std;

#define MAX_LINE_SIZE     128

class TcpConnection : public IFChannelCallBack
{
public:
    TcpConnection(EventLoop *_pLoop, int _connfd);
    ~TcpConnection();
    virtual void handle(int sockfd);
    void sendData(const std::string &data);
    void setUser(IFUser *_pUser);
    void enableConnection();
    virtual void handleRead();
    virtual void handleWrite();

private:
    Channel *pChannel;
    int connfd;
    EventLoop *pLoop;
    IFUser *pUser;
    string *inBuf;
    string *outBuf;
};
#endif