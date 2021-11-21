//
// Created by echo-djx on 2021/11/17.
//
#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
// 异步写日志的队列
template<typename T>
class LockQueue{
public:
    //多个woker线程都会写日志queue
    void push(const T &data){
        std::lock_guard<std::mutex> lock(s_mutex);
        s_queue.push(data);
        s_condvariable.notify_one();
    }
    T Pop(){
        std::unique_lock<std::mutex> lock(s_mutex);
        while (s_queue.empty()){
            //当队列为空 ，线程等待
            s_condvariable.wait(lock);
        }
        T data = s_queue.front();
        s_queue.pop();
        return data;
    }
private:
    std::queue<T> s_queue;
    std::mutex s_mutex;
    std::condition_variable s_condvariable;
};