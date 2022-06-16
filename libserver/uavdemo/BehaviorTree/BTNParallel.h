#pragma once
#include "BTNControl.h"

class BTNParallel :  public BTNControl {
private:

public:
    BTNParallel(std::string name = "");
    void Execute() override {std::cout<<"BTNParallel"<<std::endl;};
};
