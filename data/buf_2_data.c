#include"data.h"
#include<stdlib.h>
#include<stddef.h>

#define get8(addr)  *( (u8*)addr  )
#define get16(addr) *( (u16*)addr )
#define get32(addr) *( (u32*)addr )
#define get64(addr) *( (u64*)addr )

#define CURRETN_VERSION 2

    
u32 buf_2_sec_data(const u8* buf,const u32 len, sec_data* sec_data){
    u8* mbuf = buf;
    u32 size = len;
    //检查长度是否满足最低要求
    if(size < 4)
        return -1;

    sec_data->protocol_version = get8(mbuf);
    mbuf = mbuf + 1;
    size = size - 1;
    if(sec_data->protocol_version != CURRETN_VERSION )
        return -1;
    
    sec_data->type = get16(mbuf);
    sec_data->type = be_to_host16(sec_data->type);
    mbuf = mbuf + 2;
    size = size - 2;
    
    switch(sec_data->type){
        case UNSECURED:
            sec_data->u.data.buf = (u8*)malloc(size);
            if(sec_data->u.data.buf == NULL)
                return -1;
            sec_data->u.data.buf = mbuf;
            sec_data->u.data.len = size;
            return 0;
        case SIGNED:
        case SIGNED_PARTIAL_PAYLOAD:
        case SIGNED_EXTERNAL_PAYLOAD:
            if(buf_2_signed_data(mbuf,size,&sec_data->u.signed_data))
                return -1;
            return 0;
        case SIGNED_WSA:
            if(buf_2_signed_wsa(mbuf,size,&sec_data->u.signed_wsa))
                return -1;
            return 0;
        case ENCRYPTED:
            if( buf_2_encrypted_data(mbuf,size,&sec_data->u.encrypted_data))
                 return -1;
            return 0;
        case CONTENT_TYPE_CRL_REQUEST:
            if(buf_2_crl_request(mbuf,size,&sec_data->u.crl_request))
                 return -1;
            return 0;
        case CRL:
            if(buf_2_crl(mbuf,size,&sec_data->u.crl))
                 return -1;
            return 0;
        default:
            sec_data->u.other_data.buf = (u8*)malloc(size);
            if(sec_data->u.other_data.buf == NULL)
                 return -1;
            sec_data->u.other_data.buf = mbuf;
            sec_data->u.other_data.len = size;

    }
    return 0;
}

