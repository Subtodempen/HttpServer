#pragma once

#include <unistd.h>
#include <string>
#include <type_traits>

namespace WEB{

template <typename T>
void Print(const T &input, int pipeFd){
    // write to pipe
    size_t bytesWritten;
    
    if constexpr (std::is_same_v<T, std::string>)
        bytesWritten = write(pipeFd, input.c_str(), input.length());

    else {
        const char* buffer = reinterpret_cast<const char *>(&input);
        const size_t size = sizeof(input);

        bytesWritten = write(pipeFd, buffer, size);
    }

    close(pipeFd);
}

inline int ctoi(char* c){ //converts char* to int
    return stoi(std::string(c));
}

}