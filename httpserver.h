#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <poll.h>

class HttpServer
{
public:
    explict HttpServer(short port);
    ~HttpServer() {};
    
	bool Start();
	
private:
    short port_;
	std::vector<pollfd> channels;
};
#endif
