//
// Created by echo-djx on 2021/9/25.
//
#pragma once

//srpc框架的基础类
class SprcApplication{
public:
    static void Init(int argc,char **argv);
    static SprcApplication& GetInstance(){
        static  SprcApplication app;
        return app;
    }
private:
    SprcApplication(){}
    SprcApplication(const SprcApplication&){} = delete;
    SprcApplication(SprcApplication&&) = delete;
};