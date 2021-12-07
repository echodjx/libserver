//
// Created by echo-djx on 2021/12/6.
//
#include "zookeeperutil.h"
#include "srpcapplication.h"
#include <iostream>

// 全局的watcher观察器   zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path, void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT)  // 回调的消息类型是和会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE)  // zkclient和zkserver连接成功
        {
            sem_t *sem = (sem_t *) zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : s_zhandle(nullptr) {
}

ZkClient::~ZkClient() {
    if (s_zhandle != nullptr) {
        zookeeper_close(s_zhandle); // 关闭句柄，释放资源  MySQL_Conn
    }
}

// 连接zkserver
void ZkClient::Start() {
    std::string host = SrpcApplication::GetInstance().GetConfig().Load("zookeeperip").c_str();
    std::string port = SrpcApplication::GetInstance().GetConfig().Load("zookeeperport").c_str();
    std::string conf = host + ":" + port;


    /*
	zookeeper的API客户端程序提供了三个线程
	API调用线程
	网络I/O线程  pthread_create  poll
	watcher回调线程 pthread_create
	*/
    s_zhandle = zookeeper_init(conf.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == s_zhandle) {
        std::cout << "zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(s_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state) {
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    // 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
    flag = zoo_exists(s_zhandle, path, 0, nullptr);
    if (ZNONODE == flag) // 表示path的znode节点不存在
    {
        // 创建指定path的znode节点了
        flag = zoo_create(s_zhandle, path, data, datalen,
                          &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if (flag == ZOK) {
            std::cout << "znode create success... path:" << path << std::endl;
        } else {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char *path) {
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(s_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK) {
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    } else {
        return buffer;
    }
}