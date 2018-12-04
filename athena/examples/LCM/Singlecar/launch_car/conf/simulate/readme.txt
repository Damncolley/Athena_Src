simulate.xml格式：

1、可以模拟的对象：
   obu_list: 模拟自动驾驶车（带obu_planning的车）
   car_list: 模拟社会车辆
   rsd_list: 模拟RSD（路侧采集器）
   people_list: 模拟行人
   block_list: 模拟物理施工牌

2、配置参数：
   <obu name="鄂A DF001" route="key" start="2" speed="0" option="true_motion"/>
       route="key" : 采用同目录下key.route中定义的点
       start="2"   : 采用key.route第2行定义的点
       speed="0"   : 默认速度是0km/h，另，random(20)表示0-20随机选择一个速度
       option="true_motion" : 如果添加true_motion，表示带了motion和controller一起调试，否则sim_system模拟motion

3、地址文件：
   默认有一个key.route的地址文件，保存主要的地址
   可以在nad2.osm中编辑自己的地址或路径，用NadCoder.exe工具生成route文件
   