#pragma once

#include "Poller.h"
#include <vector>
#include <sys/epoll.h>

class EPollPoller : public Poller {
public:
private:
    using EventList = std::vector<epoll_event>;

    int epollfd_;
    EventList events_;
    
};