//
// Created by echo-djx on 2021/9/25.
//

#include "srpcapplication.h"
#include <iostream>
#include <unistd.h>
SrpcConfig SrpcApplication::s_config;
void ShowArgsHelp(){
    std::cout<<"format: command -i <configfile>" <<std::endl;
}
void SrpcApplication::Init(int argc,char **argv){
    if (argc<2){
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int cc = 0;
    std::string config_file;
    while ((cc = getopt(argc, argv,"i:")) != -1){
        switch (cc) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                std::cout<<"invalid args!"<<std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                std::cout<<"need <config_file>"<<std::endl;
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    //开始加载配置文件 rpcserver_ip =  prcserver_port   zookeeper_ip= zookepper_port=
    s_config.LoadConfigFile(config_file.c_str());

    std::cout << "rpcserverip:" << s_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport:" << s_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip:" << s_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport:" << s_config.Load("zookeeperport") << std::endl;
}
SrpcApplication& SrpcApplication::GetInstance(){
    static  SrpcApplication app;
    return app;
}



