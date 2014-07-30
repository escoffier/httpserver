#include "httpserver.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <channel.c>

void ConnectCB(void* arg)
{
    HttpServer* server = (HttpServer*) arg;
	server->OnConnection();
}

void ReadCB(void *arg)
{
   char buf[1024] = {0};
   int fd = (int)arg;
   if(read(fd, buf, 6000) > 0)
	{
		std::cout<<"**************recive date*************"<<std::endl;
		std::cout<<buf<<std::endl;
		std::cout<<"**************************************"<<std::endl;
	}
}
HttpServer::HttpServer(short port)
{
    port_ = port;
}

bool HttpServer::Start()
{
    char buf[1024] = {0};
    listenfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( listenfd_ == -1)
    {
        std::cout<<"fail to get socket"<<std::endl;
        return false;
    }
    struct sockaddr_in serveraddr;
    struct sockaddr_in peeraddr;
    memset(&serveraddr, 0 , sizeof(serveraddr));
    memset(&peeraddr, 0 , sizeof(peeraddr));
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port_);
    if(-1 == bind(listenfd_, (sockaddr *)&serveraddr, sizeof(serveraddr)))
    {
        std::cout<<"bind failed"<<std::endl;
        return false;
    }
    
    if(-1 == listen(listenfd_, 50))
    {
        std::cout<<"listen failed"<<std::endl;
        return false;
    }
    struct pollfd fd;
    fd.fd = listenfd_;
    fd.events = POLLIN;
    channels.push_back(fd);
    Channel* ch = new Channel(listenfd_);
    ch->SetReadCallback(ConnectCB, this);
    chs_.insert(std::pair<int, Channel*>();
    while ( true )
    {
        int eventnum;
        eventnum = poll(&channels[0], channels.size(), -1);
        if(eventnum > 0)
        {
		    for(int i = 1; i < eventnum; ++i)
			{
			    if( channels[i].revents & POLLIN)
				{
				    Channel* ch = chs_[channels[i].fd].second;
				    ch->HandleRead();
				}
			}
/*             if( channels[0].revents & POLLIN)
            {
                int connfd;
                int socklen = sizeof(sockaddr);
                connfd = accept(channels[0].fd, (sockaddr *)&peeraddr, (socklen_t *)&socklen);
                if( connfd > 0 )
                {
                    fd.fd = connfd;
                    fd.events = POLLIN;
                    std::cout<<"accept new connection from "<<peeraddr.sin_addr.s_addr<<":"<<peeraddr.sin_port<<std::endl;
                    channels.push_back(fd);
                }
            }
        
            for(int i = 1; i < eventnum; ++i)
            {
                if( channels[i].revents & POLLIN)
                {
                    if(read(channels[i].fd, buf, 6000) > 0)
                    {
                        std::cout<<"**************recive date*************"<<std::endl;
                        std::cout<<buf<<std::endl;
                        std::cout<<"**************************************"<<std::endl;
                    }
                    
                }
            } */
        }
        else
        {
            std::cout<<" event error: "<<errno<<std::endl;
        }
    }
    return true;
}

void HttpServer::AddChannel(int fd, Channel* chn)
{
    chs_.insert(std::pair<int, Channel*>(fd, chn));
}

void HttpServer::OnConnection()
{
	int connfd;
	int socklen = sizeof(sockaddr);
	connfd = accept(listenfd_, (sockaddr *)&peeraddr, (socklen_t *)&socklen);
	if( connfd > 0 )
	{
		fd.fd = connfd;
		fd.events = POLLIN;
		std::cout<<"accept new connection from "<<peeraddr.sin_addr.s_addr<<":"<<peeraddr.sin_port<<std::endl;
		channels.push_back(fd);
		Channel* ch = new Channel(connfd);
		ch->SetReadCallback(ReadCB, connfd);
		chs_.insert(std::pair<int, Channel*>(connfd, ch));
		//chn->SetWriteCallback();
	}
}




