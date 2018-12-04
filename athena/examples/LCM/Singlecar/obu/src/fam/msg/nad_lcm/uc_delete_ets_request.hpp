/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __nad_lcm_uc_delete_ets_request_hpp__
#define __nad_lcm_uc_delete_ets_request_hpp__

#include <string>

namespace nad_lcm
{

/**
 * -------------------------------------------------------------------
 * 删除电子交通牌的接口
 * -------------------------------------------------------------------
 * oct->csu：删除ets（仅电子限速牌&电子施工标志，不含红绿灯&实物施工标志）
 */
class uc_delete_ets_request
{
    public:
        std::string ets_id;

        int32_t    ets_type;

        int32_t    reason;

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
         * Returns "uc_delete_ets_request"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int uc_delete_ets_request::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int uc_delete_ets_request::decode(const void *buf, int offset, int maxlen)
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

int uc_delete_ets_request::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t uc_delete_ets_request::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* uc_delete_ets_request::getTypeName()
{
    return "uc_delete_ets_request";
}

int uc_delete_ets_request::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    char* ets_id_cstr = (char*) this->ets_id.c_str();
    tlen = __string_encode_array(buf, offset + pos, maxlen - pos, &ets_id_cstr, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->ets_type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->reason, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int uc_delete_ets_request::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    int32_t __ets_id_len__;
    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &__ets_id_len__, 1);
    if(tlen < 0) return tlen; else pos += tlen;
    if(__ets_id_len__ > maxlen - pos) return -1;
    this->ets_id.assign(((const char*)buf) + offset + pos, __ets_id_len__ - 1);
    pos += __ets_id_len__;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->ets_type, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->reason, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int uc_delete_ets_request::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->ets_id.size() + 4 + 1;
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    return enc_size;
}

uint64_t uc_delete_ets_request::_computeHash(const __lcm_hash_ptr *)
{
    uint64_t hash = 0x6fef138597566ee3LL;
    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
