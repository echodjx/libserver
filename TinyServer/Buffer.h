//
// Created by echo-djx on 2021/6/21.
//
#pragma once
#include <vector>
#include <string>
//网络库底层的缓冲器类型定义
class Buffer {
public:
    static const size_t kCheapPrepend =8;
    static const size_t kInitiaklSize = 1024;

    explicit Buffer(size_t initialSize = kInitiaklSize)
        : buffer_(kCheapPrepend + initialSize)
        , readerIndex_(kCheapPrepend)
        , writerIndex_(kCheapPrepend )
    {}

    size_t readableBytes() const {
        return writerIndex_ - readerIndex_;
    }

    size_t writableBytes() const {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const {
        return readerIndex_;
    }
    //返回缓冲区中可读数据的起始地址
    const char * peek() {
        return begin() + readerIndex_;
    }

    //onMessage string <-Buffer
    void retrieve(size_t len) {
        if (len < readableBytes()) {
            readerIndex_ += len;//应用只读取了可读缓冲区数据的一部分，就是len，还剩下readerIndex_ += len - writerIndex
        }
        else { //len == readableBytes()
            retrieveAll();
        }
    }

    void retrieveAll() {
        readerIndex_ = writerIndex_ = kCheapPrepend;
    }
    //把哦那Message函数上报的Buffer数据，转成string类型的数据返回
    std::string retrieveAllAsString() {
        return retrieveAsString(readableBytes());//应用可读数据的长度
    }

    std::string retrieveAsString(size_t len) {
        std::string result(peek(), len);
        retrieve(len); //上面一句把缓冲区中可读的数据，已经读取出来，这里肯定要对缓冲区进行复位操作
        return result;
    }
private:
    char * begin()  {
        return &*buffer_.begin();//vector底层数据首元素的地址，也就是数组的起始地址
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};
