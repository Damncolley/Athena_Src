/*-------------------------------------------------------
 * 文件名：zmq_t.h
 * 创建者：王飞虎
 * 时  间：2016-08-12
 * 描  述：zmq的公共类
-------------------------------------------------------*/

#ifndef _ZMQ_T_H_
#define _ZMQ_T_H_


extern "C"{
#include "lcm/lcm_internal.h"
}
#include "lcm/udpm_util.h"
#include "lcm/lcm-cpp.hpp"
#include "zmq.hpp"


#include "ne_msg/ne_msg_base_t.hpp"
#include "ne_msg/ne_msg_t.hpp"


class zmq_t
{
public:
    zmq::context_t _context;

public:
     pthread_spinlock_t spinlock_zmq;

public:
    zmq_t();
    virtual ~zmq_t();

    void dispatch_handlers(nad_lcm::ne_msg_base_t &ne_msg);

   virtual void communicate_send(const lcm::ReceiveBuffer* rbuf, const std::string& chan,
           const nad_lcm::ne_msg_base_t* ne_msg) = 0;

    template<typename T>
    inline void  send_nemsg(nad_lcm::ne_msg_t<T> &ne_msg)
    {
        ne_msg.encode_body();
        uint32_t len = ne_msg.getEncodedSize();
        uint8_t buf[len];
        ne_msg.encode(buf, 0, len);
        nad_lcm::ne_msg_base_t *ne_base_msg = static_cast<nad_lcm::ne_msg_base_t *>(&ne_msg);

        lcm::ReceiveBuffer rbuf;
        rbuf.data = buf;
        rbuf.data_size = len;
        rbuf.recv_utime = lcm_timestamp_now();

        communicate_send(&rbuf, "NEMSG_SKIP_LCM", ne_base_msg);
    }
};

#endif
