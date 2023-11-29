#include "HttpRequest.hpp"

#include <regex>
#include <iostream>
#include <string>
#include <unordered_map>

HttpRequest::HttpRequest(std::string request){
    if(!parserReq(request)){
        std::cerr<<"could not parse";
        
        exit(0); // change?
    }
}

HttpRequest::~HttpRequest(){} // DO SOMETHING

HttpRequest::HttpRequest(HttpRequest& req){
    Type = req.getType();
    File = req.getFile();
    Headers = req.getHeader();
    Body = req.getBody();    
}

const std::string HttpRequest::getType(){
    return Type;
}

const std::string HttpRequest::getFile(){
    return File;
}

const std::unordered_map<std::string, std::string> HttpRequest::getHeader(){
    return Headers;
}

const std::string HttpRequest::getBody(){
   return Body;
}

bool HttpRequest::parserReq(std::string request){
    std::vector<std::string> lines = splitText(request + "\r\n", "\r\n");

    if(!checkSyntax(lines.at(0)))
        return false;
        
    setType(lines.at(0));
    setFile(lines.at(0));
    setHeaders(lines);
    
    setBody(request);

    return true;
}

std::vector<std::string> HttpRequest::splitText(std::string str, std::string deli){
    std::vector<std::string> buf;
    
    while(str.size()){
        int index = str.find(deli);
        
        if(index != std::string::npos){
            buf.push_back(str.substr(0, index));
            str = str.substr(index + deli.size());
            
            if(str.size() == 0)
                buf.push_back(str);
        
        }else{
            buf.push_back(str);
            str = "";
        }
    }

    return buf;
}

bool HttpRequest::checkSyntax(std::string first){
    std::regex generalSyntax("^(GET|POST|PUT|DELETE) /([^ ]*) HTTP/1.1");
    std::smatch match;
    
    if(!std::regex_match(first, match, generalSyntax))
        return false;

    return true;
}

void HttpRequest::setType(std::string first){
    Type = first.substr(0, first.find(" "));
}

void HttpRequest::setFile(std::string first){ // first line of the http file
    int slashIndex = first.find("/");
    File = "";

    if(first.at(slashIndex + 1 ) != ' '){
        slashIndex++;
        
        while(first.at(slashIndex) != ' '){
            File += first.at(slashIndex);

            slashIndex++;
        }

        File += ".html";
    }else
        File = "index.html";
}

void HttpRequest::setHeaders(std::vector<std::string> lines){
    std::vector<std::string> buf;
    lines.erase(lines.begin());

    for(auto str : lines){
        if(str.empty()) // this is start of body
            break;

        buf = splitText(str, ":");

        Headers[buf.at(0)] = buf.at(1);
        
        buf.clear();
    }
}

void HttpRequest::setBody(std::string request){
    int start = request.find("\r\n\r\n");

    if(start != std::string::npos){
        Body = request.substr( start, (request.length() - start) );

    }else{ // no body
        Body = "";

    }
}