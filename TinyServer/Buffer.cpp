//
// Created by echo-djx on 2021/6/21.
//

#include "Buffer.h"
#include <errno.h>
#include <sys/uio.h>
//从fd上读取数据 Poller从坐在LT模式
//Buffer缓冲区是有大小的，但是从fd上读数据的时候，不知道tcp数据最终大
ssize_t Buffer::readFd(int fd, int* saveErrno){
    char extrabuf[65536] = {0}; //栈上的内存空间
    struct iovec vec[2];
    const size_t writable = writableBytes();//Buffer底层缓冲区剩余的可写空间大小
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writable < sizeof extrabuf) ? 2:1 ;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n<0) {
        *saveErrno = errno;
    }
    else if ( n <= writable) {//buffer可写缓冲区已经够存储读取出来的数据了
        writerIndex_ +=n;
    }
    else { //extrabuf里面也写入了数据
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);//writeIndex_开始写n - writable大小的数据
    }
    return n;
}
