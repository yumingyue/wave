/*=============================================================================
#
# Author: 杨广华 - edesale@qq.com
#
# QQ : 374970456
#
# Last modified: 2015-10-11 14:45
#
# Filename: data_free.c
#
# Description: 
#
=============================================================================*/
#include "data.h"
#include <stdlib.h>
#include <stddef.h>


#define	 ARRAY_FREE(m)	array_free((void**)(m))


/**
 * YGH 1
 */
static void array_free(void **addr){
	free(*addr);
	*addr = NULL;
}

/**	
 * YGH 2
 */
static void tbsdata_extension_free(tbsdata_extension* tbsdata_extension)	{
	if(NULL == tbsdata_extension->value.buf)
		return ;
	ARRAY_FREE((&tbsdata_extension->value));
}

/**
 * YGH 3
 * @elliptic_curve_point* 释放的目标
 * @algorithm 外部传入参数(这个数据在free中也不起作用，删除)
 * @field外部传入参数，在释放函数中好像没用，先保留，这个数据不需要，删除就好
 */
static void elliptic_curve_point_free(elliptic_curve_point* elliptic_curve_point){
	if(NULL != elliptic_curve_point->x.buf) 
		ARRAY_FREE(&elliptic_curve_point->x);

	if(elliptic_curve_point->type == UNCOMPRESSED)
		if(NULL != elliptic_curve_point->u.y.buf)
		ARRAY_FREE(&elliptic_curve_point->u.y);
}
 /*
 *YGH  4
 */
static void ecdsa_signature_free(ecdsa_signature* ecdsa_signature){
	
	elliptic_curve_point_free(&ecdsa_signature->r);
	if(NULL != ecdsa_signature->s.buf)
			ARRAY_FREE(&ecdsa_signature->s);
}


/*
 * YGH  5 
 * @signature* 需要释放的结构体
 * @pk_algorithm 外部传入参数,看协议中signature的定义
 */
static void signature_free(signature* signature, pk_algorithm algorithm  ){
	switch(algorithm){
		case ECDSA_NISTP224_WITH_SHA224:
			break;
		case ECDSA_NISTP256_WITH_SHA256:
			ecdsa_signature_free(&signature->u.ecdsa_signature);
			break;
		default:
			ARRAY_FREE(&signature);
	}			
}
/*
 * YGH 6
 */

static void public_key_free(public_key* public_key){
	switch(public_key->algorithm){
		case ECDSA_NISTP224_WITH_SHA224:
			break;
		case ECDSA_NISTP256_WITH_SHA256:
			elliptic_curve_point_free(&public_key->u.public_key);
			break;
		case ECIES_NISTP256:
			elliptic_curve_point_free(&public_key->u.ecies_nistp256.public_key);
			break;
		default:
			if(NULL != public_key->u.other_key.buf)
			ARRAY_FREE(&public_key->u.other_key);
			break;
	}
}

/**
 * YGH 7
 */
static void geographic_region_free(geographic_region* geographic_region){
	switch(geographic_region->region_type){
		case FROM_ISSUER:
			break;
		case CIRCLE:
			break;
		case RECTANGLE:
			ARRAY_FREE(&geographic_region->u.rectangular_region);
		case POLYGON:
			free(geographic_region->u.polygonal_region);
		case NONE:
			break;
		default:
			ARRAY_FREE(&geographic_region->u.other_region);
	}
}

/**
 * YGH 8
 */

static void psid_priority_free(psid_priority*  psid_priority){
	free(psid_priority->psid);
}

/**
 * YGH 9
 */

static void psid_priority_array_free(psid_priority_array*  psid_priority_array){
	
}


/**
 *YGH 10
 */

static void psid_array_free(psid_array* psid_array){

		
}

/**
 *YGH 11
 */
static void psid_ssp_free(psid_ssp* psid__ssp_array){
}

/**
 *YGH 12
 */
static void psid__ssp_array_free(psid_ssp_array* psid_ssp_array){
}

/**
 *YGH 13
 */

static void psid_priority_ssp_free(psid_priority_ssp* psid_priority_ssp){
}
/**
 *YGH 14
 */
static void psid_priority_ssp_array_free(psid_priority_array* psid_priority_array){
	}
