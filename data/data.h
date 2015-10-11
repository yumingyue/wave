/*=============================================================================
#
# Author: 杨广华 - edesale@qq.com
#
# QQ : 374970456
#
# Last modified: 2015-10-09 16:30
#
# Filename: data.h
#
# Description:将需要单独释放的结构体进行释放，主要释放的是结构体中带有指针
# 且指针指向一块分配了的内存/
#
=============================================================================*/
#ifndef DATA_H
#define DATA_H
#include "../utils/common.h"


/*
 * ARRAY 中的buff需要释放1
*/
#define ARRAY(TYPE,name) struct{\
        TYPE* buf;\
        u16 len;} name

typedef u64 time64;
typedef u8* psid;
typedef u16 certificate_duration;
typedef u32 crl_series;
typedef u32 time32;

/**
 * 下面两个字段都是协议中有，但是没有定义他里面有什么类型
 * */
typedef enum tbsdata_extension_type{
    UNUSED = 0,
} tbsdata_extension_type;
typedef enum tobe_encrypted_anonymous_cert_response{
    UNUSED = 0,				//unused?
}tobe_encrypted_anonymous_cert_response;


typedef enum crl_type{
    ID_ONLY = 0,
    ID_AND_EXPIRY = 1,
}crl_type;

typedef enum certificate_request_error_code{
    VERIFICATION_FAILURE = 0,
    FUTURE_REQUEST = 1,
    REQUEST_TOO_OLD = 2,
    FUTURE_ENROLMENT_CERT = 3,
    ENROLMENT_CERT_EXPIRED = 4,
    ENROLMENT_CERT_REVOKED = 5,
    ENROLMENT_CERT_UNAUTHORIZED_TYPE = 6,
    ENROLMENT_CERT_UNAUTHORIZED_REGION = 6,
    ENROLMENT_CERT_UNAUTHORIZED_PSIDS = 6,
    FUTURE_CA_CERT = 7,
    CA_CERT_EXPIERD = 8,
    CA_CEAT_REVOKED = 9,
}certificate_request_error_code;

typedef enum region_type{
    FROM_ISSUER = 0,
    CIRCLE = 1,
    RECTANGLE = 2,
    POLYGON = 3,
    NONE = 4,
}region_type;

typedef enum ecc_public_keytype{
    X_COORDINATE_ONLY = 0,
    COMPRESSED_LSB_Y_0 = 2,
    COMPRESSED_LSB_Y_1 = 3,
    UNCOMPRESSED = 4,
}ecc_public_keytype;


typedef enum array_type{
    ARRAY_TYPE_FROM_ISSUER = 0,
    ARRAY_TYPE_SPECIFIED = 1,
}array_type;

typedef enum signer_identifier_type{
    SELF = 0,
    CERTIFICATE_DIGEST_WITH_ECDSAP224 = 1,
    CERTIFICATE_DIGEST_WITH_ECDSAP256 = 2,
    CERTIFICATE = 3,
    CERTIFICATE_CHAIN = 4,
    CERTIFICATE_DIGETS_WITH_OTHER_ALGORITHM = 5,
}signer_identifier_type;

typedef enum symm_algorithm{
    AES_128_CCM = 0,
}symm_algorithm;

typedef enum holder_type{
    SDE_ANONYMOUS = 0,
    SDE_IDENTIFIED_NOT_LOCALIZED = 1,
    SDE_IDENTIFIED_LOCALIZED = 2,
    SDE_ENROLMENT = 3,
    WSA = 4,
    WSA_ENROLMENT = 5,
    SDE_CA = 6,
    WSA_CA = 7,
    CRL_SIGNER = 8,
}holder_type;

typedef enum holder_type_flags{
    FLAGS_SDE_ANONYMOUS = 1<<0,
    FLAGS_SDE_IDENTIFIED_NOT_LOCALIZED = 1<<1,
    FLAGS_SDE_IDENTIFIED_LOCALIZED = 1<<2,
    FLAGS_SDE_ENROLMENT = 1<<3,
    FLAGS_WSA = 1<<4,
    FLAGS_WSA_ENROLMENT = 1<<5,
    FLAGS_SDE_CA = 1<<6,
    FLAGS_WSA_CA = 1<<7,
    FLAGS_CRL_SIGNER = 1<<8,
}holder_type_flags;

