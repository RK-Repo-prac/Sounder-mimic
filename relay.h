#pragma once
#ifndef RELAY_H
#define RELAY_H
#include "comm_base.h"
#include "util.h"

class Relay {
public:
    Relay();
    ~Relay();
    void start();
    
private:
    std::unique_ptr<CommunicationManager> comm_manager_;
};

#endif