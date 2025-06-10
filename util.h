#pragma once
#ifndef UTIL_H
#define UTIL_H
#include<iostream>
#include<string>
#include<mutex>
#include <sstream>


extern std::mutex Logmutex;

inline void message_out(const char* file, int line, const char* func, const std::string& msg) {
    std::cout << "[" << file << ":" << line <<"] " <<func<<":"<< msg << std::endl;
}


#define LOG(msg)                                     \
    do {                                               \
        std::unique_lock<std::mutex>coutlock(Logmutex);   \
        std::ostringstream oss;                             \
        oss << msg;                                         \
        message_out(__FILE__, __LINE__, __func__, oss.str()); \
    } while(0)



#endif