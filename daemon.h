#pragma once
#ifndef DAEMON_H
#define DAEMON_H
#include "socket.h"
#include<memory>
#include<functional>
#include<thread>
#include<queue>
#include<condition_variable>
#include "comm_base.h"
#include "util.h"
#include <chrono>
class Daemon{
    
    public:
    Daemon();
    ~Daemon();
   void start();
   void mimic_message();
    
    private:
     std::unique_ptr<CommunicationManager> comm_manager_;
   
};

#endif
