cmake_minimum_required(VERSION 3.16)
project(UavCooperation)


#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")
set(SOURCE_FILES EPollPoller.h)

find_package(nlohmann_json 3.10.5 REQUIRED)



#编译当前文件夹下的所有文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./BehaviorTree SRC_LIST)
aux_source_directory(./test SRC_LIST)
aux_source_directory(./BTreeExecuteEngine SRC_LIST)

set(CMAKE_CXX_STANDARD 11)



add_executable(UavCooperation ${SRC_LIST})

target_link_libraries(UavCooperation nlohmann_json::nlohmann_json hiredis)