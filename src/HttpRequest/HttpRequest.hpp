#pragma once

#include <unordered_map>
#include <string>
#include <vector>


class HttpRequest{
    public:
        HttpRequest(std::string request);
        ~HttpRequest();

        HttpRequest(HttpRequest& req);

        const std::string getType(); // returns Type string so Get or Head
        const std::string getFile();
        const std::unordered_map<std::string, std::string> getHeader();
        const std::string getBody();
    
    private:
        std::string Type;
        std::string File;
        std::unordered_map<std::string, std::string> Headers;
        std::string Body;

        void setType(std::string first);
        void setFile(std::string first);
        
        void setHeaders(std::vector<std::string> lines);
        void setBody(std::string request);

        bool checkSyntax(std::string first);

        bool parserReq(std::string request); // returns 0 on fail and 1 on compleion also sets the varaibles
        
        std::vector<std::string> splitText(std::string subj, std::string deli);
};
