#include<stdio.h>
#include<iostream>
#include "manager.h"
#include "config.h"
#include<memory>
#include "util.h" 
std::mutex Logmutex;

int main(){
std::unique_ptr<Manager> manager_= std::make_unique<Manager>(ROUTING,6);

while(!manager_->Exit_Status()){
   continue;
}

return 0;

}