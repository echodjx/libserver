cmake_minimum_required(VERSION 3.16)
project(UavCooperation)


#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")
set(SOURCE_FILES EPollPoller.h)

#编译当前文件夹下的所有文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./BehaviorTree SRC_LIST)
set(CMAKE_CXX_STANDARD 11)

