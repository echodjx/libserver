#pragma once
#include "BTNControl.h"

class BTNParallel :  public BTNControl {
private:

public:
    BTNParallel() = default;
    void Execute() override {std::cout<<"BTNParallel";};
};
