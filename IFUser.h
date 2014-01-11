#ifndef IFUSER_H
#define IFUSER_H

#include "Declear.h"

class IFUser
{
public:
    virtual void onConnection(TcpConnection *pTcpConn) = 0;
    virtual void onMessage(TcpConnection *pTcpConn, Buffer &data) = 0;
    virtual void onCompleteWrite() = 0;
};
#endif
