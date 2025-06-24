#include "client.h"

Client::Client(){

   comm_manager_=std::make_unique<CommunicationManager>();
   ConnectionConfig relay_config={
      .local_port=CLIENT_RELAY_PORT,
      .local_key="client_relay",
      .remote_ip="127.0.0.1",
      .remote_port=FUSION_CLIENT_PORT,
      .remote_key = "fusion",
   };

   ConnectionConfig fusion_config = {
        .local_port = CLIENT_FUSION_PORT,
        .local_key = "client_fusion",
        .remote_ip = "127.0.0.1",
        .remote_port = RELAY_CLIENT_PORT,
        .remote_key = "relay"
    };

    comm_manager_->add_connection(relay_config.remote_key,relay_config);
    comm_manager_->add_connection(fusion_config.remote_key,fusion_config);
    
  ConnectionConfig daemon_coonfig={
    .local_port=CLIENT_DAEMON_PORT,
    .local_key="client-daemon",
    .remote_ip="127.0.0.1",
    .remote_port=DAEMON_CLIENT_PORT,
    .remote_key="daemon"
  };
   
  comm_manager_->add_connection(daemon_coonfig.remote_key,daemon_coonfig);
}

void Client::start(){
   LOG("Starting Client Threads");
   comm_manager_->start_all_threads();
}


Client::~Client()= default;