//
// Created by echo-djx on 2021/10/23.
//
#include <iostream>
#include <srpc/srpcapplication.h>
#include "../friend.pb.h"
#include <srpc/srpcchannel.h>

int main(int argc, char **argv){
  //整个程序启动后，想使用rpc框架，使用rpc服务，需要先调用框架的初始化函数（只初始化一次）
  SrpcApplication::Init(argc, argv);

  //调用远程发布的rpc方法login
  fixbug::FriendServiceRpc_Stub stub(new SrpcChannel());
  fixbug::GetFriendsListRequest request;
  request.set_userid(1000);

  fixbug::GetFriendsListResponse response;
  //发起rpc方法的调用 同步的rpc调用过程
  stub.GetFriendsList(nullptr,&request,&response, nullptr);//RpcChannel->RpcChannel::callMethmod 集中来做所有rpc方法调用的参数序列化和网络发送

  //一次rpc调用完成
  if (0 == response.result().errcode()){
    std::cout<<" rpc GetFriendsList response success:"<<std::endl;
    int size = response.friends_size();
    for(int i=0; i <size; ++i){
      std::cout<<"index:" <<(i+1)<<"name:"<<response.friends(i)<<std::endl;
    }
  }
  else{
    std::cout<<" rpc GetFriendsList error:"<< response.result().errcode() <<std::endl;
  }


  return 0;
}