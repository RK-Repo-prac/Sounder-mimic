#include "manager.h"

Manager::Manager(std::string routing,int t_count):thread_count_(t_count){
 routing_=routing;
 relay=std::make_shared<Relay>();
 threadpool_=std::make_shared<ThreadManager>(thread_count_);
}

bool Manager::read_config(){
  if(routing_.length() == 0){
      std::cout<<"Empty Routing String"<<std::endl;
      return 0;
  }
  else{
      size_t position = routing_.find('=');
      if (position != std::string::npos) {
        std::string values = routing_.substr(position + 1);
        std::stringstream ss(values);
        std::string token;

        while (std::getline(ss, token, ',')) {
            botlist_.emplace_back(token);
        }
        if(!botlist_.size()==0){
            return 1;
        }
        std::cout<<"Successfully Parsed The Routing String"<<std::endl;
  }
  else{
    std::cout<<"Invalid Routing String"<<std::endl;
    return 0;
  }
}
}
Manager::~Manager(){
    std::cout<<"Destructor Invoked"<<std::endl;
}

void Manager::start_threads(){
  threadpool_->insert_task(relay->receive_msg());

}