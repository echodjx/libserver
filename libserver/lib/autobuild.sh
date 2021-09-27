#!/bin/bash

set -e

# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/TinyServer  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/TcpConnection ]; then
    mkdir /usr/include/TcpConnection
fi

for header in `ls *.h`
do
    cp $header /usr/include/TcpConnection
done

cp `pwd`/lib/libTcpConnection.so /usr/lib

ldconfig
#进入http文件夹
cd http
# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/TinyServer  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/HttpConnection ]; then
    mkdir /usr/include/HttpConnection
fi

for header in `ls *.h`
do
    cp $header /usr/include/HttpConnection
done

cp `pwd`/lib/libHttpConnection.so /usr/lib

ldconfig
