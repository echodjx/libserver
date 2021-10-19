//
// Created by echo-djx on 2021/9/25.
//
#pragma once
#include <google/protobuf/service.h>


// 架构提供的专门服务发布rpc服务的网络对象类
class RpcProvider{
public:
    //框架提供给外部使用，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service){};

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run(){};
};
