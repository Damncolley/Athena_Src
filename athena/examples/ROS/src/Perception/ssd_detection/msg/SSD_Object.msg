int32     id
int32     stability                       #目标稳定性 0;未知  1;稳定 2: 不稳定目标
int32     classification                  #行人/车辆/   car :7  person: 15  bus: 6
int32     detection_status                #目标检测跟踪状态 1；未跟踪  2: acc障碍物
float64  lat_pos                         #横向位置
float64  lat_rate                        #横向速度
float64  lon_pos                         #纵向位置
float64  lon_rate                        #纵向速度
float64  width                           #宽度，单位：m
float32   score                           #检测目标的置信度
ssd_detection/Rect obj_rect           #目标矩形框像素
uint32    real_data                       # 1  真实检测值 0  预测值
uint32    lifespan

