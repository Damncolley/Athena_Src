/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __nad_lcm_ro_set_platoon_notify_hpp__
#define __nad_lcm_ro_set_platoon_notify_hpp__

#include <string>

namespace nad_lcm
{

/// rsu->obu：修改编队通知（仅RSU修改成功时发出）
class ro_set_platoon_notify
{
    public:
        std::string platoon_name;

        int32_t    type;

        std::string obu_name;

        double     speed;

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
         * Returns "ro_set_platoon_notify"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int ro_set_platoon_notify::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int ro_set_platoon_notify::decode(const void *buf, int offset, int maxlen)
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

int ro_set_platoon_notify::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t ro_set_platoon_notify::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* ro_set_platoon_notify::getTypeName()
{
    return "ro_set_platoon_notify";
}

int ro_set_platoon_notify::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    char* platoon_name_cstr = (char*) this->platoon_name.c_str();
    tlen = __string_encode_array(buf, offset + pos, maxlen - pos, &platoon_name_cstr, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    char* obu_name_cstr = (char*) this->obu_name.c_str();
    tlen = __string_encode_array(buf, offset + pos, maxlen - pos, &obu_name_cstr, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __double_encode_array(buf, offset + pos, maxlen - pos, &this->speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int ro_set_platoon_notify::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    int32_t __platoon_name_len__;
    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &__platoon_name_len__, 1);
    if(tlen < 0) return tlen; else pos += tlen;
    if(__platoon_name_len__ > maxlen - pos) return -1;
    this->platoon_name.assign(((const char*)buf) + offset + pos, __platoon_name_len__ - 1);
    pos += __platoon_name_len__;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    int32_t __obu_name_len__;
    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &__obu_name_len__, 1);
    if(tlen < 0) return tlen; else pos += tlen;
    if(__obu_name_len__ > maxlen - pos) return -1;
    this->obu_name.assign(((const char*)buf) + offset + pos, __obu_name_len__ - 1);
    pos += __obu_name_len__;

    tlen = __double_decode_array(buf, offset + pos, maxlen - pos, &this->speed, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int ro_set_platoon_notify::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->platoon_name.size() + 4 + 1;
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += this->obu_name.size() + 4 + 1;
    enc_size += __double_encoded_array_size(NULL, 1);
    return enc_size;
}

uint64_t ro_set_platoon_notify::_computeHash(const __lcm_hash_ptr *)
{
    uint64_t hash = 0xe9d39bfc33db42c6LL;
    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
