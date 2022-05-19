#pragma once
#include <vector>


class  BTNode {
private:
    int condition_;  //状态
    std::vector<BTNode> CNode_; //子节点
public:
    virtual void Execute() = 0; //执行
};