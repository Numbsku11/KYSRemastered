#!/bin/bash
cd "$(dirname "$0")"

cd .. 

g++ -g ./src/main.cpp -o ./bin/debug -I ./include/ -Werror -pedantic
