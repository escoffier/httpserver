#include <iostream>
#include "httpserver.h"

int main(int argc, char **argv )
{
    std::cout<<"start http server"<<std::endl;
    //HttpServer server(8081);
    HttpServer::GetInstance()->Start();
    return 0;
}
