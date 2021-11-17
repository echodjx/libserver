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
    void push(const T &data);
    T& Pop();
private:
    std::queue<T> s_queue;
    std::mutex s_mutex;
    std::condition_variable s_condvariable;

};