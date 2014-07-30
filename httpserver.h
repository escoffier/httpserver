#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <vector>
#include <map>
#include <poll.h>

class Channel;

class HttpServer
{
public:
    typedef void (*functor)(void* arg);
    explicit HttpServer(short port);
    ~HttpServer() {};
    
    bool Start();
    void AddChannel(int fd, Channel*); 
    void OnConnection();	
private:
    short port_;
    std::vector<pollfd> channels;
    std::map<int, Channel*> chs_;
	int listenfd_;
};
#endif
