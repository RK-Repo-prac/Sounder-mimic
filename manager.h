#pragma once


#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include<iostream>
#include<string>
#include<thread>
#include <sstream>
#include<vector>
#include<memory>
#include "relay.h"
#include "socket.h"
class Manager{
    public:
    Manager(std::string routing);
  ~ Manager();
    void start_threads();
    bool read_config();
    void relayThread();
    void clientThread();
    private:
     int thread_count_;
     std::string routing_;
     std::thread manager_thread_;
     std::shared_ptr<Relay> relay;
     std::shared_ptr<communication> sock_obj_;
    

};

#endif
