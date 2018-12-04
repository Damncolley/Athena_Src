#!/bin/sh

#杀掉某一个进程
killp()
{
    PROCESS=`ps -ef|grep $1|grep -v grep|grep -v PPID|grep -v codeblocks|awk '{ print $2}'`
    for i in $PROCESS
    do
        echo "Kill the $1 process [ $i ]"
        kill -9 $i
    done
}

killp "obu_planning_60U5Z"
killp "sim_system"
killp "sim_vui_DF001"
killp "control"
killp "sensor"
killp "planning_view"
killp "planning"
