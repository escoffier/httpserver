#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdio.h>
#include <arpa/inet.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char **argv )
{
    int fd;
	
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd < 0)
	{
	    std::cout<<"socket error: "<<errno<<std::endl;
		return 0;
	}
	
    struct sockaddr_in in;
	memset(&in, 0, sizeof(struct sockaddr_in));
	
	in.sin_family = AF_INET;
	in.sin_port = htons(8081);
	short addr = 0;
	if(1 != inet_pton(AF_INET, "192.168.60.109", &in.sin_addr))
	{
	    std::cout<<"inet_pton error: "<<errno<<std::endl;
	    return 0;
	}
	
	if( -1 == connect(fd, (struct sockaddr*)&in, sizeof(struct sockaddr_in)) )
	{
	    std::cout<<"connect error: "<<errno<<std::endl;
		return 0;
	}
	
	fd_set rset;
	int nfd;
	int res;
	char buf[2048] = {0};
	while(1)
	{
	    FD_SET(fileno(stdin), &rset);
	    FD_SET(fd, &rset);
	
	    nfd = MAX(fileno(stdin), fd);
		
		res = select(nfd, &rset, 0, 0, 0);
		
		if(res > 0)
		{
		    if(FD_ISSET(fd, &rset))
			{
			    res = read(fd, buf, 2048);
				if(res == 0)
				{
				    break;
				}
				
				write(fileno(stdout), buf, res);
			}
			
			if(FD_ISSET(fileno(stdin), &rset))
			{
				res = read(fileno(stdin), buf, 2048);
				if(res == 0)
				{
				    break;
				}
				
				write(fd, buf, res);
			}
		}
	}
    std::cout<<"start http server"<<std::endl;
    //HttpServer server(8081);
    return 0;
}
