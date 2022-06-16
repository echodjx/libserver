#pragma once
#include "BTNControl.h"


class BTNSequence : public  BTNControl {
public:
    BTNSequence(std::string name = "");
    void Execute() override {std::cout<<"BTNSequence"<<std::endl;};
};