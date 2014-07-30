#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <vector>
#include <map>
#include <poll.h>

class Channel;

class HttpServer
{
public:
    explicit HttpServer(short port);
    ~HttpServer() {};
    
    bool Start();
    void AddChannel(int fd, Channel*);   
private:
    short port_;
    std::vector<pollfd> channels;
    std::map<int, Channel*> chs_;
};
#endif
