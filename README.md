# libserver

- 参考陈硕《linux多线程服务器编程》 muduo c++网络库《linux高性能服务器编程》、grpc、brpc
- 目前完成TcpConnection、HttpConnection外部编程接口及example
- 实现rpc（同步远程过程调用）及example（Protobuf数据序列化与反序列化，ZooKeeper分布式一致性协调服务应用）
- demo：无人机集群协同控制（行为树执行逻辑、rpc控制节点、共享黑板协同控制）

## 目前开发与实验机器

- ubuntu20
- gcc version 9.3.0
- cmake version 3.16.3

## 编译库

- lib目录下  chmod +x  autobuild.sh 
- ./autobuild.sh 

## 测试用例

### TcpConnection测试

example/TcpConnection/目录下  

- 开启终端
  - make
  - ./tcpserver_test
- 开启另一个终端
  - telnet 127.0.0.1 8000
  - 输入一串字符  完成echo测试

### MyblogServer测试

example/MyblogServer/ 目录下

- 安装git
  - centos：yum install git
  - ubuntu：apt-get install git

- 当前目录下

  - chmod +x myblogtest

  - ./myblogtest

- 打开浏览器

  - 输入 localhost:8888    // localhost可改为服务器IP

  - 点击、浏览网页

演示如下：

![myblog](images/myblog.gif)

### rpc测试

**环境安装**

- protobuf
  - git clone https://github.com/google/protobuf
  - unzip  protobuf-main.zip
  - cd protobuf-main
  - sudo apt-get install autoconf automake libtool curl 
  - ./autogen.sh
  - ./configure
  - make
  - sudo make install
  - sudo ldconfig

- 安装JDK 略
- zookeeper
  - https://archive.apache.org/dist/zookeeper/ 下载 zookeeper-3.4.10.tar.gz
  - tar -zxvf zookeeper-3.4.10.tar.gz
  - cd zookeeper-3.4.10
  - cd conf/
  - mv zoo_sample.cfg zoo.cfg
  - cd ../bin
  - ./zkServer.sh start
  - cd src/c
  - sudo ./configure
  - sudo make
  - sudo make install

