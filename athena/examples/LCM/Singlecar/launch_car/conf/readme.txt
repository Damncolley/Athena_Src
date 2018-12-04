1、config.xml
   配置了每个网元的名称和IP，当前版本网络IP在这里配死。

2、db/
   数据库文件，csu/rsu/obu/rsd/sim_system支持该内存数据库

3、task/
   脚本，可在oct执行脚本，OBU启动正常时运行"_obu_name.task"脚本，RSU启动正常时运行"_rsu_name.task"脚本

4、simulate/
   仿真脚本，sim_system加载这里的文件实现仿真
