#include "TcpServer.h"
#include "EventLoop.h"

int main(int args, char **argv)
{
    EventLoop loop;
    TcpServer server(&loop);
    server.start();
    loop.loop();
    return 0;
}