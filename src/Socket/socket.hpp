#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

class SocketHelper{
    public:
        SocketHelper(const int PORT);
        ~SocketHelper();
        
        int bindSock();
        int bindSock(const int PORT);
        int startListen();
        int acceptClient();// returns client socket 
        
        std::string recvData(int clientSock);  //takes in client socket
        int sendData(int clientSock, std::string data);  //again takes in he client socket

    private:
        int sockDesc;
        struct sockaddr_in server;

};
