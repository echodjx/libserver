//
// Created by echo-djx on 2021/9/27.
//
#include "rpcprovider.h"
#include "rpcheader.pb.h"
void RpcProvider::Run() {
  std::string ip =
      SrpcApplication::GetInstance().GetConfig().Load("rpcserverip");
  uint16_t port = atoi(SrpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
  InetAddress address(port, ip);
  //创建tcpserver对象
  TcpServer server(&s_eventLoop, address, "RpcProvider");
  //绑定连接回调和消息读写回调方法
  server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
  server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
  //设置线程数量
  server.setThreadNum(4);

  //启动网络服务
  server.start();
  s_eventLoop.loop();
}
//框架提供给外部使用的，发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service) {
  //获取服务器对象的描述信息
  const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
  //获取服务的名字
  std::string service_name = pserviceDesc->name();
  //获取服务对象service的方法的数量
  int methodCnt = pserviceDesc->method_count();

  std::cout << "service_name:" << service_name << std::endl;

  ServiceInfo service_info;

  for(int i = 0;i < methodCnt; i++){
    //获取了服务对象指定下表的服务方法的描述（抽象描述）
    const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
    std::string method_name = pmethodDesc->name();
    service_info.s_methodMap.insert({method_name, pmethodDesc});

    std::cout << "method_name:" << method_name << std::endl;
  }
  service_info.s_service = service;
  s_serviceMap.insert({service_name,service_info});
}

void RpcProvider::OnConnection(const TcpConnectionPtr &conn) {
  if (!conn->connected()) {
    // 和rpc client的连接断开了
    conn->shutdown();
  }
}

//在框架内部 RpcProvider和Rpcconsumer协商好之间通信用的protobuf数据类型
//service_name method_name args 定义proto的message类型 进行数据的序列化与反序列化
//                              service_name method_name args_size
//header_size(4字节) + header_str + args_str

//已建立连接用户的读写事件回调 如果远程有一个rpc服务的调用请求 那么OnMessage方法就会响应
void RpcProvider::OnMessage(const TcpConnectionPtr &conn, Buffer *buffer,Timestamp st) {
  //网络上接收的远程rpc调用请求的字符流
  std::string recv_buf = buffer->retrieveAllAsString();

  //从字符流中读取前四个字节的内容
  uint32_t header_size = 0;
  recv_buf.copy((char*)&header_size, 4, 0);

  //根据header_size读取数据头的原始字符流
  std::string rpc_header_str = recv_buf.substr(4, header_size);
  sprpc::RpcHeader rpcHeader;
  std::string service_name;
  std::string method_name;
  uint32_t args_size;
  if(rpcHeader.ParseFromString(rpc_header_str)){
    //数据头反序列化成功
    service_name = rpcHeader.service_name();
    method_name = rpcHeader.method_name();
    args_size = rpcHeader.args_size();
  }
  else {
    //数据头反序列化失败
    std::cout << "rpc_header_str" << rpc_header_str << "rarse error!"<< std::endl;
    return;
  }
  //获取rpc方法参数的字符流数据
  std::string args_str = recv_buf.substr(4 + header_size, args_size);

  //打印调试信息
  std::cout<<"============================================"<< std::endl;
  std::cout<<"header_size: "<< header_size <<std::endl;
  std::cout<<"rpc_header_str: "<< rpc_header_str <<std::endl;
  std::cout<<"service_name"<< service_name<<std::endl;
  std::cout<<"method_name"<< method_name<<std::endl;
  std::cout<<"============================================"<< std::endl;

  //获取service对象和method对象
  auto it = s_serviceMap.find(service_name);
  if(it == s_serviceMap.end()){
    std::cout << service_name << "is not exist"<<std::endl;
    return;
  }

  auto mit = it->second.s_methodMap.find(method_name);
  if (mit == it->second.s_methodMap.end()){
    std::cout << service_name << "is not exist"<<std::endl;
    return;
  }

  google::protobuf::Service *service = it->second.s_service; //获取service对象
  const google::protobuf::MethodDescriptor *method = mit->second;//获取method对象

  //生成rpc方法调用的请求request和响应response参数
  google::protobuf::Message *request = service->GetRequestPrototype(method).New();
  if(!request->ParseFromString(args_str)){
    std::cout<< "request parse error!, content:"<< args_str << std::endl;
    return;
  }
  google::protobuf::Message *response = service->GetResponsePrototype(method).New();

  // 给method方法的调用，绑定closure的回调函数
  google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider,
                                     const TcpConnectionPtr&,google::protobuf::Message*>
                                     (this,
                                      &RpcProvider::SendRpcResponse,
                                      conn, response);

  //在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
  service->CallMethod(method, nullptr, request, response, done);

}


void RpcProvider::SendRpcResponse(const TcpConnectionPtr& conn, google::protobuf::Message* response){
  std::string response_str;
  if(response->SerializeToString(&response_str)){//response进行序列化
    //序列化成功后，通过网络吧rpc方法执行的结果发送给会rpc的调用方
    conn->send(response_str);
    conn->shutdown(); // rpcprovider主动断开连接
  }
  else{
    std::cout<< "serialize response_str error!"<< std::endl;
  }
  conn->shutdown();
}