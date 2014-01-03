#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>

using namespace std;

#define MAX_LISTEN_CONN   5
#define SERVER_PORT       65533
#define MAX_LINE_SIZE     128
#define SERVER_IP         "127.0.0.1"
#define MAX_EPOLL_FD      2
#define MAX_EPOLL_EVENTS  100

class TcpServer
{
public:
    TcpServer();
	~TcpServer();
	void start();
private:
    int tcp_listen();
	void select_server();
	void epoll_server();
};