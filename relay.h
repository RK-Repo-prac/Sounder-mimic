#pragma once
#ifndef RELAY_H
#define RELAY_H
#include "comm_base.h"

class Relay {
public:
    Relay();
    ~Relay();
    void start();
    
private:
    std::unique_ptr<CommunicationManager> comm_manager_;
    void handle_ss_message(const std::string& message);
    void handle_client_message(const std::string& message);
};

#endif