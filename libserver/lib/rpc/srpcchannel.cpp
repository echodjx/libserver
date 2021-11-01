//
// Created by echo-djx on 2021/10/23.
//

#include "srpcchannel.h"
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "srpcapplication.h"
#include "srpccontroller.h"
void SrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                              google::protobuf::RpcController* controller,
                              const google::protobuf::Message* request,
                              google::protobuf::Message* response,
                              google::protobuf:: Closure* done){

  const google::protobuf::ServiceDescriptor*sd = method->service();
  std::string service_name = sd->name(); //service_name
  std::string method_name = method->name();//method_name

  //获取参数的序列化字符串长度 args_size
  uint32_t args_size = 0;
  std::string  args_str;
  if (request->SerializeToString(&args_str)){
    args_size = args_str.size();
  }
  else{
    controller->SetFailed("serialize request error");
    return;
  }
  //定义rpc的请求header
  sprpc::RpcHeader rpcHeader;
  rpcHeader.set_service_name(service_name);
  rpcHeader.set_method_name(method_name);
  rpcHeader.set_args_size(args_size);

  uint32_t header_size = 0;
  std::string rpc_header_str;
  if(rpcHeader.SerializeToString(&rpc_header_str)){
    header_size = rpc_header_str.size();
  }
  else{
    controller->SetFailed("serialize rpc header error!");
    return;
  }

  // 组织待发送的rpc请求的字符串
  std::string  send_rpc_str;
  send_rpc_str.insert(0, std::string((char*)&header_size, 4));//header_size
  send_rpc_str += rpc_header_str;
  send_rpc_str += args_str;

  //打印调试信息
  std::cout<<"============================================"<< std::endl;
  std::cout<<"header_size: "<< header_size <<std::endl;
  std::cout<<"rpc_header_str: "<< rpc_header_str <<std::endl;
  std::cout<<"service_name"<< service_name<<std::endl;
  std::cout<<"method_name"<< method_name<<std::endl;
  std::cout<<"args_str:" << args_str <<std::endl;
  std::cout<<"============================================"<< std::endl;

  //使用tcp编程，完成rpc方法的远程调用
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if(-1 == clientfd){
    std::cout<< "create socket errno:"<< errno <<std::endl;
    exit(EXIT_FAILURE);
  }

  std::string ip = SrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
  uint16_t port = atoi(SrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  //连接rpc服务节点
  if(-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof (server_addr))){
    char errtxt[512] = {0};
    sprintf(errtxt, "connect error! errno:%d",errno);
    controller->SetFailed(errtxt);
    close(clientfd);
    return;
  }

  //发送rpc请求
  if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(),0)){
    char errtxt[512] = {0};
    sprintf(errtxt, "send error! errno: %d",errno);
    controller->SetFailed(errtxt);
    return;
  }

  //接收rpc请求的响应值
  char recv_buf[1024] = {0};
  int recv_size = 0;
  if(-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0))){
    char errtxt[512] = {0};
    sprintf(errtxt, "recv error! errno: %d",errno);
    controller->SetFailed(errtxt);
    close(clientfd);
    return;
  }

  //反序列化rpc调用的响应数据
//  std::string response_str(recv_buf, 0, recv_size);
  if(!response->ParseFromArray(recv_buf, recv_size)){
    char errtxt[512] = {0};
    sprintf(errtxt, "parse error! response_str: %s",recv_buf);
    controller->SetFailed(errtxt);
    close(clientfd);
    return;
  }
  close(clientfd);
}