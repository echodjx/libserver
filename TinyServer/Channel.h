#pragma once
#include "noncopyable.h"
#include <functional>
#include "Timestamp.h"
#include <memory>
class EvenLopp;//仅使用这个类
class Timestamp;

//Channel 通道，封装了sockefd和其感兴趣的event，如EPOLLIN、EPOKKOUT事件。
//还绑定了poller返回的具体事件

class Channel : noncopyable {
public:
    using EventCallback = std:: function<void()> ;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EvenLopp *Loop, int fd);
    ~Channel();
    //fd得到poller通知后，处理事件，调用相应的回调方法
    void handleEvent(Timestamp receiveTime);
private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EvenLopp *loop_; //事件循环
    const int fd_; //fd, Poller监听的对象
    int events_; //注册fd感兴趣的事件
    int revents_;// poller返回的具体发生的事件
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;
    //channel 通道中能够获知fd最终发生的具体的事件revents,它负责调用具体事件的回调操作
    ReadEventCallback  readCallback_;
    EventCallback  writeCallback_;
    EventCallback  closeCallback_;
    EventCallback  errorCllback_;
};