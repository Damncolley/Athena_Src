/*-------------------------------------------------------
 * 文件名：nad_msg.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：引用了msg目录下的所有头文件
-------------------------------------------------------*/


#include "nad_msg.h"

string log_rc_rsu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::rc_rsu_login_request     > *msg)
{
    char ret[256];
    sprintf(ret, "rc_rsu_login_request(%s->%s): rsu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str());
    return string(ret);
}

string log_cr_rsu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cr_rsu_login_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "cr_rsu_login_respond(%s->%s): rsu=%s, ret=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str(), ret_str(msg->body.retcode));
    return string(ret);
}

string log_cr_rsu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::cr_rsu_logout_notify     > *msg)
{
    char ret[256];
    sprintf(ret, "cr_rsu_logout_notify(%s->%s): rsu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str());
    return string(ret);
}

string log_oc_rsu_name_request(      const nad_lcm::ne_msg_t<nad_lcm::oc_rsu_name_request      > *msg)
{
    char ret[256];
    sprintf(ret, "oc_rsu_name_request(%s->%s): obu=%s, lon=%.6f, lat=%.6f",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.obu_lon, msg->body.obu_lat);
    return string(ret);
}

string log_co_rsu_name_respond(      const nad_lcm::ne_msg_t<nad_lcm::co_rsu_name_respond      > *msg)
{
    char ret[256];
    sprintf(ret, "co_rsu_name_respond(%s->%s): rsu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str());
    return string(ret);
}

string log_or_obu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::or_obu_login_request     > *msg)
{
    char ret[256];
    sprintf(ret, "or_obu_login_request(%s->%s): rsu=%s, obu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str(), msg->body.obu_name.c_str());
    return string(ret);
}

string log_rc_obu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::rc_obu_login_request     > *msg)
{
    char ret[256];
    sprintf(ret, "rc_obu_login_request(%s->%s): rsu=%s, obu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str(), msg->body.obu_name.c_str());
    return string(ret);
}

string log_cr_obu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cr_obu_login_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "cr_obu_login_respond(%s->%s): rsu=%s, obu=%s, ret=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str(), msg->body.obu_name.c_str(), ret_str(msg->body.retcode));
    return string(ret);
}

string log_ro_obu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::ro_obu_login_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "ro_obu_login_respond(%s->%s): rsu=%s, obu=%s, ret=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu_name.c_str(), msg->body.obu_name.c_str(), ret_str(msg->body.retcode));
    return string(ret);
}

