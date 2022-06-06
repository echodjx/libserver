#pragma once
#include "../BehaviorTree/BTNode.h"




class ExecuteEngine {
private:
    BTNode *root;
public:
    ExecuteEngine (BTNode *root);
    void ExecuteBTree();

};


