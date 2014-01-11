#ifndef CHANNEL_H
#define CHANNEL_H

#include "Declear.h"

class Channel
{
public:
    Channel(EventLoop *loop, int sockfd);
    void setCallBack(IFChannelCallBack *callBack);
    void setREvent(int event);
    void handleEvent();
    int getEvents();
    int getSockfd();
    void enableWriting();
    void enableReading();
    void disableWriting();
    void disableReading();
    int isWriting();
    int getIndex();
    void setIndex(int);
    
private:;
    int sockfd;
    int events;
    int revents;
    IFChannelCallBack *callBack;
    EventLoop *pLoop;
    int index;
    void update();
};
#endif