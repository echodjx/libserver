cmake_minimum_required(VERSION 3.16)
project(TinyServer)

#.so库 设置动态库的路径 放在根目录的lib文件夹下
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib)
#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(SOURCE_FILES EPollPoller.h)
aux_source_directory(. DIR_SRCS)
add_library(TinyServer SHARED  ${DIR_SRCS} )
