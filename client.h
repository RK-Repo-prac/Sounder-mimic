#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include<memory>
#include<functional>
#include<thread>
#include<queue>
#include<condition_variable>
class Client: public communication{
   
    public:
    Client();
    ~ Client();
    void client_Threads();
    void send_mgs_to_Relay();
    private:
    std::function<std::string(int)> recv_msg_;
    std::shared_ptr<communication> client_sock_;
    std::thread recv_thread_Relay_;
    std::thread recv_thread_Fusion_;
    std::thread send_thread_Relay_;
    std::queue<std::string> Fusion_msgs_;
    std::queue<std::string> Relay_msgs_;
    std::mutex Fusion_msg_mutex_;
    std::mutex Relay_msg_mutex_;
    std::condition_variable Fusion_cv_;
    std::condition_variable Relay_cv_;

};

#endif
