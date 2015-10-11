/*=============================================================================
>>>>>>> 93428c7c50a070f55cc512331fed8b68b51d667b
#
# Author: 杨广华 - edesale@qq.com
#
# QQ : 374970456
#
# Last modified:	2015-10-09 19:35
#`
# Filename:		data_free.c
#
# Description:释放内存 
#
=============================================================================*/
#include "data.h"
#include <stdlib.h>
#include <stddef.h>

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
	array_free(&tbsdata_extension->value);
}

/**
 * YGH 3
 */
static void elliptic_curve_point_free(elliptic_curve_point* elliptic_curve_point){
	if(NULL != elliptic_curve_point->x.buf)  
		array_free(&elliptic_curve_point->x);

	if(elliptic_curve_point->type == uncompressed)
		if(NULL != elliptic_curve_point->u.y.buf)
			array_free(&elliptic_curve_point->u.y);
}
/*
 * YGH  4
*/

static void elliptic_curve_point_free(elliptic_curve_point* elliptic_curve_point){
	if(NULL != elliptic_curve_point->x.buf)  
		free(&elliptic_curve_point->x);

	if(elliptic_curve_point->type == uncompressed)
		if(NULL != elliptic_curve_point->y.buf)
			free(&elliptic_curve_point->y);
}
/*
 *YGH  4
 */
static void ecdsa_signature_free(ecdsa_signature* ecdsa_signature){
	
	elliptic_curve_point_free(*ecdsa_signature=->r);

	if(NULL != ecdsa_signature->s.buf)
		free(&ecdsa_signature->s);
}


/*
 * YGH  5 
 * @signature* 需要释放的结构体
 * @pk_algorithm 外部传入参数,看协议中signature的定义
 */
static void signature_free(signature* signature, pk_algorithm pk_algorithm  ){
	switch(pk_algorithm){
		case ECDSA_NISTP224_WITH_SHA224:
			break;
		case ECDSA_NISTP256_WITH_SHA256:
			ecdsa_signature_free(&signature->u.ecdsa_signature);
			break;
		default:
			array_free(&signature->u.signature);
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
			elliptic_curve_point_free(&public_key->u.public_key);
			break;
		default:
			array_free(&public_key->u.other_key);
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
			array_free(&rectangular_regionu->u.rectangular_region);
		case POLYGON:
			free(rectangular_region->u.polygonal_region);
		case NONE:
			break;
		case default:
			array_free(&geographic_region->u.other_region);
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
}


/*
 *YGH  5  里面有一个union 先空着，等下写
 */
static void signature_free(signature* signature){
	
}




/*
 *YGH 6
 */

static void public_key_free(public_key* public_key){
	switch(public_key->algorithm){
		case ecdsa_nistp224_with_sha224:
		case ecdsa_nistp256_with_sha256:
			elliptic_curve_point_free(&public_key->public_key);
			break;
		case ecies_nistp256:
			elliptic_curve_point_free(&public_key->public_key);
			break;
		case other_value:
			array_free(&public_key->other_key);
			break;
	}
}

/*
 *YGH 7
 */
static void geographic_region_free(geographic_region* geographic_region){
	switch(geographic_region->region_type){
		case from_issuer:
			break;
		case circular:
		case rectangle:
			array_free(&rectangular_region->rectangular_region);
		case polygonal:
			free(rectangular_region->polygonal_region);
		case none:
			break;
		case other_value:
			array_free(&geographic_region->other_region);
	}
}

/*
 *YGH 8
 */

static void psid_priority_free(psid_priority*  psid_priority){
	free(psid_priority->psid);
}


/*
 *YGH 9
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



















