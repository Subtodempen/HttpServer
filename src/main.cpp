#include "Socket/socket.hpp"
#include "HttpRequest/HttpRequest.hpp"
#include "HttpResponce/HttpResponce.hpp"

#include <iostream>

#define PORT 1234


int main(){
    SocketHelper socket( PORT );

    if(socket.bindSock() == -1){
        socket.bindSock(PORT + 2);
    }


    socket.startListen();
    
    int clientSocket;
    
    for(;;){
        clientSocket = socket.acceptClient();
    
        std::string request = socket.recvData(clientSocket);

        HttpRequest requestHandle(request);
        HttpResponce responceHandle(requestHandle);
        
        socket.sendData(clientSocket, responceHandle.getResponce());
    
        shutdown(clientSocket, 2);
    }

    return 1;
}
