#pragma once
#include "BTNode.h"


class BTNRoot : public BTNode{
private:

public:
    BTNRoot(std::string name = "");
    void Execute() override {std::cout<<"Root"<<std::endl;};
};