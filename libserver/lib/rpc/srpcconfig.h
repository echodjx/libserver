//
// Created by echo-djx on 2021/10/19.
//
#pragma once
#include <unordered_map>
//读取配置文件类
class  SrpcConfig{
public:
    //解析配置文件
    void LoadConfigFile(const char *config_file);
    //查询配置信息
    std::string Load(std::string key);
private:
    std::unordered_map<std::string,std::string> s_configMap;
    //去掉配置文件中一行前后空格
    void Trim(std::string &src_buf);
};