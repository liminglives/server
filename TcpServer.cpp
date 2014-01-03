#include "TcpServer.h"

TcpServer::TcpServer()
{}

TcpServer::~TcpServer()
{}

int TcpServer::tcp_listen()
{
    int socket_ret = 0; 
	int on = 1;
	struct sockaddr_in server_addr;
	
    bzero(&server_addr, sizeof(server_addr));	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(SERVER_PORT);
	
	socket_ret = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ret < 0)
	{
	    cout << "create socket error!" << endl;
		exit(1);
	}
	fcntl(socket_ret, F_SETFL, O_NONBLOCK);
	setsockopt(socket_ret, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (-1 == bind(socket_ret, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
	    cout << "bind error!" << endl;
		exit(1);
	}
	
	if (-1 == listen(socket_ret, 5))
	{
	    cout << "listen error!" << endl;
		exit(1);
	}
	
	return socket_ret;
}

void TcpServer::select_server()
{
    int socket_listen = 0, socket_conn = 0;
	int nready = 0;
	int max_fd = 0;
	int i, maxi = -1;
        socklen_t cli_len = 0;
	struct sockaddr_in client_addr;
    fd_set fdr,fdtmp;
	int client[FD_SETSIZE] = {0};
	char line[MAX_LINE_SIZE] = {0};
	
	socket_listen = tcp_listen();
	
	max_fd = socket_listen;
	for (i=0; i<FD_SETSIZE; ++i)
	    client[i] = -1;
	FD_ZERO(&fdr);
	FD_SET(socket_listen, &fdr);
	
	while(1)
    {
        fdtmp = fdr;
		nready = select(max_fd + 1, &fdtmp, NULL, NULL, NULL);
		if (select <= 0)
		{
		    cout << "select end" << endl;
			break;
		}
		if(FD_ISSET(socket_listen, &fdtmp))
		{
		    cli_len = sizeof(client_addr);
		    socket_conn = accept(socket_listen, (struct sockaddr *)&client_addr, &cli_len);
		     std::cout<<"a client connected"<<std::endl;	
                      for (i=0; i<FD_SETSIZE; ++i)
			    if (client[i] < 0) 
				{
				    client[i] = socket_conn;
					break;
				}
			if (i == FD_SETSIZE)
                        {
			    cout << "too many client" << endl;
				exit(1);
			}			
			FD_SET(socket_conn, &fdr);
			if (socket_conn > socket_listen)
			    max_fd = socket_conn;
			if (i > maxi)
			    maxi = i;
		}
		for (i=0; i<maxi; ++i)
		{
		    int nread = 0,fd = client[i];
		    if (fd<0 && !FD_ISSET(client[i], &fdtmp))
			    continue;
                      bzero(line, sizeof(line));
			nread = read(fd, line, MAX_LINE_SIZE);
			if (nread <= 0)
			{
                            std::cout<<"read error:"<<strerror(errno)<<std::endl;
			    close(fd);
				FD_CLR(fd, &fdr);
				client[i] = -1;
			}
			else
			{
                            std::cout<<"send from client:"<<line<<std::endl;
			    if (write(fd, line, nread) != nread)
				    cout << "write error" <<strerror(errno)<< endl;
				if (--nready == 0)
				    break;
			}
		}
	}	
}

void TcpServer::epoll_server()
{
    int listenfd;
	int epollfd;
	struct epoll_event ev_reg,events[MAX_EPOLL_EVENTS];
	char line[MAX_LINE_SIZE] = {0};
	int nread;
	struct sockaddr_in cli_addr;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	
	epollfd = epoll_create(MAX_EPOLL_FD);
	
	listenfd = tcp_listen();
	ev_reg.data.fd = listenfd;
	ev_reg.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev_reg);
	
	while(1)
	{
	    int nready = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
		if (nready == -1)
		{
		    std::cout << "epoll wait error:" << strerror(errno) <<std::endl;
			break;
		}
		for (int i=0; i<nready; ++i)
		{
		    if (events[i].data.fd == listenfd)
			{
			    int connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_len);
				if (connfd < 0)
				{
				    std::cout<<"accept error:"<<strerror(errno)<<std::endl;
					continue;
				}
				fcntl(connfd, F_SETFL, O_NONBLOCK);
				ev_reg.data.fd = connfd;
				ev_reg.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev_reg);
                              cout << "new connection from " 
                              << "[" << inet_ntoa(cli_addr.sin_addr) 
                               << ":" << ntohs(cli_addr.sin_port) << "]" 
                                << " accept socket fd:" << connfd 
                                << endl;
			}
			else if (events[i].events & EPOLLIN)
			{
			    int sockfd = events[i].data.fd;
			    if (sockfd < 0)
				{
				    std::cout<<"EPOLLIN sockfd < 0"<<std::endl;
					continue;
				}
				bzero(line, sizeof(line));
				nread = read(sockfd, line, sizeof(line));
				if (nread <= 0)
				{
				    std::cout<<"read error:" <<strerror(errno)<<std::endl;
					close(sockfd);
					continue;
				}
				if (write(sockfd, line, nread) != nread)
				    std::cout<<"write error:"<<strerror(errno)<<endl;
				
			}
		}
	}
	close(listenfd);
	close(epollfd);
}

void TcpServer::start()
{
    epoll_server();
	//select_server();
}