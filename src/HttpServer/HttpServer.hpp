#pragma once

#include "../Socket/socket.hpp"
#include "../HttpRequest/HttpRequest.hpp"
#include "../HttpResponce/HttpResponce.hpp"

#include <thread>
#include <atomic>

class HttpServer{
    public:
        HttpServer(): isRunning(false), verbose(true){} //change in config
        
        void welcome();

        void serverRun();
        void serverRunThreads();
        //void serverRunNonBlock();

        void setIsRunning(bool value){
            isRunning.store(value, std::memory_order_relaxed);
        }

        bool getIsRunning(){
            return isRunning.load(std::memory_order_relaxed);
        }

    private:
        std::atomic<bool> isRunning;
        
        bool verbose; // verbose decides whether to print or not to

        int setupSock(SocketHelper& socket);
        void handleClient(int clientSock);

        inline void log(const std::string& input);
};