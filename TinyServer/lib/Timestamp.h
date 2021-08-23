#pragma once

#include <iostream>
#include <string>

// 时间类
class Timestamp {
public:
    Timestamp();

    explicit Timestamp(int64_t microSecondsSinceEpoch);

    static Timestamp now();

    std::string toString() const;

    void swap(Timestamp &that) {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

private:
    int64_t microSecondsSinceEpoch_;
};