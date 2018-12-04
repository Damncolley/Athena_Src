/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __obu_lcm_control_info_report_hpp__
#define __obu_lcm_control_info_report_hpp__

#include <vector>

namespace obu_lcm
{

/**
 * /// <summary>
 * /// 车辆信息上报
 * /// </summary>
 * /// <summary>
 * /// 当前GPS时间
 * /// </summary>
 * double gps_time;
 * /// <summary>
 * /// 当前纬度
 * /// </summary>
 * double lat;
 * /// <summary>
 * /// 当前经度
 * /// </summary>
 * double lon;
 * /// <summary>
 * /// 当前头指向
 * /// </summary>
 * double yaw;
 * /// <summary>
 * /// 当前速度
 * /// </summary>
 * double vehicle_speed;
 * /// <summary>
 * /// 当前方向盘转角
 * /// </summary>
 * double steer_angle;
 * /// <summary>
 * /// 转向状态
 * /// </summary>
 * int8_t steer_status;
 * /// <summary>
 * /// 油门值
 * /// </summary>
 * double accel_value;
 * /// <summary>
 * /// 油门状态
 * /// </summary>
 * int8_t accel_status;
 * /// <summary>
 * /// 当前刹车值
 * /// </summary>
 * double brake_value;
 * /// <summary>
 * /// 刹车状态：自动控制、人工控制
 * /// </summary>
 * int8_t brake_status;
 * /// <summary>
 * /// 闪烁状态（转向灯状态）
 * /// </summary>
 * int8_t flashing_status;
 * /// <summary>
 * /// 档位状态
 * /// </summary>
 * int8_t at_status;
 * /// <summary>
 * /// EPB状态
 * /// </summary>
 * int8_t epb_status;
 * /// <summary>
 * /// 预留字段个数
 * /// </summary>
 * int8_t num_of_reserved;
 * /// <summary>
 * /// 预留字段数组
 * /// </summary>
 * double reserved[num_of_reserved];
 *
 */
class control_info_report
{
    public:
        double     gps_time;

        double     lat;

        double     lon;

        double     yaw;

        double     vehicle_speed;

        double     steer_angle;

        int8_t     steer_status;

        double     accel_value;

        int8_t     accel_status;

        double     brake_value;

        int8_t     brake_status;

        int8_t     flashing_status;

        int8_t     at_status;

        int8_t     epb_status;

        int8_t     num_of_reserved;

        std::vector< double > reserved;

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
         * Returns "control_info_report"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int control_info_report::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int control_info_report::decode(const void *buf, int offset, int maxlen)
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

int control_info_report::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t control_info_report::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* control_info_report::getTypeName()
{
    return "control_info_report";
}

int control_info_report::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->gps_time, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->lat, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->lon, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->yaw, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->vehicle_speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->steer_angle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->steer_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->accel_value, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->accel_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->brake_value, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->brake_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->flashing_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->at_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->epb_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->num_of_reserved, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->num_of_reserved > 0) {
        tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->reserved[0], this->num_of_reserved);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int control_info_report::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->gps_time, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->lat, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->lon, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->yaw, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->vehicle_speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->steer_angle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->steer_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->accel_value, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->accel_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->brake_value, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->brake_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->flashing_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->at_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->epb_status, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->num_of_reserved, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->num_of_reserved) {
        this->reserved.resize(this->num_of_reserved);
        tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->reserved[0], this->num_of_reserved);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int control_info_report::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __double_encoded_array_size(NULL, this->num_of_reserved);
    return enc_size;
}

uint64_t control_info_report::_computeHash(const __lcm_hash_ptr *)
{
    uint64_t hash = 0xa5b415e3074fd294LL;
    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
