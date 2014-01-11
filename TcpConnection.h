#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IFChannelCallBack.h"
#include "Buffer.h"
#include "IFRun.h"
#include <string>

using namespace std;

#define MAX_LINE_SIZE     128

class TcpConnection : public IFChannelCallBack , public IFRun
{
public:
    TcpConnection(EventLoop *_pLoop, int _connfd);
    ~TcpConnection();
    void sendData(const std::string &data);
    void setUser(IFUser *_pUser);
    void enableConnection();
    virtual void handleRead();
    virtual void handleWrite();
    virtual void run();

private:
    Channel *pChannel;
    int connfd;
    EventLoop *pLoop;
    IFUser *pUser;
    Buffer inBuf;
    Buffer outBuf;
};
#endif