//
// Created by echo-djx on 2021/6/15.
//
#include "Logger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "Acceptor.h"
#include <errno.h>
#include "InetAddress.h"
static int createNonblocking(){
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        LOG_FATAL("%s:%s:%d listen socket create err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
    }
    return sockfd;
}
Acceptor::Acceptor(EventLoop *loop, const  InetAddress &listenAddr, bool reuseport)
    : loop_(loop)
    , acceptSocket_(createNonblocking())
    , acceptChannel_(loop, acceptSocket_.fd())
    , listenning_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);
    //TcpServer::start() Acceptor.listen 有新用户连接，要执行一个回调（connfd=>channel =>subloop）
    //baseLoop=>acceptCannel_(listernfd)=>
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}
Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}


void Acceptor::listen() {
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();//acceptChannel_ => Poller
}

//listenfd有时间发生了,就是有新用户连接
void Acceptor::handleRead() {
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);//轮询找到subloop,唤醒，分发当前的Channel
        }
        else {
            ::close(connfd);
        }
    }
    else {
        LOG_FATAL("%s:%s:%d accept err:%d \n", __FILE__, __FUNCTION__, __LINE__ , errno);
        if (errno == EMFILE) {
            LOG_FATAL("%s:%s:%d accept err:%d \n", __FILE__, __FUNCTION__, __LINE__ , errno);
        }
    }
}