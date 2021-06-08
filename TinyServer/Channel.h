#pragma once
#include "noncopyable.h"
#include <functional>
#include "Timestamp.h"
#include <memory>
class EvenLopp;//仅使用这个类
class Timestamp;
//EventLoop\Channel\Poller之间关系， =Reacter模型上对应 Demultiplex
//Channel 通道，封装了sockefd和其感兴趣的event，如EPOLLIN、EPOKKOUT事件。
//还绑定了poller返回的具体事件

class Channel : noncopyable {
public:
    using EventCallback = std:: function<void()> ;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EvenLopp *loop, int fd);
    ~Channel();
    //fd得到poller通知后，处理事件，调用相应的回调方法
    void handleEvent(Timestamp receiveTime);

    //设置回调函数对象
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void settCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCllback_ = std::move(cb); }

    //防止当channel被手动remove掉，channel还在执行回调操作
    void tie(const std::shared_ptr<void>&);

    int fd() const { return  fd_; }
    int events() const { return  events_; }
    int set_revents(int revt) { revents_ =revt; }

    void enableReading() { events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }


    //返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReadingb() const { return events_ & kReadEvent; }

    int  index() { return index_; }
    void  set_index(int idx) { index_ = idx; }

    //one loop per thread;
    EvenLopp* ownerLoop() { return loop_; }
    void remove();
private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

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