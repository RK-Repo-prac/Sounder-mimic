#pragma once
#ifndef RELAY_H
#define RELAY_H
#include "socket.h"
#include<memory>
#include<functional>

class Relay{
   
    public:
    Relay();
    ~ Relay();
    std::function<std::string(int)> receive_msg();
    private:
    int port_{RELAY_PORT};
    int pos_{RELAY_POS};
    std::unique_ptr<communication> sock_obj_;
    std::function<std::string(int)> recv_msg_;
};

#endif
