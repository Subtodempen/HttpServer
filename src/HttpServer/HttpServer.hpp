#pragma once

#include "../Socket/socket.hpp"
#include "../HttpRequest/HttpRequest.hpp"
#include "../HttpResponce/HttpResponce.hpp"

#include <thread>
#include <mutex>

class HttpServer{
    public:
        HttpServer(): isRunning(false), verbose(true){} //change in config
        
        void welcome();

        void serverRun();
        void serverRunThreads();
        //void serverRunNonBlock();

        void setIsRunning(bool value){
            std::lock_guard<std::mutex> lock(isRunningMutex);

            isRunning = value;
        }

        bool getIsRunning(){
            std::lock_guard<std::mutex> lock(isRunningMutex);
            
            return isRunning;
        }

    private:
        bool isRunning;
        std::mutex isRunningMutex;

        bool verbose; // verbose decides whether to print or not to

        int setupSock(SocketHelper& socket);
        void handleClient(int clientSock);

        inline void log(const std::string& input);
};