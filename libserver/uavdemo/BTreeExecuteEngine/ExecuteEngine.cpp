#include "ExecuteEngine.h"
#include "TcpConnection/AsyLogger.h"
#include <thread>
#include <queue>
#include <unistd.h>
ExecuteEngine::ExecuteEngine(BTNode *root, std::string name ) : quit_(false), root(root), name_(name){

}



void ExecuteBTree(BTNode *root, bool &quit_) {
    if (root == nullptr) {
        LOG_ERROR("empty BTree!,BTree did not generate");
        return;
    }
    std::queue<BTNode *> nodequ;
    nodequ.push(root);
    while(!quit_) {
        int x = nodequ.size();
        while(x--) {
            BTNode *node = nodequ.front();
            nodequ.pop();
            node->Execute();
            int num = node->CNode_.size();
            int idex = 0;
            while (idex < num) {
                nodequ.push(node->CNode_[idex]);
                idex++;
            }
        }
        sleep(1);
    }
    std::cout<<"stop thread"<<std::endl;
}

void ExecuteEngine::StartExecute() {
    std::cout<<&quit_<<std::endl;
    std::thread thread_(ExecuteBTree, root, std::ref(quit_));
    thread_.detach();
}

void ExecuteEngine::StopExecuteEngine() {
  //  std::cout<<quit_<<std::endl;
    quit_ = true;
    LOG_INFO("%s:BTree has stopped", name_.c_str());
}