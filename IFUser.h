#ifndef IFUSER_H
#define IFUSER_H

#include "Declear.h"
#include <string>

class IFUser
{
public:
    virtual void onConnection(TcpConnection *pTcpConn) = 0;
    virtual void onMessage(TcpConnection *pTcpConn, const std::string &mes) = 0;
};
#endif
