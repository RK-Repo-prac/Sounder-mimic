#include "comm_base.h"

CommunicationManager::CommunicationManager() : running_(false) {
    socket_ = std::make_shared<communication>();
}

CommunicationManager::~CommunicationManager() {
    running_ = false;
    for (auto& pair : connections_) {
        if (pair.second->recv_thread.joinable()) {
            pair.second->recv_thread.join();
        }
        if (pair.second->send_thread.joinable()) {
            pair.second->send_thread.join();
        }
    }
}

void CommunicationManager::add_connection(const std::string& name, const ConnectionConfig& config) {
    auto conn = std::make_unique<Connection>();
    conn->config = config;
    socket_->create_socket(config.local_port, config.local_key);
    setup_remote_address(*conn);
    
    connections_[name] = std::move(conn);
}

void CommunicationManager::setup_remote_address(Connection& conn) {
    memset(&conn.remote_addr, 0, sizeof(conn.remote_addr));
    conn.remote_addr.sin_family = AF_INET;
    conn.remote_addr.sin_port = htons(conn.config.remote_port);
    conn.remote_addr.sin_addr.s_addr = inet_addr(conn.config.remote_ip.c_str());
}

void CommunicationManager::start_all_threads() {
    running_ = true;
    
    for (auto& pair : connections_) {
        const std::string& name = pair.first;
        Connection& conn = *pair.second;
        conn.recv_thread = std::thread(&CommunicationManager::recv_thread_func, this, name);
        conn.send_thread = std::thread(&CommunicationManager::send_thread_func, this, name);
    }
}


void CommunicationManager::recv_thread_func(const std::string& connection_name) {
    auto it = connections_.find(connection_name);
    if (it == connections_.end()) return;
    
    Connection& conn = *it->second;
    socket_->recv_msg(conn.config.local_key, conn.incoming_msgs, 
                     conn.incoming_mutex, conn.incoming_cv);
}

void CommunicationManager::send_thread_func(const std::string& connection_name) {
    auto it = connections_.find(connection_name);
    if (it == connections_.end()) return;
    
    Connection& conn = *it->second;
    
    while (running_) {
        std::unique_lock<std::mutex> lock(conn.outgoing_mutex);
        conn.outgoing_cv.wait(lock, [&conn]() {
            return !conn.outgoing_msgs.empty();
        });
        
        if (!conn.outgoing_msgs.empty()) {
            std::string msg = conn.outgoing_msgs.front();
            conn.outgoing_msgs.pop();
            lock.unlock();
            socket_->send_msg(msg, conn.config.local_key, conn.remote_addr);
        }
    }
}