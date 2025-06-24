#include "daemon.h"

Daemon::Daemon(){

   comm_manager_=std::make_unique<CommunicationManager>();
   ConnectionConfig fusion_config={
      .local_port=DAEMON_FUSION_PORT,
      .local_key="daemon_fusion",
      .remote_ip="127.0.0.1",
      .remote_port=FUSION_DAEMON_PORT,
      .remote_key = "fusion",
   };

   ConnectionConfig client_config = {
        .local_port = DAEMON_CLIENT_PORT,
        .local_key = "daemon_client",
        .remote_ip = "127.0.0.1",
        .remote_port = CLIENT_DAEMON_PORT,
        .remote_key = "client"
    };

    comm_manager_->add_connection(client_config.remote_key,client_config);
    comm_manager_->add_connection(fusion_config.remote_key,fusion_config);
    
}

void Daemon::start(){

   comm_manager_->start_all_threads();
}


void Daemon::mimic_message(){
   std::string key="fusion";
   // while(1){
   //    std::this_thread::sleep_for(std::chrono::seconds(1));
   //    LOG("csi recieved on Bots");
   //    comm_manager_->insert_data("csi recieved on Bots",key);
   // }
   std::this_thread::sleep_for(std::chrono::seconds(1));
   LOG("csi recieved on Bots");
   comm_manager_->insert_data("csi recieved on Bots",key);
   
}

Daemon::~Daemon()= default;