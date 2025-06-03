#include "client.h"



Client::Client()
{
client_sock_=std::make_shared<communication>();
client_sock_->create_socket(CLIENT_FUSION_PORT,"client_fusion");
client_sock_->create_socket(CLIENT_RELAY_PORT,"client_relay");
}


void Client::client_Threads()
{
   recv_thread_Fusion_=std::thread(recv_msg,"client_fusion",Fusion_msgs_,Fusion_msg_mutex_,Fusion_cv_);
   recv_thread_Relay_=std::thread(recv_msg,"client_relay",Relay_msgs_,Relay_msg_mutex_,Relay_cv_);
   send_thread_Relay_ = std::thread(&Client::send_mgs_to_Relay, this);
}



void Client::send_mgs_to_Relay(){
   while(1){
      std::unique_lock<std::mutex>lock(Fusion_msg_mutex_);
      Fusion_cv_.wait(lock,[this](){
           return !Fusion_msgs_.empty();

      });
         std::string msg=Fusion_msgs_.front();
         Fusion_msgs_.pop();
         lock.unlock();
         client_sock_->send_msg(msg,"relay_client",client_sock_->return_ss_addr());
   }
}


Client::~Client()
{
  if(recv_thread_Fusion_.joinable()){
   recv_thread_Fusion_.join();
  }

  if (recv_thread_Relay_.joinable()) {
        recv_thread_Relay_.join();
    }
  if (send_thread_Relay_.joinable()) {
        send_thread_Relay_.join();
    }
}