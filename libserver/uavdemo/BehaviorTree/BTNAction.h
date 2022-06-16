#pragma once
#include "BTNode.h"



class BTNAction :  public BTNode{
private:
    enum ExecCode {
        kNotStarted,//未开始
        kFailed, //失败
        kSucceed,//成功
    };

public:
    BTNAction(std::string name = "");
    void Execute() override {std::cout<<"BTNAction"<<std::endl;};
};
