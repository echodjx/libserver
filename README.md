# TinyServer

参考陈硕muduo C++网络库

部分使用c++11特性来实现

目前完成TcpConnection外部编程接口

接下来准备实现http与rpc



目前开发与实验机器

- gcc version 9.3.0

- cmake version 3.16.3

编译库

- lib目录下  chmod +x  autobuild.sh 
- ./autobuild.sh 

测试用例

- example目录下  make
- ./testserver
- telnet 127.0.0.1 8000

