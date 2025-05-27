#include "threadManager.h"


ThreadManager::ThreadManager(int threadcount):thread_count_(threadcount){
   
    for(size_t i;i<thread_count_;i++){
        thread_list_.emplace_back(

            [this](){
                while (1)
                {  std::unique_lock<std::mutex> lock(threadlock);
                   cv.wait(lock,[this](){if(flag || !task_queue.empty()){return 1;}});

                   if(flag){
                    break;
                   }
                  
                   std::string message;
                   std::function<std::string(int)> task;
                   task=task_queue.front();
                   lock.unlock();
                   message=task(0);
                }
                
            }

        );
           
    }



}


void ThreadManager::insert_task(std::function<std::string(int)> task){
  
    std::unique_lock<std::mutex> lock(threadlock);
    task_queue.push(task);
    cv.notify_one();

}