//
// Created by echo-djx on 2021/6/15.
#pragma once

#include <functional>
#include "Socket.h"
#include "Channel.h"
#include "noncopyable.h"
class EventLoop;
class InetAddress;

class Acceptor :noncopyable {
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
    Acceptor(EventLoop *loop, const  InetAddress &listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) {
        newConnectionCallback_ = cb;
    }
    bool listenning() const {return listenning_; }
    void listen();
private:
    void handleRead();

    EventLoop *loop_;//Acceptotr用的就是用户定义的baseLoop，也称作mainLoop
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
};