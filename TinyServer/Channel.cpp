#include "Channel.h"
#include <sys/epoll.h>
#include "Logger.h"
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EvenLopp *loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false) {
}

Channel::~Channel() {
}

void Channel::tie(const  std::shared_ptr<void> &obj) {
    tie_ = obj;
    tied_ = true;
}

//当改变channel所表示fd的events事件后，update负责在poller里面更改fd相应的事件epoll_ctl
//EventLoop => ChannelList Poller
void Channel::update() {
    //通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    // add code...
    // loop_->updateChannel(this);
}

//在Channel所属的EventLoop中，把当前的channel删除掉
void Channel::remove() {
    //add code
    //loop->removeChannel(this)
}

//fd得到poller通知后，处理事件
void Channel::handleEvent(Timestamp receiveTime) {
    if (tied_) {
        std::shared_ptr<void> guard = tie_.lock();
        if(guard) {
            handleEventWithGuard(receiveTime);
        }
    }
}
// 根据poller通知的channel发生的具体事件,由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime) {
    LOG_INFO("channel handleEvent revents:%d\n", revents_);
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }

    if(revents_ & EPOLLERR) {
        if(errorCllback_) {
            errorCllback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI)) {
         if (readCallback_) {
             readCallback_(receiveTime);
         }
    }
    if (revents_ & EPOLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}