/**
 *YGH 15
 */
static void wsa_scope_free(wsa_scope* wsa_scope){

}

/**
 *YGH 16
 */
static void anonymous_scope_free(anonymous_scope* anonymous_scope){
}

/**
 *YGH 17
 */
static void identified_scope_free(identified_scope* identified_scope){
}
/**
 *YGH 18
 */
static void identified_not_localized_scope_free(identified_not_localized_scope* 
				identified_not_localized_scope){
}
/**
 *YGH 19
 */
static void wsa_ca_scope_free(wsa_ca_scope* wsa_ca_scope){
}

/**
 *YGH 20
 */
static void sec_data_exch_ca_scope_free(sec_data_exch_ca_scope* sec_data_exch_ca_scope){
}

/**
 *YGH 21
 */
static void root_ca_scope_free(root_ca_scope* root_ca_scope){

}
/**
 *YGH 22
 *@holder_typ 外部数据结构传入的参数
 */
static void cert_specific_data_free(cert_specific_data* cert_specific_datai,holder_type holder_type){

}
/**
 *YGH 23
 *@uint8 外部数据结构传入
 */
static void tobesigned_certificate_free(tobesigned_certificate* tobesigned_certificatei, 
				u8 version_and_type){
}
/**
 *YGH 24
 */
static void certificate_free(certificate* certificate){

}

/**
 *YGH 25
 */
static void signer_identifier_free(signer_identifier* signer_identifier){

}
 /**
 *YGH 26
 */
static void tobesigned_crl_free(tobesigned_crl* tobesigned_crl){
}
/**
 *YGH 27
 */
static void crl_free(crl* crl){
}

/**
 *YGH 28
 */
static void tobe_encrypted_certificate_request_error_free(tobe_encrypted_certificate_request_error*
				tobe_encrypted_certificate_request_error){

}
/**
 *YGH 29
 *@version_and_type外部数据结构传入参数
 */
static void tobe_encrypted_anonymous_cert_response_free(tobe_encrypted_anonymous_cert_response* 
				tobe_encrypted_anonymous_cert_response, u8 version_and_type){
}
/**
 *YGH 30
 */
static void tobesigned_certificate_request_free(tobesigned_certificate_request* 
				tobesigned_certificate_request){
}
/**
 *YGH 31
 */
static void certificate_request_free(certificate_request* certificate_request){

}

/**
 *YGH 32??????/////////////现不写，保留下
 */
static void tobesigned_data_free(tobesigned_data* tobesigned_data, content_type type){
}
/**
 *YGH 33
 */
static void signed_data_free(signed_data* signed_data, content_type type){
}
/**
 *YGH 34
 */
static void tobe_encrypted_free(tobe_encrypted* tobe_encrypted){
}
/**
 *YGH 35
 */
static void aes_ccm_ciphertext_free(aes_ccm_ciphertext* aes_ccm_ciphertext){
	
}
/**
 *YGH 36
 */
static void ecies_nist_p256_encrypted_key_free(ecies_nist_p256_encrypted_key* 
				ecies_nist_p256_encrypted_key){
}
/**
 *YGH 37
 */
static void recipient_info_free(recipient_info* recipient_info, pk_algorithm algorithm){

}

/**
 *YGH 38
 */
static void  encrypted_data_free(encrypted_data* encrypted_data){
	
}
/**
 *YGH 39
 */
static void tobesigned_wsa_free(tobesigned_wsa* tobesigned_wsa){
}
/**
 *YGH 40
 */
static void signed_wsa_free(signed_wsa* signed_wsa){

}
/**
 *大力 41
 */

void sec_data_free(sec_data* sec_data){
    if(sec_data == NULL)
        return ;
    switch(sec_data->type){
        case UNSECURED:
           ARRAY_FREE(&sec_data->u.data);
            break;
        case SIGNED:
        case SIGNED_EXTERNAL_PAYLOAD:
        case SIGNED_PARTIAL_PAYLOAD:
            ARRAY_FREE(&sec_data->u.signed_data);
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
            ARRAY_FREE(&sec_data->u.crl);
            break;
        default:
			ARRAY_FREE(&sec_data->u.other_data);
            break;

    }
}



















