#!/bin/bash

set -e

make

if [ ! -d echodjx.github.io ]; then
git clone https://github.com/echodjx/echodjx.github.io.git
fi

cp  blogserver_test  echodjx.github.io/

cd echodjx.github.io/

./blogserver_test