string log_cr_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::cr_obu_logout_notify     > *msg)
{
    char ret[256];
    sprintf(ret, "cr_obu_logout_notify(%s->%s): obu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str());
    return string(ret);
}

string log_rc_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::rc_obu_logout_notify     > *msg)
{
    char ret[256];
    sprintf(ret, "rc_obu_logout_notify(%s->%s): obu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str());
    return string(ret);
}

string log_ro_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::ro_obu_logout_notify     > *msg)
{
    char ret[256];
    sprintf(ret, "ro_obu_logout_notify(%s->%s): obu=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str());
    return string(ret);
}

string log_uo_route_request(         const                   nad_lcm::uo_route_request           *msg)
{
    char ret[256];
    sprintf(ret, "uo_route_request: obu=%s, kp=%d, dest=%s",
        msg->obu_name.c_str(), msg->num_of_kp, msg->destination.c_str());
    return string(ret);
}

string log_or_route_request(         const nad_lcm::ne_msg_t<nad_lcm::or_route_request         > *msg)
{
    char ret[256];
    sprintf(ret, "or_route_request(%s->%s): obu=%s, kp=%d, dest=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.num_of_kp, msg->body.destination.c_str(), route_reason_str(msg->body.route_reason));
    return string(ret);
}

string log_rc_route_request(         const nad_lcm::ne_msg_t<nad_lcm::rc_route_request         > *msg)
{
    char ret[256];
    sprintf(ret, "rc_route_request(%s->%s): obu=%s, kp=%d, dest=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.num_of_kp, msg->body.destination.c_str(), route_reason_str(msg->body.route_reason));
    return string(ret);
}

string log_cr_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::cr_route_respond         > *msg)
{
    char ret[256];
    sprintf(ret, "cr_route_respond(%s->%s): obu=%s, ret=%s, kp=%d, lane=%d, time=%ld, dest=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), msg->body.route.num_of_kp, msg->body.route.num_of_lane, msg->body.route.time_stamp, msg->body.route.destination.c_str(), route_reason_str(msg->body.route.route_reason));
    return string(ret);
}

string log_ro_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::ro_route_respond         > *msg)
{
    char ret[256];
    sprintf(ret, "ro_route_respond(%s->%s): obu=%s, ret=%s, kp=%d, lane=%d, time=%ld, dest=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), msg->body.route.num_of_kp, msg->body.route.num_of_lane, msg->body.route.time_stamp, msg->body.route.destination.c_str(), route_reason_str(msg->body.route.route_reason));
    return string(ret);
}

string log_rc_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::rc_route_respond         > *msg)
{
    char ret[256];
    sprintf(ret, "rc_route_respond(%s->%s): obu=%s, ret=%s, kp=%d, lane=%d, time=%ld, dest=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), msg->body.route.num_of_kp, msg->body.route.num_of_lane, msg->body.route.time_stamp, msg->body.route.destination.c_str(), route_reason_str(msg->body.route.route_reason));
    return string(ret);
}

string log_ou_route_respond(         const                   nad_lcm::ou_route_respond           *msg)
{
    char ret[256];
    sprintf(ret, "ou_route_respond: obu=%s, ret=%s, kp=%d, lane=%d, time=%ld, dest=%s, reason=%s",
        msg->obu_name.c_str(), ret_str(msg->retcode), msg->route.num_of_kp, msg->route.num_of_lane, msg->route.time_stamp, msg->route.destination.c_str(), route_reason_str(msg->route.route_reason));
    return string(ret);
}

string log_uo_start_auto_request(    const                   nad_lcm::uo_start_auto_request      *msg)
{
    char ret[256];
    sprintf(ret, "uo_start_auto_request: obu=%s, time=%ld",
        msg->obu_name.c_str(), msg->time_stamp);
    return string(ret);
}

string log_or_start_auto_request(    const nad_lcm::ne_msg_t<nad_lcm::or_start_auto_request    > *msg)
{
    char ret[256];
    sprintf(ret, "or_start_auto_request(%s->%s): obu=%s, time=%ld, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.time_stamp, start_reason_str(msg->body.start_reason));
    return string(ret);
}

string log_rc_start_auto_request(    const nad_lcm::ne_msg_t<nad_lcm::rc_start_auto_request    > *msg)
{
    char ret[256];
    sprintf(ret, "rc_start_auto_request(%s->%s): obu=%s, time=%ld, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.time_stamp, start_reason_str(msg->body.start_reason));
    return string(ret);
}

string log_cr_start_auto_respond(    const nad_lcm::ne_msg_t<nad_lcm::cr_start_auto_respond    > *msg)
{
    char ret[256];
    sprintf(ret, "cr_start_auto_respond(%s->%s): obu=%s, ret=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), start_reason_str(msg->body.start_reason));
    return string(ret);
}

string log_ro_start_auto_respond(    const nad_lcm::ne_msg_t<nad_lcm::ro_start_auto_respond    > *msg)
{
    char ret[256];
    sprintf(ret, "ro_start_auto_respond(%s->%s): obu=%s, ret=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), start_reason_str(msg->body.start_reason));
    return string(ret);
}

string log_ou_start_auto_respond(    const                   nad_lcm::ou_start_auto_respond      *msg)
{
    char ret[256];
    sprintf(ret, "ou_start_auto_respond: obu=%s, ret=%s, reason=%s",
        msg->obu_name.c_str(), ret_str(msg->retcode), start_reason_str(msg->start_reason));
    return string(ret);
}

string log_uo_stop_auto_request(     const                   nad_lcm::uo_stop_auto_request       *msg)
{
    char ret[256];
    sprintf(ret, "uo_stop_auto_request: obu=%s",
        msg->obu_name.c_str());
    return string(ret);
}

string log_or_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::or_stop_auto_notify      > *msg)
{
    char ret[256];
    sprintf(ret, "or_stop_auto_notify(%s->%s): obu=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), stop_reason_str(msg->body.stop_reason));
    return string(ret);
}

string log_rc_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::rc_stop_auto_notify      > *msg)
{
    char ret[256];
    sprintf(ret, "rc_stop_auto_notify(%s->%s): obu=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), stop_reason_str(msg->body.stop_reason));
    return string(ret);
}

string log_cu_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::cu_stop_auto_notify      > *msg)
{
    char ret[256];
    sprintf(ret, "cu_stop_auto_notify(%s->%s): obu=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), stop_reason_str(msg->body.stop_reason));
    return string(ret);
}

string log_ro_stop_auto_respond(     const nad_lcm::ne_msg_t<nad_lcm::ro_stop_auto_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "ro_stop_auto_respond(%s->%s): obu=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), stop_reason_str(msg->body.stop_reason));
    return string(ret);
}

string log_ou_stop_auto_respond(     const                   nad_lcm::ou_stop_auto_respond       *msg)
{
    char ret[256];
    sprintf(ret, "ou_stop_auto_respond: obu=%s, ret=%s, reason=%s",
        msg->obu_name.c_str(), ret_str(msg->retcode), stop_reason_str(msg->stop_reason));
    return string(ret);
}

string log_dr_info_report(           const nad_lcm::ne_msg_t<nad_lcm::dr_info_report           > *msg)
{
    char ret[256];
    sprintf(ret, "dr_info_report(%s->%s): sensor=%d, obstacle=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.num_of_sensor, msg->body.num_of_obstacle);
    return string(ret);
}

string log_or_info_report(           const nad_lcm::ne_msg_t<nad_lcm::or_info_report           > *msg)
{
    char ret[256];
    sprintf(ret, "or_info_report(%s->%s): obu=%s, obstacle=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu.obu_name.c_str(), msg->body.num_of_obstacle);
    return string(ret);
}

string log_rc_info_report(           const nad_lcm::ne_msg_t<nad_lcm::rc_info_report           > *msg)
{
    char ret[256];
    sprintf(ret, "rc_info_report(%s->%s): rsu=%s, light=%d, limspeed=%d, block=%d, platoon=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.rsu.rsu_name.c_str(), msg->body.num_of_light, msg->body.num_of_limspeed, msg->body.num_of_block, msg->body.num_of_platoon);
    return string(ret);
}

string log_cu_info_report(           const nad_lcm::ne_msg_t<nad_lcm::cu_info_report           > *msg)
{
    char ret[256];
    sprintf(ret, "cu_info_report(%s->%s): rsu=%d, light=%d, limspeed=%d, block=%d, platoon=%d, task=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.num_of_rsu, msg->body.num_of_light, msg->body.num_of_limspeed, msg->body.num_of_block, msg->body.num_of_platoon, msg->body.num_of_task);
    return string(ret);
}

string log_cr_info_report(           const nad_lcm::ne_msg_t<nad_lcm::cr_info_report           > *msg)
{
    char ret[256];
    sprintf(ret, "cr_info_report(%s->%s): ret=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        ret_str(msg->body.retcode));
    return string(ret);
}

string log_mo_change_lane_request(   const                   nad_lcm::mo_change_lane_request     *msg)
{
    char ret[256];
    sprintf(ret, "mo_change_lane_request: dir=%s, start=%d, end=%d, reason=%s, status=%s",
        cl_direction_str(msg->direction), msg->starting_lane, msg->ending_lane, cl_reason_str(msg->reason), cl_status_str(msg->status));
    return string(ret);
}

string log_or_change_lane_request(   const nad_lcm::ne_msg_t<nad_lcm::or_change_lane_request   > *msg)
{
    char ret[256];
    sprintf(ret, "or_change_lane_request(%s->%s): obu=%s, dir=%s, start=%d, end=%d, reason=%s, status=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), cl_direction_str(msg->body.direction), msg->body.starting_lane, msg->body.ending_lane, cl_reason_str(msg->body.reason), cl_status_str(msg->body.status));
    return string(ret);
}

string log_ro_change_lane_respond(   const nad_lcm::ne_msg_t<nad_lcm::ro_change_lane_respond   > *msg)
{
    char ret[256];
    sprintf(ret, "ro_change_lane_respond(%s->%s): obu=%s, ret=%s, dir=%s, start=%d, end=%d, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), cl_direction_str(msg->body.direction), msg->body.starting_lane, msg->body.ending_lane, cl_reason_str(msg->body.reason));
    return string(ret);
}

string log_om_change_lane_respond(   const                   nad_lcm::om_change_lane_respond     *msg)
{
    char ret[256];
    sprintf(ret, "om_change_lane_respond: ret=%s, dir=%s,  end=%d, reason=%s",
        ret_str(msg->retcode), cl_direction_str(msg->direction), msg->ending_lane, cl_reason_str(msg->reason));
    return string(ret);
}

string log_uc_add_platoon_request(   const                   nad_lcm::uc_add_platoon_request     *msg)
{
    char ret[256];
    sprintf(ret, "uc_add_platoon_request: platoon=%d, speed=%.1fkm/h, dest=%s",
        msg->num_of_platoon, msg->speed, msg->destination.c_str());
    return string(ret);
}

string log_cr_add_platoon_request(   const nad_lcm::ne_msg_t<nad_lcm::cr_add_platoon_request   > *msg)
{
    char ret[256];
    sprintf(ret, "cr_add_platoon_request(%s->%s): platoon=%d, speed=%.1fkm/h, dest=%s, kp=%d, lane=%d, time=%ld",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.num_of_platoon, msg->body.speed, msg->body.destination.c_str(), msg->body.route.num_of_kp, msg->body.route.num_of_lane, msg->body.route.time_stamp);
    return string(ret);
}

string log_ro_add_platoon_notify(    const nad_lcm::ne_msg_t<nad_lcm::ro_add_platoon_notify    > *msg)
{
    char ret[256];
    sprintf(ret, "ro_add_platoon_notify(%s->%s): platoon=%d, speed=%.1fkm/h, dest=%s, kp=%d, lane=%d, time=%ld",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.num_of_platoon, msg->body.speed, msg->body.destination.c_str(), msg->body.route.num_of_kp, msg->body.route.num_of_lane, msg->body.route.time_stamp);
    return string(ret);
}

string log_ou_add_platoon_notify(    const                   nad_lcm::ou_add_platoon_notify      *msg)
{
    char ret[256];
    sprintf(ret, "ou_add_platoon_notify: platoon=%d, speed=%.1fkm/h, dest=%s, kp=%d, lane=%d, time=%ld",
        msg->num_of_platoon, msg->speed, msg->destination.c_str(), msg->route.num_of_kp, msg->route.num_of_lane, msg->route.time_stamp);
    return string(ret);
}

string log_rc_add_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::rc_add_platoon_respond   > *msg)
{
    char ret[256];
    sprintf(ret, "ou_add_platoon_notify(%s->%s): speed=%.1fkm/h, dest=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.speed, msg->body.destination.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cu_add_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::cu_add_platoon_respond   > *msg)
{
    char ret[256];
    sprintf(ret, "cu_add_platoon_respond(%s->%s): speed=%.1fkm/h, dest=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.speed, msg->body.destination.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_set_platoon_request(   const                   nad_lcm::uc_set_platoon_request     *msg)
{
    char ret[256];
    sprintf(ret, "uc_set_platoon_request: type=%s, obu=%s, speed=%.1fkm/h",
        sp_str(msg->type), msg->obu_name.c_str(), msg->speed);
    return string(ret);
}

string log_cr_set_platoon_request(   const nad_lcm::ne_msg_t<nad_lcm::cr_set_platoon_request   > *msg)
{
    char ret[256];
    sprintf(ret, "cr_set_platoon_request(%s->%s): type=%s, obu=%s, speed=%.1fkm/h",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        sp_str(msg->body.type), msg->body.obu_name.c_str(), msg->body.speed);
    return string(ret);
}

string log_ro_set_platoon_notify(    const nad_lcm::ne_msg_t<nad_lcm::ro_set_platoon_notify    > *msg)
{
    char ret[256];
    sprintf(ret, "ro_set_platoon_notify(%s->%s): type=%s, obu=%s, speed=%.1fkm/h",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        sp_str(msg->body.type), msg->body.obu_name.c_str(), msg->body.speed);
    return string(ret);
}

string log_ou_set_platoon_notify(    const                   nad_lcm::ou_set_platoon_notify      *msg)
{
    char ret[256];
    sprintf(ret, "ou_set_platoon_notify: type=%s, obu=%s, speed=%.1fkm/h",
        sp_str(msg->type), msg->obu_name.c_str(), msg->speed);
    return string(ret);
}

string log_rc_set_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::rc_set_platoon_respond   > *msg)
{
    char ret[256];
    sprintf(ret, "ro_set_platoon_notify(%s->%s): type=%s, obu=%s, speed=%.1fkm/h, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        sp_str(msg->body.type), msg->body.obu_name.c_str(), msg->body.speed, ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cu_set_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::cu_set_platoon_respond   > *msg)
{
    char ret[256];
    sprintf(ret, "cu_set_platoon_respond(%s->%s): type=%s, obu=%s, speed=%.1fkm/h, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        sp_str(msg->body.type), msg->body.obu_name.c_str(), msg->body.speed, ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_delete_platoon_request(const                   nad_lcm::uc_delete_platoon_request  *msg)
{
    char ret[256];
    sprintf(ret, "uc_delete_platoon_request: reason=%s",
        dpr_str(msg->reason));
    return string(ret);
}

string log_cr_delete_platoon_request(const nad_lcm::ne_msg_t<nad_lcm::cr_delete_platoon_request> *msg)
{
    char ret[256];
    sprintf(ret, "cr_delete_platoon_request(%s->%s): reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        dpr_str(msg->body.reason));
    return string(ret);
}

string log_rc_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::rc_delete_platoon_notify > *msg)
{
    char ret[256];
    sprintf(ret, "rc_delete_platoon_notify(%s->%s): reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        dpr_str(msg->body.reason));
    return string(ret);
}

string log_cu_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::cu_delete_platoon_notify > *msg)
{
    char ret[256];
    sprintf(ret, "cu_delete_platoon_notify(%s->%s): reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        dpr_str(msg->body.reason));
    return string(ret);
}

string log_ro_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::ro_delete_platoon_notify > *msg)
{
    char ret[256];
    sprintf(ret, "ro_delete_platoon_notify(%s->%s): reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        dpr_str(msg->body.reason));
    return string(ret);
}

string log_ou_delete_platoon_notify( const                   nad_lcm::ou_delete_platoon_notify   *msg)
{
    char ret[256];
    sprintf(ret, "ou_delete_platoon_notify: reason=%s",
        dpr_str(msg->reason));
    return string(ret);
}

string log_uc_oct_login_request(     const                   nad_lcm::uc_oct_login_request       *msg)
{
    char ret[256];
    sprintf(ret, "uc_oct_login_request: user=%s, pwd=%s",
        msg->csu_user.c_str(), msg->csu_password.c_str());
    return string(ret);
}

string log_cu_oct_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_oct_login_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "cu_oct_login_respond(%s->%s): csu=%s, ret=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.csu_name.c_str(), ret_str(msg->body.retcode));
    return string(ret);
}

string log_uc_config_request(        const                   nad_lcm::uc_config_request          *msg)
{
    char ret[256];
    sprintf(ret, "uc_config_request: csu=%s",
        msg->csu_name.c_str());
    return string(ret);
}

string log_cu_config_respond(        const nad_lcm::ne_msg_t<nad_lcm::cu_config_respond        > *msg)
{
    char ret[256];
    sprintf(ret, "cu_config_respond(%s->%s): csu=%s, obu=%d, rsu=%d, rsd=%d, task=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.csu_name.c_str(), msg->body.num_of_obu, msg->body.num_of_rsu, msg->body.num_of_rsd, msg->body.num_of_task);
    return string(ret);
}

string log_uc_exec_task_request(     const                   nad_lcm::uc_exec_task_request       *msg)
{
    char ret[256];
    sprintf(ret, "uc_exec_task_request: task=%s",
        msg->task_name.c_str());
    return string(ret);
}

string log_cu_exec_task_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_exec_task_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "cu_exec_task_respond(%s->%s): task=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.task_name.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_stop_task_request(     const                   nad_lcm::uc_stop_task_request       *msg)
{
    char ret[256];
    sprintf(ret, "uc_stop_task_request: task=%s",
        msg->task_name.c_str());
    return string(ret);
}

string log_cu_stop_task_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_stop_task_respond     > *msg)
{
    char ret[256];
    sprintf(ret, "cu_stop_task_respond(%s->%s): task=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.task_name.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cr_exec_task_func_request(const nad_lcm::ne_msg_t<nad_lcm::cr_exec_task_func_request> *msg)
{
    char ret[2048];
    sprintf(ret, "cr_exec_task_func_request(%s->%s): task=%s, seq=%d, func=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.task_name.c_str(), msg->body.seq_no, msg->body.task_func.c_str());
    return string(ret);
}

string log_rc_exec_task_func_respond(const nad_lcm::ne_msg_t<nad_lcm::rc_exec_task_func_respond> *msg)
{
    char ret[256];
    sprintf(ret, "rc_exec_task_func_respond(%s->%s): task=%s, seq=%d, fret=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.task_name.c_str(), msg->body.seq_no, msg->body.task_ret.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_set_ets_request(       const                   nad_lcm::uc_set_ets_request         *msg)
{
    char ret[256];
    sprintf(ret, "uc_set_ets_request: id=%s, type=%s, val=%d, reason=%s",
        msg->ets_id.c_str(), tet_str(msg->ets_type), msg->ets_value, ets_reason_str(msg->reason));
    return string(ret);
}

string log_cr_set_ets_request(       const nad_lcm::ne_msg_t<nad_lcm::cr_set_ets_request       > *msg)
{
    char ret[256];
    sprintf(ret, "cr_set_ets_request(%s->%s): id=%s, type=%s, val=%d, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason));
    return string(ret);
}

string log_rc_set_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::rc_set_ets_respond       > *msg)
{
    char ret[256];
    sprintf(ret, "rc_set_ets_respond(%s->%s): id=%s, type=%s, val=%d, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cu_set_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::cu_set_ets_respond       > *msg)
{
    char ret[256];
    sprintf(ret, "cu_set_ets_respond(%s->%s): id=%s, type=%s, val=%d, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_add_ets_request(       const                   nad_lcm::uc_add_ets_request         *msg)
{
    char ret[256];
    sprintf(ret, "uc_add_ets_request: id=%s, type=%s, val=%d, reason=%s",
        msg->ets_id.c_str(), tet_str(msg->ets_type), msg->ets_value, ets_reason_str(msg->reason));
    return string(ret);
}

string log_cr_add_ets_request(       const nad_lcm::ne_msg_t<nad_lcm::cr_add_ets_request       > *msg)
{
    char ret[256];
    sprintf(ret, "cr_add_ets_request(%s->%s): id=%s, type=%s, val=%d, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason));
    return string(ret);
}

string log_rc_add_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::rc_add_ets_respond       > *msg)
{
    char ret[256];
    sprintf(ret, "rc_add_ets_respond(%s->%s): id=%s, type=%s, val=%d, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cu_add_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::cu_add_ets_respond       > *msg)
{
    char ret[256];
    sprintf(ret, "cu_add_ets_respond(%s->%s): id=%s, type=%s, val=%d, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), msg->body.ets_value, ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_uc_delete_ets_request(    const                   nad_lcm::uc_delete_ets_request      *msg)
{
    char ret[256];
    sprintf(ret, "uc_delete_ets_request: id=%s, type=%s, reason=%s",
        msg->ets_id.c_str(), tet_str(msg->ets_type), ets_reason_str(msg->reason));
    return string(ret);
}

string log_cr_delete_ets_request(    const nad_lcm::ne_msg_t<nad_lcm::cr_delete_ets_request    > *msg)
{
    char ret[256];
    sprintf(ret, "cr_delete_ets_request(%s->%s): id=%s, type=%s, reason=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), ets_reason_str(msg->body.reason));
    return string(ret);
}

string log_rc_delete_ets_respond(    const nad_lcm::ne_msg_t<nad_lcm::rc_delete_ets_respond    > *msg)
{
    char ret[256];
    sprintf(ret, "rc_delete_ets_respond(%s->%s): id=%s, type=%s, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_cu_delete_ets_respond(    const nad_lcm::ne_msg_t<nad_lcm::cu_delete_ets_respond    > *msg)
{
    char ret[256];
    sprintf(ret, "rc_delete_ets_respond(%s->%s): id=%s, type=%s, reason=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.ets_id.c_str(), tet_str(msg->body.ets_type), ets_reason_str(msg->body.reason), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_rc_log_report(            const nad_lcm::ne_msg_t<nad_lcm::rc_log_report            > *msg)
{
    char ret[256];
    sprintf(ret, "rc_log_report(%s->%s): level=%s, log=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        log_str(msg->body.log_level), msg->body.log.c_str());
    return string(ret);
}

string log_cu_log_report(            const nad_lcm::ne_msg_t<nad_lcm::cu_log_report            > *msg)
{
    char ret[256];
    sprintf(ret, "cu_log_report(%s->%s): level=%s, log=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        log_str(msg->body.log_level), msg->body.log.c_str());
    return string(ret);
}

string log_rc_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::rc_alarm_report          > *msg)
{
    char ret[256];
    sprintf(ret, "rc_alarm_report(%s->%s): level=%s, alarm=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        alarm_str(msg->body.alarm_level), msg->body.alarm.c_str());
    return string(ret);
}

string log_cu_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::cu_alarm_report          > *msg)
{
    char ret[256];
    sprintf(ret, "cu_alarm_report(%s->%s): level=%s, alarm=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        alarm_str(msg->body.alarm_level), msg->body.alarm.c_str());
    return string(ret);
}

string log_ro_log_report(            const nad_lcm::ne_msg_t<nad_lcm::ro_log_report            > *msg)
{
    char ret[256];
    sprintf(ret, "ro_log_report(%s->%s): level=%s, log=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        log_str(msg->body.log_level), msg->body.log.c_str());
    return string(ret);
}

string log_ou_log_report(            const                   nad_lcm::ou_log_report              *msg)
{
    char ret[256];
    sprintf(ret, "ou_log_report: level=%s, log=%s",
        log_str(msg->log_level), msg->log.c_str());
    return string(ret);
}

string log_ro_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::ro_alarm_report          > *msg)
{
    char ret[256];
    sprintf(ret, "ro_alarm_report(%s->%s): level=%s, alarm=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        alarm_str(msg->body.alarm_level), msg->body.alarm.c_str());
    return string(ret);
}

string log_ou_alarm_report(          const                   nad_lcm::ou_alarm_report            *msg)
{
    char ret[256];
    sprintf(ret, "ro_alarm_report: level=%s, alarm=%s",
        alarm_str(msg->alarm_level), msg->alarm.c_str());
    return string(ret);
}

string log_sensor_obstacle_report(   const                   nad_lcm::sensor_obstacle_report     *msg)
{
    char ret[256];
    sprintf(ret, "sensor_obstacle_report: obstacle=%d",
        msg->num_of_obstacle);
    return string(ret);
}

string log_uc_call_car_request(      const                   nad_lcm::uc_call_car_request        *msg)
{
    char ret[256];
    sprintf(ret, "uc_call_car_request: obu=%s, kp=%d, dest=%s",
        msg->obu_name.c_str(), msg->num_of_kp, msg->destination.c_str());
    return string(ret);
}

string log_cu_call_car_respond(      const nad_lcm::ne_msg_t<nad_lcm::cu_call_car_respond      > *msg)
{
    char ret[256];
    sprintf(ret, "cu_call_car_respond(%s->%s): obu=%s, ret=%s, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), ret_str(msg->body.retcode), msg->body.description.c_str());
    return string(ret);
}

string log_om_center_line_report(    const                   nad_lcm::om_center_line_report      *msg)
{
    char ret[256];
    sprintf(ret, "om_center_line_report: replan=%d, lanes=%d",
        msg->replan_flag, msg->num_of_lanes);
    return string(ret);
}

string log_uo_upcall_request(        const                   nad_lcm::uo_upcall_request          *msg)
{
    char ret[512];
    sprintf(ret, "uo_upcall_request: obu=%s, upcall=%s, platoon=%d, speed=%.1fkm/h, dest=%s, type=%d, reason=%d, kp=%d",
        msg->obu_name.c_str(), upcall_str(msg->upcall_type), msg->num_of_platoon, msg->speed, msg->destination.c_str(), msg->type, msg->reason, msg->num_of_kp);
    return string(ret);
}

string log_or_upcall_request(        const nad_lcm::ne_msg_t<nad_lcm::or_upcall_request        > *msg)
{
    char ret[512];
    sprintf(ret, "or_upcall_request(%s->%s): obu=%s, upcall=%s, platoon=%d, speed=%.1fkm/h, dest=%s, type=%d, reason=%d, kp=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), upcall_str(msg->body.upcall_type), msg->body.num_of_platoon, msg->body.speed, msg->body.destination.c_str(), msg->body.type, msg->body.reason, msg->body.num_of_kp);
    return string(ret);
}

string log_rc_upcall_request(        const nad_lcm::ne_msg_t<nad_lcm::rc_upcall_request        > *msg)
{
    char ret[512];
    sprintf(ret, "rc_upcall_request(%s->%s): obu=%s, upcall=%s, platoon=%d, speed=%.1fkm/h, dest=%s, type=%d, reason=%d, kp=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), upcall_str(msg->body.upcall_type), msg->body.num_of_platoon, msg->body.speed, msg->body.destination.c_str(), msg->body.type, msg->body.reason, msg->body.num_of_kp);
    return string(ret);
}

//oct 新增
string log_uc_call_park_car_request(      const                   nad_lcm::uc_call_park_car_request   *msg)
{
    char ret[256];
    sprintf(ret, "uc_call_park_car_request(): obu=%s, des=%s",
        msg->obu_name.c_str(),  msg->destination.c_str());
    return string(ret);
}
string log_cu_call_park_info_report(      const nad_lcm::ne_msg_t<nad_lcm::cu_call_park_info_report > *msg)
{
    char ret[256];
    sprintf(ret, "cu_call_park_info_report(%s->%s): obu=%s, call_park_state=%d",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu.obu_name.c_str(), msg->body.car_status);
    return string(ret);
}
string log_cu_call_park_car_respond(      const nad_lcm::ne_msg_t<nad_lcm::cu_call_park_car_respond > *msg)
{
    char ret[256];
    sprintf(ret, "cu_call_park_car_respond(%s->%s): obu=%s, ret=%d, desc=%s",
        msg->header.local_ne_name.c_str(), msg->header.peer_ne_name.c_str(),
        msg->body.obu_name.c_str(), msg->body.retcode, msg->body.description.c_str());
    return string(ret);
}
