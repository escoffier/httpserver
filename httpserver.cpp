#include "httpserver.h"
#include <iostream>
#include <sys/socket.h>


HttpServer::HttpServer(short port)
{
    prot_ = port;
}

bool HttpServer::Start()
{
    int listenfd;
	listenfd = socket(AF_INT, SOCK_STREAM, IPPROTO_TCP);
	if( listenfd == -1)
	{
	    std::cout<<"fail to get socket"<<std::endl;
		return false;
	}
    struct sockaddr_in serveraddr;
	struct sockaddr_in peeraddr;
	memset(&serveraddr, 0 , sizeof(serveraddr));
	memset(&peeraddr, 0 , sizeof(peeraddr));
	
	serveraddr.sin_family = AF_INT;
	serveraddr.sin_addr.s_addr = htol(INADDR_ANY);
	serveraddr.sin_port = htons(port_);
	if(-1 = bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr)))
	{
	    std::cout<<"bind failed"<<std::endl;
		return false;
	}
	
	if(-1 == listen(listenfd, 50))
	{
	    std::cout<<"listen failed"<<std::endl;
		return false;
	}
	struct pollfd fd;
	fd.fd = listenfd;
	fd.events = POLLIN;
	channels.push_back(fd);
	while ( true )
	{
	    int eventnum;
	    eventnum = poll(&channels[0], channels.size(), NULL);
		if(eventnum > 0)
		{
		    if( channels[0].revents & POLLIN)
			{
			    int connfd;
			    connfd = accept(channels[0].fd, (sockaddr *)&peeraddr, sizeof(sockaddr));
				if( connfd > 0 )
				{
				    fd.fd = connfd;
					fd.events = POLLIN;
					channels.push_back(fd);
				}
			}
		
		    for(int i = 1; i < eventnum; ++i)
			{
			    if( channels[i].revents & (POLLIN)
				{
				    if(read(channels[i].fd, buf 6000) > 0)
					{
					    std::cout<<buf<<std::endl;
					}
					
				}
			}
		}
		else
		{
		}
	}
	return true;
}