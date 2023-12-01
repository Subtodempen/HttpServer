#include "HttpServer/HttpServer.hpp"

#include <iostream>
#include <future>

int main(){//not yet but config file will be an arg
    HttpServer http;
    http.welcome();

    std::future<void> EventualValue = std::async(std::launch::async, &HttpServer::serverRun, &http); //create a func

    http.serverRun();

    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds

    sleep_for(nanoseconds(1000));
    
    http.setIsRunning(false);

    return 1;
}
