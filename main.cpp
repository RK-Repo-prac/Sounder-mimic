#include<stdio.h>
#include<iostream>
#include "manager.h"
#include "config.h"
#include<memory>

int main(){
std::unique_ptr<Manager> manager_= std::make_unique<Manager>(ROUTING);
if(!manager_->read_config()){
   manager_.reset(nullptr);
   return 1;
}

return 0;

}