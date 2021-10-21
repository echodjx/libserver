//
// Created by echo-djx on 2021/9/27.
//
#include "rpcprovider.h"
#include "srpcapplication.h"
#include <functional>
void RpcProvider::Run() {
  std::string ip = SrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
  uint16_t port = atoi(SrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
  InetAddress address(port,ip);
  //创建tcpserver对象
  TcpServer server(&s_eventLoop, address, "RpcProvider");
  //绑定连接回调和消息读写回调方法
  server.setConnectionCallback(
      std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
  server.setMessageCallback(
      std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
  //设置线程数量
  server.setThreadNum(4);

  //启动网络服务
  server.start();
  s_eventLoop.loop();
}

void RpcProvider::NotifyService(google::protobuf::Service *service) {}

void RpcProvider::OnConnection(const TcpConnectionPtr &conn) {
  if (!conn->connected()) {
    // 和rpc client的连接断开了
    conn->shutdown();
  }
}

void RpcProvider::OnMessage(const TcpConnectionPtr &conn, Buffer *buffer,Timestamp st) {}