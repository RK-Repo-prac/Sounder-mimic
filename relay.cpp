#include "relay.h"



Relay::Relay()
{
   sock_obj_=std::make_unique<communication>();
}




std::function<std::string(int)> Relay::receive_msg(){
sock_obj_->create_socket(port_,pos_);
   recv_msg_= [this](int pos){
         return this->sock_obj_->recv_msg(this->pos_);
   };
   return recv_msg_;
}

Relay::~Relay(){}