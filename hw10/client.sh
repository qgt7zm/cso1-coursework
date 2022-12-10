#!/bin/bash
# Run the client application
clang schat.c -o client
./client $1 $2
