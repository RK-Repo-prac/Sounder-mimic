#include "manager.h"

Manager::Manager(std::string routing,int count){
 LOG("Manager Constructor Started");
 routing_=routing;
 thread_manager_=std::make_unique<ThreadManager>(count);
 start_threads();
}

Manager::~Manager(){
  LOG("Manager Destructor Invoked");
    thread_manager_->set_exit_status_();
}

void Manager::start_threads(){
  thread_manager_->insert_task([this](){this->relayThread();});
  thread_manager_->insert_task([this](){this->clientThread();});
  thread_manager_->insert_task([this](){this->FusionThread();});
  thread_manager_->insert_task([this](){this->DaemonThread();});

}

void Manager::relayThread(){
  if(routing_.length() == 0){
      LOG("Empty Routing String");
      exit_status=true;
  }
  else{
      std::vector<std::string> botlist;
      size_t position = routing_.find('=');
      if (position != std::string::npos) {
        std::string values = routing_.substr(position + 1);
        std::stringstream ss(values);
        std::string token;

        while (std::getline(ss, token, ',')) {
            botlist.emplace_back(token);
        }
        if(!botlist.size()==0){
        }
        LOG("Successfully Parsed The Routing String");
        {std::unique_lock<std::mutex> lock(routinglock_);
          routing_success_=true;
          routing_cv_.notify_all();
          lock.unlock();
        }
        std::unique_ptr<Relay> relay=std::make_unique<Relay>();
        relay->start();
      }
  else{
    LOG("Invalid Routing String");
    { std::lock_guard<std::mutex>lock(exitmutex);
   exit_status=true;
}
  }
}
}


void Manager::clientThread(){

  LOG("Lock acquired starting clientThread");

  { std::unique_lock<std::mutex> lock(routinglock_);
    routing_cv_.wait(lock,[this](){return routing_success_;});
    std::unique_ptr<Client> client=std::make_unique<Client>();
    client->start();
    lock.unlock();
  }
 
}


void Manager::FusionThread(){
  LOG("Lock acquired starting RelayThread");

  { std::unique_lock<std::mutex> lock(routinglock_);
    routing_cv_.wait(lock,[this](){return routing_success_;});
    std::unique_ptr<Fusion> fusion=std::make_unique<Fusion>();
    fusion->start();
    lock.unlock();
  }
 
}

void Manager::DaemonThread(){
  LOG("Lock acquired starting DaemonThread");
   std::unique_ptr<Daemon> daemon=std::make_unique<Daemon>();
  { std::unique_lock<std::mutex> lock(routinglock_);
    routing_cv_.wait(lock,[this](){return routing_success_;});
    daemon->start();
    LOG("Waiting for 10 seconds before starting sounding");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    LOG("Finished Waiting");
    lock.unlock();
  }
 daemon->mimic_message();
}

bool Manager::Exit_Status()
{ std::lock_guard<std::mutex>lock(exitmutex);
  return exit_status;
}