#pragma once
#include "BTNControl.h"
#include "BTNAction.h"
class BTNSelector : public BTNControl{
private:
    BTNAction* Select(); //根据条件选择执行的节点

public:
    BTNSelector(std::string name = "");
    void Execute() override {std::cout<<"BTNSelector"<<std::endl;};
};