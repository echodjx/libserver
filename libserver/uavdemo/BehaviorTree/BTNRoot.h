#pragma once
#include "BTNode.h"


class BTNRoot : public BTNode{
private:

public:
    BTNRoot() = default;
    void Execute() override {std::cout<<"BTNParallel";};
};