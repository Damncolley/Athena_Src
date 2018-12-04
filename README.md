#### ATHENA_20181204
#### 版本更新说明
1. obu更新：a.解决初始切片下发两次问题；b.重新规划失败之后obu段错误。
2. planning更新：a.不同速度，规划长度的参数不同，并写入配置文件;b.更改ACC算法。

#### ATHENA_20181123
#### 版本更新说明
1. control更新：解决prescan停车刹不住的情况。
2. /athena/examples/LCM/Singlecar/下加入重卡仿真环境launch_truck（龙灵山地图和key.route)。
3. /athena/examples/LCM/Singlecar/launch_truck/bin/下加入sim_system，方便重卡的仿真测试。
4. planning更新：重新规划目的地后，planning给control下发退出自动驾驶的bug修复。

#### ATHENA_20181121
#### 版本更新说明
1. control更新(解决偶尔出现停车刹不住的情况）。
2. /athena/examples/LCM/APPS/下加入vui的apk安装包。
3. /athena/docs/README.md里新增软件框架图和推荐安装的第三方库的版本。

#### ATHENA_20181119
#### 版本更新说明
1. 地图nad.osm更新。
2. control更新（使用之前适用于unity仿真的版本）。
3. /athena/examples/titan3/launch/路径下增加arm版sim_vui。
4. 更新planning（a.从惯导读取车速信息而不是从can读取，并写作配置文件; b.最大加速度改为0.5m/s2; c.设置最大规划长度，并写作配置文件; d.配置文件增加对障碍物的log开关）。

#### ATHENA_20181118
#### 版本更新说明
1. /athena/core/arm/路径下增加各模块的arm库。
2. /athena/examples/titan3/launch/路径下增加适配titan3的athena例子。
3. 增加/athena/install_arm.sh脚本，用于titan3或ARM核下的动态库安装；并将之前x86动态库安装脚本名称更改为install_x86.sh。
4. 更新控制模块的库和例子。
5. 加入/athena/docs/README.md说明文档。

#### ATHENA_20181108
#### 版本更新说明
1. 更新头文件，路径： "/athena/core/x86/模块名称/include/"。
2. 更新库文件，路径： "/athena/core/x86/模块名称/lib/"。
3. 更新完头文件和库文件后，运行脚本 /athena/install.sh，将各模块动态库文件拷贝到系统目录下"/usr/local/lib/"。
4. 更新examples（目前为基于LCM的单车版本），在"/athena/examples/LCM/Singlecar/模块名称/"目录下建立各自模块的工程，编写实例调用动态库（注：链接的动态库文件为"/usr/local/lib/***.so"，查找的头文件路径为"../../../../core/x86/模块名称/include/"）。
5. 编译生成可执行文件，和配置文件一起拷贝到"/athena/examples/LCM/Singlecar/launch/"目录下相应的位置。


#### 版本负责人
章品

#### 模块负责人

1. 网络模块：王翠
2. 规划模块：枚元元
3. 控制模块：王知权
4. 感知模块：

