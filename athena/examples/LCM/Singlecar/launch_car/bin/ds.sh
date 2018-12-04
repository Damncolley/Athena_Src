#!/bin/sh
./dd.sh
if [ -x *.osm ];then
  rm ./*.osm
fi
if [ -x *.log ];then
  rm ./*.log
fi
if [ -s ../log/ ];then
  rm ../log/*
fi
if [ -x ./1/LonTestData.txt ];then
  rm ./1/LonTestData.txt
fi
if [ -x ./2/LonTestData.txt ];then
  rm ./2/LonTestData.txt
fi
if [ -x ./3/LonTestData.txt ];then
  rm ./3/LonTestData.txt
fi
if [ -x ./1/lateral_control_log.txt ];then
  rm ./1/lateral_control_log.txt
fi
if [ -x ./2/lateral_control_log.txt ];then
  rm ./2/lateral_control_log.txt
fi
if [ -x ./3/lateral_control_log.txt ];then
  rm ./3/lateral_control_log.txt
fi
