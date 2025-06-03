#include "manager.h"

Manager::Manager(std::string routing){
 routing_=routing;
}

bool Manager::read_config(){
  
}
Manager::~Manager(){
    std::cout<<"Destructor Invoked"<<std::endl;
}

void Manager::start_threads(){
  

}

void Manager::relayThread(){
  if(routing_.length() == 0){
      std::cout<<"Empty Routing String"<<std::endl;
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
        std::cout<<"Successfully Parsed The Routing String"<<std::endl;
        std::shared_ptr<Relay> relay=std::make_shared<Relay>();
        relay->relay_Threads();
      }
  else{
    std::cout<<"Invalid Routing String"<<std::endl;
  }
}
}


void Manager::clientThread(){
 
}