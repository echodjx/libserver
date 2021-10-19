//
// Created by echo-djx on 2021/10/19.
//
#include "srpcconfig.h"
#include <iostream>
//解析配置文件
void SrpcConfig::LoadConfigFile(const char *config_file){
    FILE *fp = fopen(config_file,"r");
    if(nullptr == fp){
        std::cout<< config_file << "is not exist" <<std::endl;
        exit(EXIT_FAILURE);
    }

    //1.注释 2.正确的配置项  3.去掉开头多余空格
    while(!feof(fp)){
        char buf[1024] = {};
        fgets(buf, 1024 , fp);

        //去空格
        std::string read_buf(buf);
        Trim(read_buf);
        // 判断#注释
        if(read_buf[0] == '#' || read_buf.empty()) continue;
        // 解析配置项
        int idx = read_buf.find('=');
        if(idx == -1){
            //配置项不合法
            continue;
        }
        std::string key;
        std::string value;
        key = read_buf.substr(0,idx);
        Trim(key);
        // 去掉后面的换行等等
        int endidx = read_buf.find('\n', idx );
        value = read_buf.substr(idx+1, endidx-idx-1);
        s_configMap.insert({key,value});
    }
}
//查询配置信息
std::string SrpcConfig::Load(std::string key){
    auto it = s_configMap.find(key);
    if( it == s_configMap.end()){
        return  "";
    }
    return it->second;
}

void SrpcConfig::Trim(std::string &src_buf){
    //去掉字符串前面空格
    int idx = src_buf.find_first_not_of(' ');
    if(idx != -1){
        //前面有空格
        src_buf = src_buf.substr(idx, src_buf.size()-idx);
    }
    //去掉字符串后面空格
    idx = src_buf.find_last_not_of(' ');
    if(idx != -1){
        //后面有空格
        src_buf = src_buf.substr(0, idx+1);
    }
}