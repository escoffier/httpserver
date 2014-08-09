#include "httpserver.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "channel.h"

void ConnectCB(void* arg)
{
    //HttpServer* server = (HttpServer*) arg;
    HttpServer::GetInstance()->OnConnection();
}

void ReadCB(void *arg)
{  
    char buf[1024] = {0};
    int fd = *((int*)(&arg));
    HttpServer::GetInstance()->OnRead(fd);
}

HttpServer* HttpServer::instance = NULL;

HttpServer* HttpServer::GetInstance()
{
    if(instance == NULL)
    {
        instance = new HttpServer(8081);
    }

    return instance;
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
    int flag = fcntl(listenfd_, F_GETFL, NULL);
    if(flag < 0)
    {
        std::cout<<"fcntl, "<<flag<<std::endl;
        return false;
    }
    flag = fcntl(listenfd_, F_SETFL, flag|O_NONBLOCK);
    if(flag < 0)
    {
        std::cout<<"fcntl set fail: "<<flag<<std::endl;
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
    std::cout<<"listen fd: "<<listenfd_<<std::endl;
    struct pollfd fd;
    fd.events = 0;
    fd.revents = 0;
    fd.fd = listenfd_;
    fd.events |= POLLIN;
    channels.push_back(fd);
    Channel* ch = new Channel(listenfd_);
    ch->SetReadCallback(ConnectCB, this);
    chs_.insert(std::pair<int, Channel*>(listenfd_, ch));
    while ( true )
    {
        int eventnum;
        eventnum = poll(&channels[0], channels.size(), -1);
        std::cout<<"event fd number: "<<eventnum<<std::endl;
        if(eventnum > 0)
        {
            for(int i = 0; i <channels.size(); ++i)
            {
                if( channels[i].revents & POLLIN)
                {   
                    std::cout<<"event: "<<channels[i].revents<<std::endl;
                    std::cout<<"detect event on "<<channels[i].fd<<std::endl;
                    Channel* ch = chs_[channels[i].fd];
                    ch->HandleRead();
                }
            }
        }
        else if(eventnum < 0)
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
    struct sockaddr_in peeraddr;
    struct pollfd fd;
    fd.events = 0;
    fd.revents = 0;
    int socklen = sizeof(sockaddr);
    connfd = accept(listenfd_, (sockaddr *)&peeraddr, (socklen_t *)&socklen);
    if( connfd > 0 )
    {
        channels[0].revents = 0;
        fd.fd = connfd;
        fd.events |= POLLIN;
        std::cout<<"accept new connection("<<connfd<<") from "<<peeraddr.sin_addr.s_addr<<":"<<peeraddr.sin_port<<std::endl;
        channels.push_back(fd);
        Channel* ch = new Channel(connfd);
        ch->SetReadCallback(ReadCB,(void*)connfd);
        chs_.insert(std::pair<int, Channel*>(connfd, ch));
        //chn->SetWriteCallback();
    }
    else if(connfd < 0)
    {
        if(EAGAIN == errno)
        {
            std::cout<<"accept return cause EAGAIN"<<std::endl;
        }
    }
}

void HttpServer::OnRead(int fd)
{
    char buf[1024] = {0};
    //int fd = *((int*)(&arg));
    std::cout<<"read fd: "<<fd<<std::endl;
    int n = read(fd, buf, 6000);
    if(n > 0)
    {
        std::cout<<"**************recive date*************"<<std::endl;
        std::cout<<buf<<std::endl;
        std::cout<<"**************************************"<<std::endl;
    }
    else if(0 == n)
    {
        close(fd);
        std::cout<<"connection close"<<std::endl;
        delete(chs_[fd]);
        chs_.erase(fd);
        for(std::vector<pollfd>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if(fd == it->fd)
            {
                channels.erase(it);
                break;
            }
        }
    }
    else
    {
        std::cout<<"read error: "<<errno<<std::endl;
    }
}




