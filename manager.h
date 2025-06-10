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
#include "client.h"
#include "daemon.h"
#include "threadManager.h"
#include "fusion.h"
#include "util.h"
#include <chrono>

class Manager{
    public:
    Manager(std::string routing,int);
  ~ Manager();
    void start_threads();
    void relayThread();
    void clientThread();
    void FusionThread();
    void DaemonThread();
    bool Exit_Status();
    private:
     int thread_count_;
     std::string routing_;
     std::thread manager_thread_;
     bool routing_success_={false};
     std::mutex routinglock_;
     std::mutex exitmutex;
     std::condition_variable routing_cv_;
     std::unique_ptr<ThreadManager> thread_manager_;
     bool exit_status{false};

};

#endif
