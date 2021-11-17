//
// Created by echo-djx on 2021/11/17.
//
#pragma once
#include "lockqueue.h"

//异步日志

enum LogLevel{
    INFO, //普通信息
    ERROR, //错误信息
};

class AsyLogger{
public:
  static AsyLogger& GetInstance();//日志单例
    void SetLogLevel(LogLevel level);//设置日志级别
    void Log(std::string msg);//写日志
private:
    int s_loglevel; //记录日志级别
    LockQueue<std::string> s_lckQue;//日志缓冲队列
};

#define LOG_INFO(logmsgFormat, ...) \
    do \
    { \
        AsyLogger &logger = AsyLogger::instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.Log(buf); \
    } while({0});
#define LOG_INFO(logmsgFormat, ...) \
    do \
    { \
        AsyLogger &logger = AsyLogger::instance(); \
        logger.setLogLevel(ERROR); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.Log(buf); \
    } while({0});