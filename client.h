#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include<memory>
#include<functional>
#include<thread>
#include<queue>
#include<condition_variable>
#include "comm_base.h"
#include "util.h"

class Client{
    
    public:
    Client();
    ~Client();
   void start();
    
    private:
     std::unique_ptr<CommunicationManager> comm_manager_;
   
};

#endif
