//#include "TcpServer.h"
#include "EventLoop.h"
#include "EchoServer.h"

int main(int args, char **argv)
{
    EventLoop loop;
    //TcpServer server(&loop);
    EchoServer server(&loop);
    server.start();
    loop.loop();
    return 0;
}