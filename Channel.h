#ifndef CHANNEL_H
#define CHANNEL_H

#include "Declear.h"

class Channel
{
public:
    Channel(EventLoop *loop, int sockfd);
    void setCallBack(IFChannelCallBack *callBack);
    void registerEvent();
    void setREvent(int event);
    void handleEvent();
    int getEvents();
    int getSockfd();
    
private:;
    int sockfd;
    int events;
    int revents;
    IFChannelCallBack *callBack;
    EventLoop *pLoop;
    void update();
};
#endif