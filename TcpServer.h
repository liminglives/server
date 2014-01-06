#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>

#include "ChannelCallBack.h"
#include "Channel.h"

using namespace std;

#define MAX_LISTEN_CONN   5
#define SERVER_PORT       65533
#define MAX_LINE_SIZE     128
#define SERVER_IP         "127.0.0.1"
#define MAX_EPOLL_FD      2
#define MAX_EPOLL_EVENTS  100

class TcpServer : public ChannelCallBack
{
public:
    TcpServer();
    ~TcpServer();
    void start();
    virtual void handle(int sockfd);
private:
    int tcp_listen();
    void select_server();
    void epoll_server();
    int epollfd;
    int listenfd;
};
#endif