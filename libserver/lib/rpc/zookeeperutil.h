//
// Created by echo-djx on 2021/12/6.
//
#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class  ZkClient {
public:
    ZkClient();
    ~ZkClient();
    //zkclient启动连接zkserver
    void Start();
    //在zkserver上指定path创建znode节点
    void Create(const char *path, const char *data, int datalen, int state=0);
    std::string GetData(const char *path);

private:
    //zk客户端句柄
    zhandle_t *s_zhandle;
};