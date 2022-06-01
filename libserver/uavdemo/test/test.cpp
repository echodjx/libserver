#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include "../BehaviorTree/BTNode.h"
#include "../BehaviorTree/BTNSequence.h"
#include "../BehaviorTree/BTNSelector.h"
#include "../BehaviorTree/BTNRoot.h"
#include "../BehaviorTree/BTNParallel.h"
#include "../BehaviorTree/BTNAction.h"
using namespace std;

using json = nlohmann::json;
BTNode* CreateCNode(json j) {
    string name;
    string event;
    vector<BTNode*> CNode;
    BTNode* node;
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        auto au = it.value();
        if(it.key() == "name") name = it.value();
        if(it.key() == "Event") event = it.value();
        else if(it.key() == "CNode") {
            for(auto tmp : it.value()) {
                BTNode* cnode = CreateCNode(tmp);
                CNode.emplace_back(cnode);
            }
        }
    }
    if(name == "BTNRoot") node = new BTNRoot();
    else if(name == "BTNParallel") node = new BTNParallel();
    else if(name == "BTNSelector") node = new BTNSelector();
    else if(name == "BTNAction") node = new BTNAction();
    else if(name == "BTNSequence") node = new BTNSequence();
    node->SetName(name);
    for(auto &tmp : CNode) {
        node->AddCNode(tmp);
    }
    return node;
}

BTNode* ToTree (json j) {
    BTNode *root;
    root = CreateCNode(j);
    return root;
}

static void case0()
{
    json j = {
            {"age", 23},
            {"name", "tanxiaohai"},
            {"jobs", {"superhero", "neighborhood"}},
            {"gear", {"suits", "2099"}}
    };
    ifstream ifs;
    ifs.open("../JsonBTree/UavBTree.json");
    std::stringstream ss;
    if ( !ifs)
    {
        cout << "文件不能打开" <<endl;
    }
    else {
        string str;
        ifs>>ss.rdbuf();
       // cout<<ss<<endl;
    }
    ifs.close();
    json j2;
    ss>>j2;
    //cout<<j2.dump(2)<<endl;
    ToTree(j2);
}

int main()
{
    case0();

    return 0;
}