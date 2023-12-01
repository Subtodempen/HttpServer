#include "HttpServer/HttpServer.hpp"

#include <iostream>
#include <future>

int main(){//not yet but config file will be an arg
    HttpServer http;
    http.welcome();

    std::future<void> server = std::async(std::launch::async, &HttpServer::serverRun, &http); //create a func

    char c = ' ';

    while(c != 'q')
        std::cin>>c;
    
    http.setIsRunning(false);
    
    return 1;
}
