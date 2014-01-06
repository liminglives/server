#ifndef TCPSERVER_H#define TCPSERVER_H

#include <sys/epoll.h>#include <map>

#include "IFAcceptorCallBack.h"
#include "Channel.h"#include "TcpConnection.h"

using namespace std;

#define MAX_LISTEN_CONN   5
#define SERVER_PORT       65533
#define MAX_LINE_SIZE     128
#define SERVER_IP         "127.0.0.1"
#define MAX_EPOLL_FD      2
#define MAX_EPOLL_EVENTS  100

class TcpServer : public IFAcceptorCallBack
{
public:
    TcpServer();
    ~TcpServer();
    void start();
    //virtual void handle(int sockfd);       virtual void newTcpConnection(int sockfd);
private:
    //int tcp_listen();
    void epoll_server();
    int epollfd;
    int listenfd;       map<int, TcpConnection *> mapSocketTcpConnection;};#endif