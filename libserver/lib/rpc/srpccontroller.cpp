//
// Created by echo-djx on 2021/11/1.
//

#include "srpccontroller.h"

SrpcController::SrpcController(){
  m_failed = false;
  m_errText = "";
}
void SrpcController::Reset(){
  m_failed = false;
  m_errText = "";
}
bool SrpcController::Failed() const{
    return m_failed;
}
std::string SrpcController::ErrorText() const{
  return m_errText;
}
void SrpcController::SetFailed(const std::string& reason){
  m_failed = true;
  m_errText = reason;
}
//need to finished
void SrpcController::StartCancal(){}
bool SrpcController::IsCanceled(){return false;}
void SrpcController::NotifyOnCancel(google::protobuf::Closure* callback){}