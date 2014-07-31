#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <vector>
#include <map>
#include <poll.h>

class Channel;

class HttpServer
{
private:
    explicit HttpServer(short port);
    ~HttpServer() {};
public:
    typedef void (*functor)(void* arg);

    
    bool Start();
    void AddChannel(int fd, Channel*); 
    void OnConnection();
    void OnRead(int fd);
public:
    static HttpServer* GetInstance();
private:
    short port_;
    std::vector<pollfd> channels;
    std::map<int, Channel*> chs_;
    int listenfd_;
private:
    static HttpServer* instance;    
};

struct EventFileDes
{
    HttpServer* server
    int fd;
};

#endif
