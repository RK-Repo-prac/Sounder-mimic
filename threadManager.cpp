#include "threadManager.h"


ThreadManager::ThreadManager(int threadcount):thread_count_(threadcount){
    LOG("ThreadManager Constructor Started");
    LOG("Thread Count: "<<thread_count_);
    for(size_t i=0;i<thread_count_;i++){
        LOG("Launching Thread :"<<i);
        thread_list_.emplace_back(

            [this](){
                while (1)
                {  
                    
                std::function<void()> task;
                   {std::unique_lock<std::mutex> lock(threadlock);
                   cv.wait(lock,[this](){return flag || !task_queue.empty();});

                   if(flag){
                    break;
                   }
                  
                   std::string message;
                   task=task_queue.front();
                   task_queue.pop();
                   LOG("Starting the thread");
                }
                   task();
                }
                
            }

        );
           
    }



}


void ThreadManager::insert_task(std::function<void()> task){
    LOG("ThreadManager got a task");
    std::unique_lock<std::mutex> lock(threadlock);
    task_queue.push(task);
    cv.notify_one();

}

void ThreadManager::set_exit_status_(){
    { std::unique_lock<std::mutex> lock(threadlock);
        flag=true;
        lock.unlock();
    }
}

ThreadManager::~ThreadManager(){

    { std::unique_lock<std::mutex> lock(threadlock);
        flag=true;
        lock.unlock();
    }
    for (std::thread &worker : thread_list_) {
        if(worker.joinable()){
            worker.join();
        }
    }
}