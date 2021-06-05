cmake_minimum_required(VERSION 3.16)
project(TinyServer)

#.so库 设置动态库的路径 放在根目录的lib文件夹下
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib)
#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
aux_source_directory(. SRC_LIST)
add_library(TinyServer SHARED  ${SRC_LIST} noncopyable.h Logger.cpp Logger.h Timestamp.cpp Timestamp.h InetAddress.cpp InetAddress.h)