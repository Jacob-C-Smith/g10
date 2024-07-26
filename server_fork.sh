#!/bin/bash

# Print ifconfig info
/sbin/ifconfig eth0 | grep 'inet' | awk '{print $2}'

# Start
echo "[Docker] Starting g10 server on $(hostname)"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib/

cd build;ln -s ../lib lib

# Start g10 server
./g10_example 2>&1 

# Stop
echo "[Docker] g10 server stopped"