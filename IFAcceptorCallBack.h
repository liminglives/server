#ifndef IFACCEPTORCALLBACK_H
#define IFACCEPTORCALLBACK_H

class IFAcceptorCallBack
{
public:
    virtual void newTcpConnection(int sockfd) {};
    virtual ~IFAcceptorCallBack(){}
};

#endif
