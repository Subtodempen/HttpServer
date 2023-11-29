#pragma once

#include <vector>
#include <fstream>

#include <unistd.h>
#include <cstdlib>


class codeHandler{
    public:
        std::string compileCode(std::string html);                     //try catch
        std::string runCode(const std::string& binFName);                //on succses it returns the output of the progam 
    
    private:
        void clearFile(const std::string fName);                //create a file io class
        void appendFile(const std::string fName, std::string data);

        std::vector<std::string> getCode(std::string html);
        
        std::string compile(std::vector<std::string> code);     //returns a file name to the binary file
        std::string run(std::string binFName);                       //takes in the binary file name 
        
        std::string readPipe(int pipeFd[]);
        //std::string searchForOut();
};