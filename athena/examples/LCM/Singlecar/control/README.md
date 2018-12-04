# Athena软件开发套件Control 
## svn 操作说明
### 下载
svn update
### 上传
svn ci -m ""

#### 项目介绍
Athena软件套件控制层软件命名为control，与旧版本controller相比有以下区别：1 软件框架优化。control软件分为三层，应用层，逻辑层以及控制器，应用层主要功能包括openGL显示、消息收发，调试、日志；逻辑层主要是刹车、档位、油门、驾驶模式、EPB等的逻辑处理，控制器为横纵向控制算法。2 消息更改。接收规划层bcm消息更改，控制命令信息更改；3 注释符合doxygen规范；4 代码风格为Google style。

#### 软件架构
软件架构说明

### 部分关键宏定义
typedef enum{
    LCM = 1,  /**< LCM消息*/
}MessageType;
<<<<<<< HEAD

typedef enum{
    CS55 = 1,  /**< LCM消息*/
}VehicleType;

typedef enum{
    INVALID = 0,             /**< 无效*/
    HUMAN_DRIVING_MODE = 1,  /**< 人工驾驶*/
    AUTO_DRIVING_MODE = 3,   /**< 自动驾驶*/
}DrivingMode;

typedef enum{
    UNCONTROLLABLE       = 0,   /**< 不可控*/
    CONTROLLABLE         = 1,   /**< 可控但处于非控制状态*/
    CONTROLLED           = 2,   /**< 正在控制中*/
}DrivingModeFeedBack;

typedef enum{
    POSITION_P = 0,          /**< P档位*/
    POSITION_R = 1,          /**< R档位*/
    POSITION_N = 2,          /**< N档位*/
    POSITION_D = 3,          /**< D档位*/
}GearLevel;

typedef enum{
    POSITION_P_FEEDBACK         = 0,  /**< P档位反馈*/
    POSITION_R_FEEDBACK         = 1,  /**< R档位反馈*/
    POSITION_N_FEEDBACK         = 2,  /**< N档位反馈*/
    POSITION_D_FEEDBACK         = 3,  /**< D档位反馈*/
    POSITION_SHIFTING_FEEDBACK  = 4,  /**< 档位切换过程中*/
    POSITION_INVALID_FEEDBACK   = 5,  /**< 无效档位*/
}GearLevelFeedBack;

#### 更新日志
1.新建分支 athena_control_alex，后面的更改在该分支下更改
2.修改文件生成目录，将controller_lib动态链接库默认生成到config目录，对应修改control工程的库文件查找目录
3.将坐标转换功能提前到路径判断之前，方便在规划无路径下发时也能显示车辆状态
4.仿真测试OK，未实车测试


1.经历了实车测试，仅测试横向控制
2.修改配置，使在release编译时使用arm交叉编译
3.规划仅在启动自动驾驶时重规划，会发送启动指令，但退出自动驾驶不会重规划，不会发送退出指令
4.横向效果较差，需要参数优化

=======

typedef enum{
    CS55 = 1,  /**< LCM消息*/
}VehicleType;

typedef enum{
    INVALID = 0,             /**< 无效*/
    HUMAN_DRIVING_MODE = 1,  /**< 人工驾驶*/
    AUTO_DRIVING_MODE = 3,   /**< 自动驾驶*/
}DrivingMode;

typedef enum{
    UNCONTROLLABLE       = 0,   /**< 不可控*/
    CONTROLLABLE         = 1,   /**< 可控但处于非控制状态*/
    CONTROLLED           = 2,   /**< 正在控制中*/
}DrivingModeFeedBack;

typedef enum{
    POSITION_P = 0,          /**< P档位*/
    POSITION_R = 1,          /**< R档位*/
    POSITION_N = 2,          /**< N档位*/
    POSITION_D = 3,          /**< D档位*/
}GearLevel;

typedef enum{
    POSITION_P_FEEDBACK         = 0,  /**< P档位反馈*/
    POSITION_R_FEEDBACK         = 1,  /**< R档位反馈*/
    POSITION_N_FEEDBACK         = 2,  /**< N档位反馈*/
    POSITION_D_FEEDBACK         = 3,  /**< D档位反馈*/
    POSITION_SHIFTING_FEEDBACK  = 4,  /**< 档位切换过程中*/
    POSITION_INVALID_FEEDBACK   = 5,  /**< 无效档位*/
}GearLevelFeedBack;

#### 更新日志
1.新建分支 athena_control_alex，后面的更改在该分支下更改
2.修改文件生成目录，将controller_lib动态链接库默认生成到config目录，对应修改control工程的库文件查找目录
3.将坐标转换功能提前到路径判断之前，方便在规划无路径下发时也能显示车辆状态
4.仿真测试OK，未实车测试


1.经历了实车测试，仅测试横向控制
2.修改配置，使在release编译时使用arm交叉编译
3.规划仅在启动自动驾驶时重规划，会发送启动指令，但退出自动驾驶不会重规划，不会发送退出指令
4.横向效果较差，需要参数优化
#### 更新日志  mwb

1、在controller_agent.cpp文件void ControllerAgent::PathConvert(path& path_msg,const Trajectory *trajectory_msg)函数中添加
	tar_speed赋值语句temp_point_.tar_speed_ = trajectory_msg ->points_[i].p_v_

2、在controller_agent.cpp文件 void ControllerAgent::GetControllerInfo(DebugOutput &debug_output)函数中添加 
	temp_point_.tar_speed_ = it->tar_speed_;

3、在control_logic.cpp文件void ControlLogic::GetControlLogicDebugOutput(ControlLogicDebugOutput &control_logic_debug_output)函数中
修改
	control_logic_debug_output_ = control_logic_debug_output;
为
	control_logic_debug_output = control_logic_debug_output_;

4、修改
 typedef enum{
	    UNCONTROLLABLE      = 0,   /**< 不可控*/
	    CONTROLLABLE         = 1,   /**< 可控但处于非控制状态*/
	    CONTROLLED           = 2,   /**< 正在控制中*/
	    
 }DrivingModeFeedBack;
为
 typedef enum{
	    CONTROLLABLE         = 1,   /**< 可控但处于非控制状态*/
	    CONTROLLED           = 2,   /**< 正在控制中*/
	    UNCONTROLLABLE      = 3,   /**< 不可控*/
}DrivingModeFeedBack;

5、修改打印不对应




>>>>>>> origin/athena_control_mwb
