#ifndef __NE_lcm_hpp__
#define __NE_lcm_hpp__


#include <lcm/lcm-cpp.hpp>
#include "ne_msg_t.hpp"

#include <iostream>

class NE_LCM: public lcm::LCM
{
public:

    NE_LCM(std::string lcm_url) : lcm::LCM (lcm_url)
    {
    }

    //网元间消息用publish_nemsg发送
    //网元内消息用原始接口publish发送
    template <class T>
    int publish_nemsg(T &msg)
    {
        msg.encode_body();
        string tem("NEMSG_");
        tem += stoupper(msg.header.local_ne_name);
        int ret = publish(tem, &msg);
        if (ret != 0 || msg.data_len > 8 * 1024)
        {
            printf("!!!! publish_nemsg(%s)->%s: body_len=%d, ret=%d\n",
                msg.header.peer_channel.c_str(), msg.header.peer_ne_name.c_str(), msg.data_len, ret);
        }
        return ret;
    };

    int async_handle(struct timeval tv)
    {
        int ret = 0;
        int lcm_fd = getFileno();
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(lcm_fd, &readfds);
        int status = 0;
        status = select (lcm_fd+1, &readfds, NULL, NULL, &tv);
        if (status < 0)  //select出错的情况
        {
            std::cout<< "ERROR! select failed\n"<< std::endl;
            return ret;
        }

        //status == 0表示超时，status > 0表示有文件描述符已经ready
        if (FD_ISSET (lcm_fd,&readfds))
        {
             ret = handle();
        }
        return ret;
    }
};

#endif
