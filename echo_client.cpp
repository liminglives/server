#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

//using namespace std;

#define SERVER_PORT 65533
#define MAX_LINE_SIZE         128
#define SERVER_IP  "127.0.0.1"

int tcp_connect(int sockfd)
{
    int ret;
	struct sockaddr_in serveraddr; 
	
	bzero(&serveraddr,sizeof(serveraddr));  
    serveraddr.sin_family=AF_INET;  
    serveraddr.sin_port=htons(SERVER_PORT);  
    inet_pton(AF_INET,SERVER_IP,&serveraddr.sin_addr);  
	
	ret = connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if (ret < 0)
	{
	    std::cout << "connect error: " << strerror(errno) << std::endl;
		exit(0);
	}
	return ret;
}


void echo_client()
{
    int sockfd, serverfd;
	char buf[MAX_LINE_SIZE] = {0};
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
	    std::cout << "create socket error" << std::endl;
		exit(0);
	}
	
	tcp_connect(sockfd);
	while(1)  
    {  
        int len = 0,nwrite,nread;
        bzero(buf,sizeof(buf));  
        std::cin >> buf;  
	if (strcmp(buf, "q")==0)
		    break;
        len = strlen(buf);
        if ((nwrite=write(sockfd,buf,len))!=len)
        {
            std::cout<<"write error:"<<strerror(errno)<<" nwrite="<<nwrite<<std::endl;
             break;
        }  
        bzero(buf,sizeof(buf));  
        //len = strlen(buf);
        std::cout<<"have written "<< len << "charaters"<<std::endl;
        if ((nread=read(sockfd,buf,sizeof(buf)))<=0)
        {std::cout<<"read error:"<<strerror(errno)<<" nread="<<nread<<std::endl;break;}
        std::cout << "from server: " << buf << std::endl; 
    }  
	close(sockfd);
}

int main()
{
    echo_client();
    return 0;
}
