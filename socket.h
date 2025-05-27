#pragma once

#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include "config.h"



class communication{
   public:

   communication();
   ~ communication();
   int create_socket(int,int);
   void send_msg(std::string,int);
   std::string recv_msg(int);

   private:
   std::vector<int> socket_fd;
   struct sockaddr_in socket_addr_;
   short	sin_family;
   int address_family_;
   int socket_type_;
   int protocol_;
   WSADATA wsaData;

};


#endif