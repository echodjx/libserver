#pragma once
#include "noncopyable.h"
#include <string>
#include <iostream>
// LOG_INFO("%s %d", arg1,arg2)
#define LOG_INFO(logmsgFormat, ...) \
    do \
    { \
        Loffer &logger =Logger::instance(); \
        logger.setLogLevel(INFO);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat , ##__VA_ARGS__); \
        logger.log(buf);\
    }while(0)
#define LOG_ERROPR(logmsgFormat, ...) \
    do \
    { \
        Loffer &logger =Logger::instance(); \
        logger.setLogLevel(ERROPR);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat , ##__VA_ARGS__); \
        logger.log(buf);\
    }while(0)
#define LOG_FATAL(logmsgFormat, ...) \
    do \
    { \
        Loffer &logger =Logger::instance(); \
        logger.setLogLevel(FATAL);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat , ##__VA_ARGS__); \
        logger.log(buf);\
    }while(0)
//暂时不打印DEBUG的信息
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...) \
    do \
    { \
        Loffer &logger =Logger::instance(); \
        logger.setLogLevel(DEBUG);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat , ##__VA_ARGS__); \
        logger.log(buf);\
    }while(0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif
//定义日志的级别 INFO ERROPR FATAL DEBUG
enum LoLevel {
    INFO, //普通信息
    ERROR, //错误信息
    FATAL, //core信息
    DEBUG, //调试信息
};

// 输出一个日志类
class Logger :noncopyable{
public:
    //获取日志唯一的实例对象
    static Logger& instance();
    //设置日志级别
    void  setLogLevel(int level);
    //写日志
    void log(std::string msg);
private:
    int logLevel_;
    Logger(){}
};