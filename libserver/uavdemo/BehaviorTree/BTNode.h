#pragma once
#include <vector>


class  BTNode {
private:
    enum StatusCode {
        kUnknown,
        kRunning, //运行
        kBlocking,//阻塞
    };
    enum ExecCode {
        kNotStarted,//未开始
        kFailed, //失败
        kSucceed,//成功
    };
    std::vector<BTNode> CNode_; //子节点
public:
    BTNode ();
    virtual void Execute() = 0; //执行
};