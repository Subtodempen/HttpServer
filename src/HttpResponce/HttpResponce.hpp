#pragma once

#include"../HttpRequest/HttpRequest.hpp"

class HttpResponce{
    public:
        HttpResponce(HttpRequest req); // input a http request class
        ~HttpResponce();
        
        std::string getResponce();
    private:
        std::string responce;
        void setResponce(HttpRequest req);

        std::string readFile(std::string fName);
        
        void parseHtml(std::string& html);
        void removeSub(std::string& str, std::string start, std::string end);

        std::string findCss(std::string html);
        std::string findJs(std::string html);

        std::string handleGet(HttpRequest req);
        std::string handlePut(HttpRequest req);
        std::string handlePost(HttpRequest req);
        std::string handleDelete(HttpRequest req);

};
