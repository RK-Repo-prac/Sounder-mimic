#pragma once
#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include "socket.h"
#include <memory>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>
#include <map>

struct ConnectionConfig {
    int local_port;
    std::string local_key;
    std::string remote_ip;
    int remote_port;
    std::string remote_key;
};

class CommunicationManager {
public:
    CommunicationManager();
    ~CommunicationManager();
    void add_connection(const std::string& name, const ConnectionConfig& config);   
    void start_all_threads();    

private:
    struct Connection {
        ConnectionConfig config;
        std::queue<std::string> incoming_msgs;
        std::queue<std::string> outgoing_msgs;
        std::mutex incoming_mutex;
        std::mutex outgoing_mutex;
        std::condition_variable incoming_cv;
        std::condition_variable outgoing_cv;
        std::thread recv_thread;
        std::thread send_thread;
        struct sockaddr_in remote_addr;
    };
    
    std::shared_ptr<communication> socket_;
    std::map<std::string, std::unique_ptr<Connection>> connections_;
    bool running_;    
    void recv_thread_func(const std::string& connection_name);
    void send_thread_func(const std::string& connection_name);
    void setup_remote_address(Connection& conn);
};

#endif
