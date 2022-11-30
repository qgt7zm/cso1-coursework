#!/bin/bash
# Run the client application
clang client.c -o client
# ./client "128.143.69.115" $1
./client $1 $2
