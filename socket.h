#pragma once

#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include "config.h"
#include <map>
#include <mutex>
#include<queue>
#include<condition_variable>
#include<chrono>
#include<functional>

class communication{
   public:
   communication();
   ~ communication();
   void create_socket(int,std::string);
   void send_msg(std::string,std::string,const struct sockaddr_in,const std::string &);
   void recv_msg(std::string,std::queue<std::string> &,std::mutex &,std::condition_variable&,std::function<void()> recievecallback = nullptr);
   const struct sockaddr_in& return_ss_addr()const {return ss_;}
   struct sockaddr_in get_map_addr(const std::string);

   private:
   std::map<std::string,std::pair<int,sockaddr_in>> socket_fd_;
   short	sin_family;
   int address_family_;
   int socket_type_;
   int protocol_;
   struct sockaddr_in ss_;

};

#endif