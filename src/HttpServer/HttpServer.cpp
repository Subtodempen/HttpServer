#include "HttpServer.hpp"

#define PORT 1236


void HttpServer::welcome(){
    std::cout<<"Welcome to....."<<std::endl;
    
    std::cout << 
    "██████  ██████  ██ ███    ██  ██████ ███████ \n"
    "██   ██ ██   ██ ██ ████   ██ ██      ██      \n"
    "██████  ██████  ██ ██ ██  ██ ██      █████   \n"
    "██      ██   ██ ██ ██  ██ ██ ██      ██      \n"
    "██      ██   ██ ██ ██   ████  ██████ ███████ \n";

    std::cout<<"Made by Demirel Pehlivan \n";
    std::cout<<"Version: v1.0.0"<<std::endl;
}

int HttpServer::setupSock(SocketHelper &socket){
    //SocketHelper socket( PORT );

    if(socket.bindSock() == -1){
        //unsable to bind socket change port

        return -1;
    }

    socket.startListen();

    return 1;
}

inline void HttpServer::log(const std::string& input){
    if(verbose)
        std::cout<<input<<std::endl;
}

void HttpServer::handleClient(int clientSock){
    std::string request = SocketHelper::recvData(clientSock);

    log("Recieved data sening responce....");

    HttpRequest requestHandle(request);
    HttpResponce responceHandle(requestHandle);

    log("Responce created....");

    SocketHelper::sendData(clientSock, responceHandle.getResponce());
    
    log("Sending responce....");

    shutdown(clientSock, 2);
}

void HttpServer::serverRun(){
    if(getIsRunning()){return;}
    
    setIsRunning(true);

    SocketHelper socket( PORT );
    if(setupSock(socket) == -1){
        std::cerr<<"can not bind program will stop try again"<<std::endl;

        return;
    }

    int clientSocket;

    while((clientSocket = socket.acceptClient()) != -1){
        handleClient(clientSocket);
    
        if(!getIsRunning())
            break;
    }

    setIsRunning(false);
}

void HttpServer::serverRunThreads(){
    if(getIsRunning()){return;}

    setIsRunning(true);

    SocketHelper socket( PORT );
    int clientSocket;

    if(setupSock(socket) == -1){
        std::cerr<<"can not bind program will stop."<<std::endl;

        return;
    }

    while((clientSocket = socket.acceptClient()) != -1){
        //create new thread
        //dont wait for thread to finish

        std::thread thread(&HttpServer::handleClient, this, clientSocket);

        thread.detach();

        if(getIsRunning())
            break;
    }

    setIsRunning(false);
}