#pragma once
#include "../BehaviorTree/BTNode.h"



class ExecuteEngine {
private:
    bool quit_;
    BTNode *root;
    std::string name_;
public:
    ExecuteEngine (BTNode *root, std::string name = "uav");
   // static void ExecuteBTree(BTNode *root,  bool &quit_);
    void StartExecute();
    void StopExecuteEngine();
};


