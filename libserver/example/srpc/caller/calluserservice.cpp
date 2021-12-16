//
// Created by echo-djx on 2021/10/23.
//
#include <iostream>
#include <srpc/srpcapplication.h>
#include "../user.pb.h"
#include <srpc/srpcchannel.h>

int main(int argc, char **argv){
  //整个程序启动后，想使用rpc框架，使用rpc服务，需要先调用框架的初始化函数（只初始化一次）
  SrpcApplication::Init(argc, argv);

  //调用远程发布的rpc方法login
  fixbug::UserServiceRpc_Stub stub(new SrpcChannel());
  fixbug::LoginRequest request;
  request.set_name("duan jia xin");
  request.set_pwd("123456");

  fixbug::LoginResponse response;
  //发起rpc方法的调用 同步的rpc调用过程
  stub.Login(nullptr,&request,&response, nullptr);//RpcChannel->RpcChannel::callMethmod 集中来做所有rpc方法调用的参数序列化和网络发送

  //一次rpc调用完成
  if (0 == response.result().errcode()){
    std::cout<<"rpc login response success:"<< response.sucess() <<std::endl;
  }
  else{
    std::cout<<"rpc login error:"<< response.result().errcode() <<std::endl;
  }

  //调用远程发布的rpc方法Register
  fixbug::RegisterRequest req;
  req.set_id(2000);
  req.set_name("srpc");
  req.set_pwd("123");
  fixbug::RegisterResponse rsp;

  stub.Register(nullptr, &req, &rsp, nullptr);
  //一次rpc调用完成
  if (0 == rsp.result().errcode()){
      std::cout<<"rpc register response success:"<< rsp.sucess() <<std::endl;
  }
  else{
      std::cout<<"rpc register error:"<< rsp.result().errcode() <<std::endl;
  }

  return 0;
}