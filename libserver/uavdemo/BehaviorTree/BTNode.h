#pragma once
#include <vector>
#include <string>
#include <iostream>
class  BTNode {
private:
    enum StatusCode { //状态码
        kUnStarted,//未开始
        kRunning, //运行中
        kBlocking,//阻塞
        kEnded,//结束
    };

    std::string name_;
public:
    std::vector<BTNode*> CNode_; //子节点
    BTNode (std::string name = "");
    virtual void Execute() = 0; //执行
    void SetName(std::string str) {name_ = str;}
    void AddCNode(BTNode *node) {
        CNode_.emplace_back(node);
    }
};