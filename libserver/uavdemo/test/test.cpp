#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>
#include "../BehaviorTree/BTNode.h"
#include "../BehaviorTree/BTNSequence.h"
#include "../BehaviorTree/BTNSelector.h"
#include "../BehaviorTree/BTNRoot.h"
#include "../BehaviorTree/BTNParallel.h"
#include "../BehaviorTree/BTNAction.h"
#include "../BTreeExecuteEngine/ExecuteEngine.h"
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

void doTest()
{
    //redis默认监听端口为6387 可以再配置文件中修改
    redisContext* c = redisConnect("127.0.0.1", 6379);
    if ( c->err)
    {
        redisFree(c);
        printf("Connect to redisServer faile\n");
        return ;
    }
    printf("Connect to redisServer Success\n");

    const char* command1 = "set stest1 value1";
    redisReply* r = (redisReply*)redisCommand(c, command1);

    if( NULL == r)
    {
        printf("Execut command1 failure\n");
        redisFree(c);
        return;
    }
    if( !(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK")==0))
    {
        printf("Failed to execute command[%s]\n",command1);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command1);

    const char* command2 = "strlen stest1";
    r = (redisReply*)redisCommand(c, command2);
    if ( r->type != REDIS_REPLY_INTEGER)
    {
        printf("Failed to execute command[%s]\n",command2);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);


    const char* command3 = "get stest1";
    r = (redisReply*)redisCommand(c, command3);
    if ( r->type != REDIS_REPLY_STRING)
    {
        printf("Failed to execute command[%s]\n",command3);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    printf("The value of 'stest1' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command3);

    const char* command4 = "get stest2";
    r = (redisReply*)redisCommand(c, command4);
    if ( r->type != REDIS_REPLY_NIL)
    {
        printf("Failed to execute command[%s]\n",command4);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command4);


    redisFree(c);

}

int main()
{
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

 //  case0();
    doTest();
    ExecuteEngine engine( ToTree(j2));
    engine.StartExecute();
    return 0;
}