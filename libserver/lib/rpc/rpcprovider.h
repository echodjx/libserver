//
// Created by echo-djx on 2021/9/25.
//
#pragma once
#include "srpcapplication.h"
#include <memory>
#include <TcpConnection/TcpServer.h>
#include <TcpConnection/TcpConnection.h>
#include <TcpConnection/EventLoop.h>
#include <TcpConnection/InetAddress.h>
#include <functional>
#include <unordered_map>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
// 架构提供的专门服务发布rpc服务的网络对象类
class RpcProvider{
public:
    //框架提供给外部使用，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    //EventLoop
    EventLoop s_eventLoop;

    //服务类型信息
    struct ServiceInfo{
      google::protobuf::Service *s_service;//保存服务对象
      std::unordered_map<std::string, const google::protobuf::MethodDescriptor*>s_methodMap;//保存服务方法
    };
    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> s_serviceMap;

    //socket连接回调
    void OnConnection(const TcpConnectionPtr&);

    //已建立连接用户的读写事件回调
    void OnMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    //Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const TcpConnectionPtr&, google::protobuf::Message*);
};
