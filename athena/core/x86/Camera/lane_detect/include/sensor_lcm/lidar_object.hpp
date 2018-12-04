/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __sensor_lcm_lidar_object_hpp__
#define __sensor_lcm_lidar_object_hpp__

#include "sensor_lcm/vbox_3d.hpp"

namespace sensor_lcm
{

/**
 * 
 * 激光雷达目标检测
 * 
 * int8_t  type;    //（1-car，2-person，3-
 * int8_t  id;      //主要用于跟踪目标
 * int8_t  status;  //检测状态：1-新目标、2-已跟踪
 * float   score;   //置信度
 * vbox_3d v_box;   //车辆坐标系的3D目标框
 * float   lon_rate;   //y方向速度，单位为：m/s
 * float   lat_rate;   //x方向速度，单位为：m/s
 *
 */
class lidar_object
{
    public:
        int8_t     type;

        int8_t     id;

        int8_t     status;

        float      score;

        sensor_lcm::vbox_3d v_box;

        float      lon_rate;

        float      lat_rate;

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to reqad while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "lidar_object"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int lidar_object::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int lidar_object::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int lidar_object::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t lidar_object::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* lidar_object::getTypeName()
{
    return "lidar_object";
}

int lidar_object::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->id, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->score, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->v_box._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->lon_rate, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_encode_array(buf, offset + pos, maxlen - pos, &this->lat_rate, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int lidar_object::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->id, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->score, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->v_box._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->lon_rate, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __float_decode_array(buf, offset + pos, maxlen - pos, &this->lat_rate, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int lidar_object::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += this->v_box._getEncodedSizeNoHash();
    enc_size += __float_encoded_array_size(NULL, 1);
    enc_size += __float_encoded_array_size(NULL, 1);
    return enc_size;
}

uint64_t lidar_object::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == lidar_object::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, (void*)lidar_object::getHash };

    uint64_t hash = 0x8ed7c6a3e8a72433LL +
         sensor_lcm::vbox_3d::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
