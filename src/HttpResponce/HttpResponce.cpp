#include "HttpResponce.hpp"
#include "../codeHandler/codeHandler.hpp"

#include <fstream>
#include <sstream>

#include <iostream>

HttpResponce::HttpResponce(HttpRequest req){
    setResponce(req);
}

HttpResponce::~HttpResponce(){}

void HttpResponce::parseHtml(std::string& html){    
    const std::string cppStartTag = "<cpp>";
    const std::string cppEndTag = "</cpp>";
    
    std::string temp = html;
    
    //remove <script>, <style> and <cpp> for html
    removeSub(html, "<link rel=\"stylesheet\"", "ss\">");
    removeSub(html, "<script src=", "</script>");

    removeSub(html, cppStartTag, cppEndTag);

    html += ("<style>\n" + findCss(temp) + "\n</style>");
    html += ("<script>\n" + findJs(temp) + "\n</script>");

    //do cpp
    codeHandler codeHandle;
    std::string out = " ";

    try{
        std::string binFName = codeHandle.compileCode(temp);
        
        out = codeHandle.runCode(binFName);
    
    }catch(const std::runtime_error& e){
        std::cerr << "error:"<<e.what() << std::endl;

        exit(0);
    
    }catch(...){
        return;
    }

    html.insert(temp.find(cppStartTag) - 30, out);
}

void HttpResponce::removeSub(std::string& str, std::string start, std::string end){
    int startPos = str.find(start);

    while(startPos != std::string::npos){
        str.erase(startPos, str.find(end, startPos) + end.length());

        startPos = str.find(start, startPos);
    }
}

std::string HttpResponce::findCss(std::string html){
    std::string str, file = "", buf = ""; 
    int pos = 0, start = 0, end = 0;

    while((start = html.find("<link", start)) != std::string::npos){
        end = html.find('>', start);

        if(end == std::string::npos)
            return "";

        str = html.substr(start, (end - start) + 1);

        if(str.find("rel=\"stylesheet\"") != std::string::npos){
            pos = str.find("href=\"") + 6;

            while(str.at(pos) != '\"'){
                file += str.at(pos);

                pos++;
            }

            buf += readFile("../build/" + file) + "\n";

            file.clear();
        }

        start++;
    }

    return buf;
}

std::string HttpResponce::findJs(std::string html){
    const std::string find = "<script src=\"";
    std::string buf = "", file = "";
    int start = 0;

    while((start = html.find(find, start)) != std::string::npos){
        start += find.length();

        while(html.at(start) != '\"'){
            file += html.at(start);

            start++;
        }

        buf += readFile("../build/" + file);
        file.clear();
    }

    return buf;
}

void HttpResponce::setResponce(HttpRequest req){
    if(req.getType() == "GET"){
        responce = handleGet(req);
    
    }else if(req.getType() == "PUT"){
        responce = handlePut(req);

    }else if(req.getType() == "POST"){
        responce = handlePost(req);
    
    }else if(req.getType() == "DELETE"){
        responce = handleDelete(req);
    }
}

std::string HttpResponce::getResponce(){
    return responce;
}

std::string HttpResponce::readFile(std::string fName){
    std::ifstream file(fName);
    std::stringstream buf;

    if(!file.is_open()){
        file.open("../build/error.html");

        if(!file.is_open())
            return " ";
    }

    buf << file.rdbuf();

    file.close();

    return buf.str();
}



std::string HttpResponce::handleGet(HttpRequest req){
    const std::string status = "HTTP/1.1 200 OK\r\n";
    std::string html = readFile(req.getFile());

    parseHtml(html);

    std::string headers = "Server: PrinceServer\r\nContent-Type: text/html\r\n"; //todo check ntotes

    return status + headers + "\r\n" + html;
}

std::string HttpResponce::handlePut(HttpRequest req){
    return "501 Not Implemented\r\n"; 
}

std::string HttpResponce::handlePost(HttpRequest req){
    return "501 Not Implemented\r\n"; 
}

std::string HttpResponce::handleDelete(HttpRequest req){
    return "501 Not Implemented\r\n"; 
}