#include "fusion.h"

Fusion::Fusion(){

   comm_manager_=std::make_unique<CommunicationManager>();
   ConnectionConfig daemon_config={
      .local_port=FUSION_DAEMON_PORT,
      .local_key="fusion_daemon",
      .remote_ip="127.0.0.1",
      .remote_port=DAEMON_FUSION_PORT,
      .remote_key = "daemon",
   };

   ConnectionConfig client_config = {
        .local_port = FUSION_CLIENT_PORT,
        .local_key = "fusion_client",
        .remote_ip = "127.0.0.1",
        .remote_port = CLIENT_FUSION_PORT,
        .remote_key = "client"
    };

    comm_manager_->add_connection(client_config.remote_key,client_config);
    comm_manager_->add_connection(daemon_config.remote_key,daemon_config);
    



}

void Fusion::start(){
   LOG("Starting Fusion Threads");
   comm_manager_->start_all_threads();
}


Fusion::~Fusion()= default;