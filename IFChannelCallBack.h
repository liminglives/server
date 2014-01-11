#ifndef IFCHANNELCALLBACK_H
#define IFCHANNELCALLBACK_H

class IFChannelCallBack
{
public:
    //ChannelCallBack() {}
    virtual void handle(int sockfd) = 0;
    virtual void handleRead() = 0;
    virtual void handleWrite() = 0;
    virtual ~IFChannelCallBack() {}
};
#endif
