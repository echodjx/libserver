//
// Created by echo-djx on 2021/9/25.
//

#include "srpcapplication.h"
#include <iostream>
#include <unistd.h>
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

}
SrpcApplication& SrpcApplication::GetInstance(){
    static  SrpcApplication app;
    return app;
}



