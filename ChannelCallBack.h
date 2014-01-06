#ifndef CHANNELCALLBACK_H
#define CHANNELCALLBACK_H

class ChannelCallBack
{
public:
    //ChannelCallBack() {}
    virtual void handle(int sockfd) {}  
    //virtual ~ChannelCallBack() {}
};
#endif
