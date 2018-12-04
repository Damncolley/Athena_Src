/*-------------------------------------------------------
 * 文件名：zmq_t.cpp
 * 创建者：王飞虎
 * 时  间：2016-08-12
 * 描  述：zmq的公共类
-------------------------------------------------------*/

#include "zmq_t.h"
#include "../nad_msg.h"


zmq_t::zmq_t(): _context(1)
{
    pthread_spin_init(&spinlock_zmq, 0);
}

zmq_t::~zmq_t()
{
    pthread_spin_destroy(&spinlock_zmq);
}

void zmq_t::dispatch_handlers(nad_lcm::ne_msg_base_t &ne_msg)
{
    int len = ne_msg.getEncodedSize();
    uint8_t buf[len];
    ne_msg.encode(buf, 0, len);

    int64_t recv_utime = lcm_timestamp_now();

    lcm_recv_buf_t rbuf;
    rbuf.data = buf;
    rbuf.data_size = len;
    rbuf.recv_utime = recv_utime;
    rbuf.lcm = g_lcm->getUnderlyingLCM();

    if (::lcm_try_enqueue_message(rbuf.lcm, ne_msg.header.peer_channel.c_str()))
    {
        ::lcm_dispatch_handlers(rbuf.lcm, &rbuf, ne_msg.header.peer_channel.c_str());
    }
}
