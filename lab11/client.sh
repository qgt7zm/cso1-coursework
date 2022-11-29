#!/bin/bash
# Run the client application
# The IP for portal05.cs.virginia.edu is 128.143.69.115
clang client.c -o client
./client "128.143.69.115" $1
# ./client $1 $2
