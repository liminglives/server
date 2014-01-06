#ifndef CHANNEL_H
#define CHANNEL_H

#include "ChannelCallBack.h"

class Channel
{
public:
    Channel(int epollfd, int sockfd);
    void setCallBack(ChannelCallBack *callBack);
    void registerEvent();
    void setREvent(int event);
    void handleEvent();
    
private:
    int epollfd;
    int sockfd;
    int events;
    int revents;
    ChannelCallBack *callBack;
};
#endif