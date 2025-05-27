#pragma once


#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include<iostream>
#include<string>
#include<thread>
#include <sstream>
#include<vector>
#include "threadManager.h"
#include<memory>
#include "relay.h"
class Manager{
    public:
    Manager(std::string routing,int t_count);
  ~ Manager();
    void start_threads();
    bool read_config();
    private:
     int thread_count_;
     std::string routing_;
     std::thread manager_thread_;
     std::vector<std::string> botlist_;
     std::shared_ptr<ThreadManager> threadpool_;
     std::shared_ptr<Relay> relay;
    

};

#endif
