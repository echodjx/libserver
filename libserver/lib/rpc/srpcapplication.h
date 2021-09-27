//
// Created by echo-djx on 2021/9/25.
//
#pragma once

//srpc框架的基础类
class SrpcApplication{
public:
    static void Init(int argc,char **argv);
    static SrpcApplication& GetInstance();
private:
    SrpcApplication(){}
    SrpcApplication(const SrpcApplication&) = delete;
    SrpcApplication(SrpcApplication&&) = delete;
};