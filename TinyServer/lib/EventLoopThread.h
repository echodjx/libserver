//
// Created by echo-djx on 2021/6/11.
//
#pragma once
#include "noncopyable.h"
#include <functional>
#include "Thread.h"
#include <mutex>
#include <condition_variable>
#include "EventLoop.h"
#include <string>
#include <string.h>
class EventLoopThread : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const std::string &name = std::string());
    ~EventLoopThread();

    EventLoop* startLoop();
private:
    void threadFunc();

    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallback callback_;
};
