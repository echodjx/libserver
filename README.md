# TinyServer

参考陈硕《linux多线程服务器编程》 muduo c++网络库

部分使用c++11特性来实现

目前完成TcpConnection、HttpConnection外部编程接口

接下来准备实现一个http服务器与rpc

## 目前开发与实验机器

- ubuntu20
- gcc version 9.3.0
- cmake version 3.16.3

## 编译库

- lib目录下  chmod +x  autobuild.sh 
- ./autobuild.sh 

## 测试用例

**TcpConnection测试**

example/TcpConnection/目录下  

- 开启终端
  - make
  - ./tcpserver_test
- 开启另一个终端
  - telnet 127.0.0.1 8000
  - 输入一串字符  完成echo测试

**HttpConnection测试**

example/HttpConnection/目录下  

- 开启终端
  - make
  - ./httpserver_test

- 打开浏览器
  - 输入http://localhost:8000/
  - 输入http://localhost:8000/hello
  - 输入http://localhost:8000/favicon.ico

