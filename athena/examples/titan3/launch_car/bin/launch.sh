#!/bin/sh

#构造退出文件
echo "#!/bin/sh" > ./dd.sh
echo "" >> ./dd.sh
echo "#杀掉某一个进程" >> ./dd.sh
echo "killp()" >> ./dd.sh
echo "{" >> ./dd.sh
echo "    PROCESS=\`ps -ef|grep \$1|grep -v grep|grep -v PPID|grep -v codeblocks|awk '{ print \$2}'\`" >> ./dd.sh
echo "    for i in \$PROCESS" >> ./dd.sh
echo "    do" >> ./dd.sh
echo '        echo "Kill the $1 process [ $i ]"' >> ./dd.sh
echo "        kill -9 \$i" >> ./dd.sh
echo "    done" >> ./dd.sh
echo "}" >> ./dd.sh
echo "" >> ./dd.sh

#构造退出+删除日志文件
echo "#!/bin/sh" > ./ds.sh
echo "./dd.sh" >> ./ds.sh
echo "if [ -x *.osm ];then" >> ./ds.sh
echo "  rm ./*.osm" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x *.log ];then" >> ./ds.sh
echo "  rm ./*.log" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -s ../log/ ];then" >> ./ds.sh
echo "  rm ../log/*" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./1/LonTestData.txt ];then" >> ./ds.sh
echo "  rm ./1/LonTestData.txt" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./2/LonTestData.txt ];then" >> ./ds.sh
echo "  rm ./2/LonTestData.txt" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./3/LonTestData.txt ];then" >> ./ds.sh
echo "  rm ./3/LonTestData.txt" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./1/lateral_control_log.txt ];then" >> ./ds.sh
echo "  rm ./1/lateral_control_log.txt" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./2/lateral_control_log.txt ];then" >> ./ds.sh
echo "  rm ./2/lateral_control_log.txt" >> ./ds.sh
echo "fi" >> ./ds.sh
echo "if [ -x ./3/lateral_control_log.txt ];then" >> ./ds.sh
echo "  rm ./3/lateral_control_log.txt" >> ./ds.sh
echo "fi" >> ./ds.sh

#拷贝某个网络进程，参数如："../src/obu/obu_planning/bin/Debug/obu_planning" "obu_planning_DF001"
copyp()
{
    if [ -f $1 ]; then
        cp $1 ./$2
    fi
    echo 'killp "'$2'"' >> ./dd.sh
    chmod +x ./$2

    if [ $3 = "gdb" ]; then
        #启动gdb执行的方法
        gnome-terminal -e 'bash -c "echo r|gdb ./'$2'; exec bash"'
    else
        #正常执行方法
        gnome-terminal -e 'bash -c "./'$2'; exec bash"'
    fi
    
    echo "start: "$2
}

#拷贝某个单车进程，参数如："1" "controller"
carp()
{
    if [ ! -d ./$1 ]; then
        mkdir ./$1
    fi
    if [ -f ./$2 ]; then
        cp ./$2 ./$1/$2
    fi
    if [ -f ./controller_value.cfg ]; then
        cp ./controller_value.cfg ./$1/controller_value.cfg
    fi
    if [ -f ./engine_map.txt ]; then
        cp ./engine_map.txt ./$1/engine_map.txt
    fi
    if [ -f ./looptest.txt ]; then
        cp ./looptest.txt ./$1/looptest.txt
    fi
    echo 'killp "'$2'"' >> ./dd.sh
    chmod +x ./$2

    #修改组播地址
    sed -i "s/OBU_URL = udpm:\/\/239.255.*?ttl=3/OBU_URL = udpm:\/\/239.255.76.2$1:762$1?ttl=3/g" ./$1/config/control.cfg
    sed -i "s/OBU_URL = udpm:\/\/239.255.*?ttl=3/OBU_URL = udpm:\/\/239.255.76.2$1:762$1?ttl=3/g" ./$1/planning_value.cfg
    sed -i "s/OBU_URL = udpm:\/\/239.255.*?ttl=3/OBU_URL = udpm:\/\/239.255.76.2$1:762$1?ttl=3/g" ./$1/motion_planning_value.cfg
    sed -i "s/OBU_URL = udpm:\/\/239.255.*?ttl=3/OBU_URL = udpm:\/\/239.255.76.2$1:762$1?ttl=3/g" ./$1/sensor.cfg
    
    #切换目录
    cd ./$1
    
    #正常执行方法
    gnome-terminal -e 'bash -c "./'$2'; exec bash"'
    
    #启动gdb执行的方法
    #gnome-terminal -e 'bash -c "echo r|gdb ./'$2'; exec bash"'
    
    #切换目录
    sleep 1
    cd ..

    echo "start: "$1"/"$2
}

#要替换为本机IP地址的csu、rsu的name
set_ip()
{
    csuname="csu"
    rsuname="rsu_1"
    IP=`ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|grep -v 10.8.0.|awk '{print $2}'|tr -d "addr:地址"`
    sed -i "s/csu name=\"$csuname\" ip=\".*\" port/csu name=\"$csuname\" ip=\"$IP\" port/g" ../conf/config.xml
    sed -i "s/rsu name=\"$rsuname\" ip=\".*\" port/rsu name=\"$rsuname\" ip=\"$IP\" port/g" ../conf/config.xml
    echo "set csu&rsu ip to: "$IP
}

#开始启动
echo ""
echo "start begin"

#步骤1：修改config.xml中的csu/rsu的ip为本机ip
set_ip

#步骤2：请编辑../conf/simulate.xml，选择启动哪些模块的仿真

#步骤3：选择启动哪些进程，至少启动csu_planning/rsu_planning和本车的obu_planning
copyp "../src/obu/obu_planning/bin/Debug/obu_planning" "obu_planning_60U5Z"
#copyp "../src/sim/sim_system/bin/Debug/sim_system" "sim_system"
copyp "../src/sim/sim_vui/bin/Debug/sim_vui" "sim_vui_DF001"


#步骤4：选择启动单车进程，第1个参数是车号，注意不能修改启动顺序
carp "1" "control"
carp "1" "sensor"
carp "1" "planning_view"
carp "1" "planning"
#carp "1" "NXDMonitor"
#
#carp "2" "controller"
#carp "2" "sensor"
#carp "2" "motion"
#carp "2" "NXDMonitor"
#
#carp "3" "controller"
#carp "3" "sensor"
#carp "3" "motion"
#carp "3" "LcmMonitor"

echo "start ok!"

#步骤6：使用 ./dd.sh 杀掉所有启动的进程
chmod +x ./dd.sh
chmod +x ./ds.sh
echo "提示：可以执行 ./dd.sh 杀掉所有启动的进程"
echo "提示：可以执行 ./ds.sh 杀掉所有启动的进程，删除所有日志文件"
echo ""
