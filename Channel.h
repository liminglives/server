#ifndef CHANNEL_H
#define CHANNEL_H

#include "IFChannelCallBack.h"

class Channel
{
public:
    Channel(int epollfd, int sockfd);
    void setCallBack(IFChannelCallBack *callBack);
    void registerEvent();
    void setREvent(int event);
    void handleEvent();
    
private:
    int epollfd;
    int sockfd;
    int events;
    int revents;
    IFChannelCallBack *callBack;
};
#endif