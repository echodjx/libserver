#include "ExecuteEngine.h"
#include "TcpConnection/AsyLogger.h"
#include <thread>
#include <queue>
ExecuteEngine::ExecuteEngine(BTNode *root) : root(root) {

}



void ExecuteEngine::ExecuteBTree(BTNode *root) {
    if (root == nullptr) {
        //LOG_ERROR("empty BTree!");
    }
    std::queue<BTNode *> nodequ;

    std::cout<<"start thread"<<std::endl;

}


void ExecuteEngine::StartExecute() {
    std::thread thread_(ExecuteEngine::ExecuteBTree,root);
    thread_.join();
}