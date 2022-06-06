//
// Created by echo-djx on 2021/11/17.
//
#include "AsyLogger.h"
#include <time.h>
#include "Timestamp.h"
#include <iostream>
AsyLogger::AsyLogger(){
    //启动写日志线程
    std::thread writeLogTask([&](){
        for(;;){
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);
            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

            FILE *pf = fopen(file_name,"a+");
            if(pf == nullptr){
                std::cout << "logger file:"<<file_name << "open error"<< std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = s_lckQue.Pop();

            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d:",nowtm->tm_hour,nowtm->tm_min,nowtm->tm_sec);
            msg.insert(0,Timestamp::now().toString()+" ");
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    //设置分离线程（守护进程）
    writeLogTask.detach();
}
AsyLogger& AsyLogger::GetInstance(){
    static AsyLogger logger;
    return logger;
}
void AsyLogger::SetLogLevel(LogLevel level){
        s_loglevel = level;
}

//把日志信息写入lockqueue缓冲区当中
void AsyLogger::Log(std::string msg){
    s_lckQue.push(msg);
}