typedef enum certificate_content_flags{
    USE_START_VALIDITY = 1<<0,
    LIFETIME_IS_DURATION = 1<<1,
    ENCRYPTION_KEY = 1<<2,
}certificate_content_flags;

typedef enum content_type{
    UNSECURED = 0,
    SIGNED = 1,
    ENCRYPTED = 2,
    CERTIFICATE_REQUEST = 3,
    CERTIFICATE_RESPONSE = 4,
    ANOYMOUS_CERTIFICATE_RESPONSE = 5,
    CERTIFICATE_REQUSET_ERROR = 6,
    CONTENT_TYPE_CRL_REQUEST = 7,
    CRL = 8,
    SIGNED_PARTIAL_PAYLOAD = 9,
    SIGNED_EXTERNAL_PAYLOAD = 10,
    SIGNED_WSA = 11,
    CERTIFACATE_RESPONSE_ACKNOWLEDGMENT = 12,
}content_type;

typedef enum tbsdata_flags{
    USE_GENERATION_TIME = 1<<0,
    EXPIRES = 1<<1,
    USE_LOCATION = 1<<2,
    EXTENSIONS = 1<<3
}tbsdata_flags;

typedef struct time64_with_standard_deviation{
    time64 time;
    u8 long_std_dev;
}time64_with_standard_deviation;

/*tbsdata_extension    需要释放 2
*/
typedef struct tbsdata_extension{
    tbsdata_extension_type type;
    ARRAY(u8,value);
}tbsdata_extension;

typedef struct three_d_location{
    s32 latitude;
    s32 longitude;
    u8 elevation[2];
}three_d_location;

typedef struct hashedid8{
    u8 hashedid8[8];
}hashedid8;

typedef enum pk_algorithm{
    ECDSA_NISTP224_WITH_SHA224 = 0,
    ECDSA_NISTP256_WITH_SHA256 = 1,
    ECIES_NISTP256 =2,
}pk_algorithm;
/*
 *elliptic_curve_point 需要释放  3
 *
 *
 */
typedef struct elliptic_curve_point{
    ecc_public_keytype type;
    ARRAY(u8,x);
    union{
        ARRAY(u8,y);
    }u;
}elliptic_curve_point;
/*
 *ecdsa_signature ，需要释放 4
 *
 */
typedef struct ecdsa_signature{
    elliptic_curve_point r;
    ARRAY(u8,s);
}ecdsa_signature;

/*
 *
 *signature 开始释放。   5
 *
 */
typedef struct signature{
    union{
        ecdsa_signature ecdsa_signature;
        ARRAY(u8,signature);
    }u;
}signature;

/*
 *
 *public-key 需要释放6
 *
 */
typedef struct public_key{
    pk_algorithm algorithm;
    union{
        elliptic_curve_point public_key;
        struct {
            symm_algorithm supported_symm_alg; 
            elliptic_curve_point public_key;
            ARRAY(u8,other_key);
        }ecies_nistp256;
    }u;
}public_key;

typedef struct two_d_location{
    s32 latitude;
    s32 longitude;
}two_d_location;

typedef two_d_location* polygonal_region;



typedef struct rectangular_region{
    two_d_location north_west;
    two_d_location south_east;
}rectangular_region;


typedef struct circular_region{
    two_d_location center;
    u16 radius;
}circular_region;
/*
 *需要释放     7
 */
typedef struct geographic_region{
    region_type region_type;
    union{
        circular_region circular_region;
        ARRAY(rectangular_region,rectangular_region);
        polygonal_region polygonal_region;
        ARRAY(u8,other_region);
    }u;
}geographic_region;
/*
 *需要释放 8
 */
typedef struct psid_priority{
    psid psid;
    u8 max_priority;
}psid_priority;

/*
 *需要释放 9
 */

typedef struct psid_priority_array{
    array_type type;
    union{
        ARRAY(psid_priority,permissions_list);
        ARRAY(u8,other_permissions);
    }u;
}psid_priority_array;
/*
 *需要释放 10
 */

typedef struct psid_array{
    array_type type;
    union{
        ARRAY(psid,permissions_list);
        ARRAY(u8,other_permissions);
    }u;
}psid_array;

/*
 *需要释放 11
 */




