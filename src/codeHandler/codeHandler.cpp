#include "codeHandler.hpp"

#include <iostream>

std::string codeHandler::compileCode(std::string html){
    std::vector<std::string> code = getCode(html);
    std::string binFName;

    const std::string INCLUDE = "#include\"../include/webInclude/webIO.hpp\"\n";
    const std::string MAIN = "int main(int argc, char *argv[]){";

    if(code.empty()){
        throw std::invalid_argument("No cpp tag");

        return {}; //std::string() or empty strimng
    }

    
    code.insert(code.begin(), INCLUDE);
    code.insert(code.begin() + 1, MAIN);
    
    code.push_back("\n}");

    binFName = compile(code);

    return binFName;
}

std::string codeHandler::runCode(const std::string& binFName){
    return run(binFName);
}

std::vector<std::string> codeHandler::getCode(std::string html){
    //extract cpp from html
    const std::string startTag = "<cpp>";
    const std::string endTag = "</cpp>";

    std::vector<std::string> buf;
    std::string code, str = "";

    int start = 0;
    start = html.find(startTag);
    
    while(start != std::string::npos){
        code = html.substr(start + startTag.length(), (html.find(endTag, start) - start) - endTag.length());

        for(char c : code){
            if(c == ';' || c == '}'){
                buf.push_back(str + c);
            
                str.clear();
            }
            else
                str += c;

        }

        start = code.find(startTag, start);
    }


    return buf;
}

std::string codeHandler::compile(std::vector<std::string> code){
    //first wrte code to a cpp file than compile said file 
    //TODO fix files 
    const std::string fName = "build.cpp";
    
    const std::string binFile = "bin";
    const std::string command = "g++ " + fName + " -o " + binFile;

    clearFile(fName); // makesure file is empty

    for(std::string line : code)
        appendFile(fName, line); 
    
    
    if(system(NULL) == 0){
        throw std::runtime_error("can not run system(); No shell available");

        return {};
    }

    
    if(system(command.c_str()) == -1){
        throw std::runtime_error("Could not compile");

        return {};
    }

    return binFile;
}

std::string codeHandler::run(std::string binFName){
    //pipe setup
    int pipeFd[2];

    if(pipe(pipeFd) == -1)
        throw std::invalid_argument("can not pipe output; program continuies with no output");
    
    
    pid_t pid = fork();

    if(pid < 0){
        throw std::runtime_error("Can not fork");
    
        return {};
    }else if(pid == 0){
        //child process
        static char *argv[2] = {0};
        static char *envp[] = {0};

        argv[0] = &binFName.front();                      //origanly i used c_str() but it returned const char* and that didnt work with execve so
        argv[1] = &std::to_string(pipeFd[1]).front();     // i used const_cast<char*> to remove the const but that leaded to undefined behavour as // other parts of the code may assume its still const so i use .data()instead
        
        execve(binFName.c_str(), argv, envp); // no args first isn argv
    
        throw std::runtime_error("can not execve"); //on comletian execv doesnt return
    }
    //parent 
    
    //read pipe
    return readPipe(pipeFd);
}


std::string codeHandler::readPipe(int pipeFd[]){
    close(pipeFd[1]);

    std::string pipeData = "";
    char buf;

    while(read(pipeFd[0], &buf, 1) > 0)
        pipeData += buf;

    close(pipeFd[0]);

    return pipeData;
}

void codeHandler::clearFile(std::string fName){
    std::ofstream File;
    
    File.open(fName, std::ofstream::out | std::ofstream::trunc);
    
    File.close();
}

void codeHandler::appendFile(std::string fName, std::string data){
    std::ofstream File;

    File.open(fName, std::ios_base::app); // append instead of overwrite
    File << data;

    File.close(); 
}