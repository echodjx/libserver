#!/bin/bash

set -e

if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

cd ..

# 把头文件拷贝到 /usr/include/TinyServer  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/srpc ]; then
    mkdir /usr/include/srpc
fi

for header in `ls *.h`
do
    cp $header /usr/include/srpc
done

cp `pwd`/lib/libsrpc.so /usr/lib

ldconfig

