#ifndef IFCHANNELCALLBACK_H
#define IFCHANNELCALLBACK_H

class IFChannelCallBack
{
public:
    //ChannelCallBack() {}
    virtual void handle(int sockfd) {};  
    virtual ~IFChannelCallBack() {}
};
#endif
