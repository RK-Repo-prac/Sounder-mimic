#include "comm_base.h"

CommunicationManager::CommunicationManager() : running_(false) {
    socket_ = std::make_unique<communication>();
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
    LOG("Started listening on socket "<<connection_name);
    auto it = connections_.find(connection_name);
    if (it == connections_.end()) return;
    
    Connection& conn = *it->second;
    socket_->recv_msg(conn.config.local_key, conn.incoming_msgs, 
                     conn.incoming_mutex, conn.incoming_cv);

    std::string message_to_forward;
    
     while(1){
        {   LOG("Before Acquiring the Lock");
            std::unique_lock<std::mutex> inlock(conn.incoming_mutex);
            LOG("After Acquiring the Lock");
            if(it->first=="fusion"){
            LOG("The Size of the incomming queue is: "<<it->second->incoming_msgs.size());
        }
            conn.incoming_cv.wait_for(inlock, std::chrono::seconds(1), [&conn](){
                return !conn.incoming_msgs.empty();
            });
            
            if (!conn.incoming_msgs.empty()) {
                message_to_forward = conn.incoming_msgs.front();  // Simple string copy
                conn.incoming_msgs.pop();
            }
        } 
        
        if (!message_to_forward.empty()) {
            {
                std::lock_guard<std::mutex> outlock(conn.outgoing_mutex);
                conn.outgoing_msgs.push(message_to_forward);
                conn.outgoing_cv.notify_one();
            }
        }
     }
}

void CommunicationManager::send_thread_func(const std::string& connection_name) {
    LOG("Sending Message to: "<<connection_name);
    auto it = connections_.find(connection_name);
    if (it == connections_.end()) return;
    
    Connection& conn = *it->second;
    while (1) {
        std::unique_lock<std::mutex> lock(conn.outgoing_mutex);
        conn.outgoing_cv.wait_for(lock, std::chrono::seconds(1), [&conn]() {
            return !conn.outgoing_msgs.empty();
        });
        
        if (!conn.outgoing_msgs.empty()) {
            std::string msg = conn.outgoing_msgs.front();
            conn.outgoing_msgs.pop();
            lock.unlock();
            LOG("Sending Message");
            socket_->send_msg(msg, conn.config.local_key, conn.remote_addr);
        }
    }
}


void CommunicationManager::insert_data(std::string msg,std::string &key){
Connection* conn = nullptr;
auto it = connections_.find(key);
if (it != connections_.end()) {
    conn = it->second.get();}
   {
    std::unique_lock<std::mutex> ll(conn->incoming_mutex);
    conn->incoming_msgs.push(msg);

   }

 conn->incoming_cv.notify_one();  
   
}