typedef struct psid_ssp{
    psid psid;
    ARRAY(u8,service_specific_permissions);
}psid_ssp;

typedef struct psid_ssp_array{
    array_type type;
    union{
        ARRAY(psid_ssp,permissions_list);
        ARRAY(u8,other_permissions);
    }u;
}psid_ssp_array;

typedef struct psid_priority_ssp{
    psid psid;
    u8 max_priority;
    ARRAY(u8,service_specific_permissions);
}psid_priority_ssp;

typedef struct psid_priority_ssp_array{
    array_type type;
    union{
        ARRAY(psid_priority_ssp,permissions_list);
        ARRAY(u8,other_permissions);
    }u;
}psid_priority_ssp_array;

typedef struct wsa_scope{
    u8* name;
    psid_priority_ssp_array permissions;
    geographic_region region;
}wsa_scope;

typedef struct anonymous_scope{
    ARRAY(u8,additionla_data);
    psid_ssp_array permissions;
    geographic_region region;
}anonymous_scope;

typedef struct identified_scope{
    u8* name;
    psid_ssp_array permissions;
    geographic_region region;
}identified_scope;

typedef struct identified_not_localized_scope{
    u8* name;
    psid_ssp_array permissions;
}identified_not_localized_scope;

typedef struct wsa_ca_scope{
    ARRAY(u8,name);
    psid_priority_array permissions;
    geographic_region region;
}wsa_ca_scope;

typedef struct sec_data_exch_ca_scope{
    ARRAY(u8,name);
    holder_type_flags permitted_holder_types;
    psid_array permissions;
    geographic_region region;
}sec_data_exch_ca_scope;

typedef struct root_ca_scope{
    ARRAY(u8,name);
    holder_type_flags permitted_holder_types;
    struct{
    }u;
}psid_priority_ssp_array;

typedef struct wsa_scope{
    u8* name;
    psid_priority_ssp_array permissions;
    geographic_region region;
}wsa_scope;

typedef struct anonymous_scope{
    ARRAY(u8,additionla_data);
    psid_ssp_array permissions;
    geographic_region region;
}anonymous_scope;

typedef struct identified_scope{
    u8* name;
    psid_ssp_array permissions;
    geographic_region region;
}identified_scope;

typedef struct identified_not_localized_scope{
    u8* name;
    psid_ssp_array permissions;
}identified_not_localized_scope;

typedef struct wsa_ca_scope{
    ARRAY(u8,name);
    psid_priority_array permissions;
    geographic_region region;
}wsa_ca_scope;

typedef struct sec_data_exch_ca_scope{
    ARRAY(u8,name);
    holder_type_flags permitted_holder_types;
    psid_array permissions;
    geographic_region region;
}sec_data_exch_ca_scope;

typedef struct root_ca_scope{
    ARRAY(u8,name);
    holder_type_flags permitted_holder_types;
    struct{
        psid_array secure_data_permissions;
        psid_priority_array wsa_permissions;
        ARRAY(u8,other_permissons);
    }flags_content;
    geographic_region region;
}root_ca_scope;

typedef struct cert_specific_data{
    union{
        root_ca_scope root_ca_scope;
        sec_data_exch_ca_scope sde_ca_scope;
        wsa_ca_scope wsa_ca_scope;
        crl_series* responsible_series;
        identified_not_localized_scope id_non_loc_scope;
        identified_scope id_scope;
        anonymous_scope anonymous_scope;
        wsa_scope wsa_scope;
        ARRAY(u8,other_scope);
    }u;
}cert_specific_data;

typedef struct tobesigned_certificate{
    holder_type holder_type;
    certificate_content_flags cf;
    union{
        struct{
            hashedid8 signer_id;
            pk_algorithm signature_alg;
        }no_root_ca;
    }u;
    cert_specific_data scope;
    time32 expiration;
    crl_series crl_series;
    union{
        public_key verification_key;
        ARRAY(u8,other_key_material);
    }verion_and_type;
    struct{
        certificate_duration lifetime;
        time32 start_validity;
        public_key encryption_key;
        ARRAY(u8,other_cert_content);
    }flags_content;
}tobesigned_certificate;

typedef struct certificate{
    u8 version_and_type;
    tobesigned_certificate unsigend_certificate;
    union{
        signature signature;
        elliptic_curve_point reconstruction_value;
        ARRAY(u8,signature_material);
    }u;
}certificate;

