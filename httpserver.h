#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <vector>
#include <poll.h>

class HttpServer
{
public:
    explicit HttpServer(short port);
    ~HttpServer() {};
    
	bool Start();
	
private:
    short port_;
	std::vector<pollfd> channels;
};
#endif
