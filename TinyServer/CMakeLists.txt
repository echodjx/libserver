cmake_minimum_required(VERSION 3.16)
project(TinyServer)

#.so库 设置动态库的路径 放在根目录的lib文件夹下
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib)
#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")
set(SOURCE_FILES EPollPoller.h)

aux_source_directory(. SRC_LIST)
#add_library(TinyServer SHARED  Acceptor.cpp Acceptor.h Channel.h Channel.cpp  DefaultPoller.cpp EPollPoller.h EPollPoller.cpp EventLoop.h EventLoop.cpp InetAddress.h InetAddress.cpp Logger.cpp Logger.h noncopyable.h Poller.h Poller.cpp TcpServer.cpp TcpServer.h Thread.cpp Thread.h Timestamp.cpp Timestamp.h CurrentThread.cpp CurrentThread.h EventLoopThread.cpp EventLoopThread.h EventLoopThreadPool.cpp EventLoopThreadPool.h Socket.cpp Socket.h )

add_library(TinyServer SHARED ${SRC_LIST})