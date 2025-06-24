#include "relay.h"

Relay::Relay() {
    comm_manager_ = std::make_unique<CommunicationManager>();
    ConnectionConfig ss_config = {
        .local_port = RELAY_SS_PORT,
        .local_key = "relay_ss",
        .remote_ip = "127.0.0.1",
        .remote_port = CLIENT_RELAY_PORT,
        .remote_key = "client"
    };
    comm_manager_->add_connection(ss_config.remote_key, ss_config);
    ConnectionConfig client_config = {
        .local_port = RELAY_CLIENT_PORT,
        .local_key = "relay_client", 
        .remote_ip = SS_IP_ADDR,
        .remote_port = SENSING_SERVER_PORT,
        .remote_key = "ss"
    };
    comm_manager_->add_connection(client_config.remote_key, client_config);

}


void Relay::start() {
    LOG("Starting Relay Threads");
    comm_manager_->start_all_threads();
}


Relay::~Relay() = default;