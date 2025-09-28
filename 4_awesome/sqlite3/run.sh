#!/bin/bash

# 定义你要执行的命令或程序
command="./demo"
count=1

# 使用 while 循环执行命令 100 次
while [ $count -le 100 ]
do
    #echo "第 $count 次执行命令"
    $command
    #count=$((count + 1))
    #sleep 1  # 等待 1 秒
done

