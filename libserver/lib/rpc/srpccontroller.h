//
// Created by echo-djx on 2021/11/1.
//
#pragma once
#include <google/protobuf/service.h>
#include <string.h>


class SrpcController: public google::protobuf::RpcController{
public:
  SrpcController();
  void Reset();
  bool Failed() const;
  std::string ErrorText() const;
  void SetFailed(const std::string& reason);
  //need to finished
  void StartCancal();
  bool IsCanceled();
  void NotifyOnCancel(google::protobuf::Closure* callback);

private:
  bool m_failed;// RPC方法执行过程中的状态
  std::string m_errText; // RPC方法执行过程中的错误信息
};