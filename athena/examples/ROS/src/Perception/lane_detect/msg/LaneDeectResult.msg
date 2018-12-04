int32 NO_LANE_DETECTED = 0
int32 LEFT_LANE_DETECTED = 1
int32 RIGHT_LANE_DETECTED = 2
int32 BOTH_LANE_DETECTED = 3

std_msgs/Header header
int32 quality
lane_detect/LanePoint[] left_points
lane_detect/LanePoint[] right_points
