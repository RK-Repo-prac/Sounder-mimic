#pragma once
#ifndef FUSION_H
#define FUSION_H
#include "socket.h"
#include<memory>
#include<functional>
#include<thread>
#include<queue>
#include<condition_variable>
#include "comm_base.h"
#include "util.h"

class Fusion{
    
    public:
    Fusion();
    ~Fusion();
   void start();
    
    private:
     std::unique_ptr<CommunicationManager> comm_manager_;
   
};

#endif
