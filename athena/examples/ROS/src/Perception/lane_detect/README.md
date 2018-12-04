## lane_detect

### Introduction

这是一个用来检测车道线的ROS包，通过订阅图像消息进行车道线检测，发布车道线检测渲染的结果以及离散的车道线数据点。

订阅的消息如下：

- /usb_cam/image_raw     **[sensor_msgs/Image]**

发布的消息如下：

- /usb_cam/image_lane_detected   **[sensor_msgs/Image]**
- /laneDetectQuality  **[lane_detect/LaneDeectResult]**

发布的服务如下：

- laneDetect **[lane_detect/LaneDetector]**

---



### Build

#### Prerequisition

该软件包依赖**[ROS Kinetic](http://wiki.ros.org/kinetic/Installation/Ubuntu)**，**OpenCV 3.0**及以上 以及**[LCM](https://github.com/lcm-proj/lcm/releases/download/v1.4.0/lcm-1.4.0.zip)**

- ubuntu 16.04
- ROS kinetic
- OpenCV 3.0 
- lcm

#### compile

```shell
$cd your_catkin_ws
$catkin_make
```



---

### Usage

- 离线rosbag测试

  1. 下载[rosbag](www.baidu.com)
  2. 输入下面的命令

  ```shell
  $rosbag play XX.bag
  $roslaunch lane_detect lane_detect.launch
  ```

- 在线测试


---

### Acknologement

如果想了解更多信息，请访问[环宇智行](http://www.in-driving.com/)。![图片](C:\Users\kinggreat24\Desktop\assets\thumb.jpg)