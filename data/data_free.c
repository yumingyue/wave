#include "data.h"
#include <stdlib.h>
#include <stddef.h>
/**
 * 作者：大力
 * /
static void array_free(void **addr){
    free(*addr);
    *(u16*)( (char*)*addr+1 ) = 0;
}
/**
 * 作者：大力
 */
void sec_data_free(sec_data* sec_data){
    if(sec_data == NULL)
        return ;
    switch(sec_data->type){
        case UNSECURED:
            array_free(sec_data->u.data);
            break;
        case SIGNED:
        case SIGNED_EXTERNAL_PAYLOAD:
        case SIGNED_PARTIAL_PAYLOAD:
            array_free(&sec_data->u.signed_data);
            break;
        case SIGNED_WSA:
            signed_wsa_free(&sec_data->u.signed_wsa);
            break;
        case ENCRYPTED:
            encrypted_data_free(&sec_data->u.encrypted_data);
            break;
        case CONTENT_TYPE_CRL_REQUEST:
            crl_request_free(&sec_data->u.crl_request);
            break;
        case CRL:
            crl_free(&sec_data->u.crl);
            break;
        default:
            array_free(sec_data->u.other_data);
            break;

    }
}
