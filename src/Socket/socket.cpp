#include "socket.hpp"


SocketHelper::SocketHelper(const int PORT){
    sockDesc = socket(AF_INET, SOCK_STREAM, 0);

    if(sockDesc == -1){
        std::cerr<<"can not open socket";

        exit(0);//chnage for later
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
}

SocketHelper::~SocketHelper(){
    shutdown(sockDesc, 2);
}

int SocketHelper::bindSock(){
    if(bind(sockDesc, reinterpret_cast<struct sockaddr*>(&server), sizeof(server))){
        std::cerr<<"Could not bind with port "<<(int)server.sin_port<<" Trying again\n";

        return -1;
    }

    return 1;
}

int SocketHelper::bindSock(const int PORT){
    server.sin_port = htons( PORT );

    return bindSock();
}

int SocketHelper::startListen(){
    if(listen(sockDesc, 20) == -1){
        //errro
        std::cerr<<"listen errir";

        exit(0); // handle errors betetr
    }

    return 1;
}

int SocketHelper::acceptClient(){
    struct sockaddr_in client;
    int clientSock; 

    socklen_t sockSize = static_cast<socklen_t>(sizeof(struct sockaddr_in));
    
    clientSock = accept(sockDesc, reinterpret_cast<struct sockaddr *>(&client), &sockSize);

    return clientSock;
}

std::string SocketHelper::recvData(int clientSock){
    char clientMsg[200];
        
    if(recv(clientSock, clientMsg, 200, 0) == -1){
        std::cerr<<"could not recv Error is: ";// implement cerrno errors TODO
    }

    return std::string(clientMsg);
}

int SocketHelper::sendData(int clientSock, std::string data){
    if(send(clientSock, data.c_str(), data.length(), 0) == -1){
        std::cerr<<"send error";

        return -1;
    }

    return 1;
}
