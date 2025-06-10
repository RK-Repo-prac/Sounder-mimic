#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include<thread>
#include<vector>
#include<queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include<string>
#include<iostream>
#include "util.h"

class ThreadManager{


public:
ThreadManager(int);
~ ThreadManager();
void insert_task(std::function<void()> task);
void set_exit_status_();

private:
int thread_count_;
bool flag{false};
std::vector<std::thread> thread_list_;
std::mutex threadlock;
std::condition_variable cv;
std::queue<std::function<void()>> task_queue;


};


#endif