typedef struct signer_identifier{
    signer_identifier_type type;
    union{
        hashedid8 digest;
        certificate certificate;
        ARRAY(certificate,certificates);
        struct{
            pk_algorithm algorithm;
            hashedid8 digest;
        }other_algorithm;
        ARRAY(u8,id);
    }u;
} signer_identifier;

typedef struct crl_request{
    hashedid8 issuer;
    crl_series crl_series;
    time32 issue_date;
}crl_request;

typedef struct certid10{
    u8 certid10[10];
}certid10;

typedef struct id_and_date{
    certid10 id;
    time32 expiry;
}id_and_date;

typedef struct tobesigned_crl{
    crl_type type;
    crl_series crl_series;
    hashedid8 ca_id;
    u32 crl_serial;
    time32 start_period;
    time32 issue_date;
        certificate_duration lifetime;
        time32 start_validity;
        public_key encryption_key;
        ARRAY(u8,other_cert);
    }flags_content;
    public_key verification_key;
    public_key response_encryption_key;
}tobesigned_certificate_request;

typedef struct certificate_request{
    signer_identifier signer;
    tobesigned_certificate_request unsigned_csr;
    signature signature;
}certificate_request;

typedef struct tobesigned_data{
   tbsdata_flags tf;
   union{
        struct {
            psid psid;
            ARRAY(u8,data);
        }type_signed,type_signed_partical;
        psid psid;
        ARRAY(u8,data);
   }u;
   struct {
        time64_with_standard_deviation generation_time;
        time64 exipir_time;
        three_d_location generation_location;
        ARRAY(tbsdata_extension,extensions);
        ARRAY(u8,other_data);
   }flags_content;
}tobesigned_data;


typedef struct signed_data{
    struct signer_identifier signer;
    struct tobesigned_data unsigned_data;
    struct signature signature;
}signed_data;

typedef struct tobe_encrypted{
    content_type type;
    union{
        ARRAY(u8,plain_text);
        signed_data signed_data;
        certificate_request request;
        tobe_encrypted_certificate_response 
                response;
        tobe_encrypted_anonymous_cert_response 
                anon_response;
        tobe_encrypted_certificate_request_error
                request_error;
        crl_request crl_request;
        crl crl;
        tobe_encrypted_certificate_response_acknowledgment
                ack;
        ARRAY(u8,data);
    }u;
}tobe_encrypted;

typedef struct aes_ccm_ciphertext{
    u8 nonce[12];
    ARRAY(u8,ccm_ciphertext);
}aes_ccm_ciphertext;


typedef struct ecies_nist_p256_encrypted_key{
    elliptic_curve_point  v;
    ARRAY(u8,c);
    u8 t[20];
}ecies_nist_p256_encrypted_key;

typedef struct recipient_info{
    hashedid8 cert_id;
    union{
        ecies_nist_p256_encrypted_key enc_key;
        ARRAY(u8,other_enc_key);
    }u;
}recipient_info;


typedef struct encrypted_data{
    symm_algorithm symm_algorithm;
    ARRAY(recipient_info,recipients);
    union{
        aes_ccm_ciphertext ciphertext;
        ARRAY(u8,other_ciphertext);
    }u;
}encrypted_data;

typedef struct tobesigned_wsa{
    ARRAY(u8,permission_indices);
    tbsdata_flags tf;
    ARRAY(u8,data);
    time64_with_standard_deviation generation_time;
    time64 expire_time;
    three_d_location generation_location;
    struct{
        ARRAY(tbsdata_extension,extension);
        ARRAY(u8,other_data);
    }flags_content;
}tobesigned_wsa;

typedef struct signed_wsa{
    signer_identifier signer;
    tobesigned_wsa unsigned_wsa;
    signature signature;
}signed_wsa;





typedef struct sec_data{
	u8 protocol_version;
	content_type type;
	union
	{
		ARRAY(u8,data);
		struct signed_data signed_data;//（content_type)
		struct signed_wsa signed_wsa;
		struct encrypted_data encrypted_data;
		struct crl_request crl_request;
		struct crl crl;
        ARRAY(u8,other_data);
	}u;
}sec_data;
#endif
