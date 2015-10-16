#ifndef SEC_H
#define SEC_H
#include"../pssme/pssme_db.h"
#include"../cme/cme_db.h"
#include"../cme/cme.h"
#include"../pssme/pssme.h"

struct sec_db{
    struct cme_db cme_db;
    struct pssme_db pssme_db;
};

enum sign_with_fast_verification{
    YES_UNCOMPRESSED = 0,
    YES_COMPRESSED = 1,
    NO = 2,
};
/**
 * 签名数据
 *
 * @type：只能是signed signed_partial_payload signed_external_payload,其他返回错误。
 * @data:需要签名的data，编码好了的。
 * @exter_data:同上。
 * @psid:填写data的psid字段
 * @ssp：服务特殊权限，
 * @set_generation_time:是否要填写generation_time字段，
 * @generation_time:对应时间,这个结构题里面有，时间误差，填写255代表不知道。
 * @set_generation_location: 是否填写generation_location字段，
 * @generation_location:位置信息.
 * @set_expiry_time:是否要填写expiry_time字段
 * @expiry_time:过期时间。
 * @signer_type:签发这个数据的签名类型，
 * @cert_chain_len:如果是证书连，相关的长度，这个说明请看协议。
 * @cert_chain_max_len:证书连最长证书。
 * @fs_type:快速认证类型，
 * @compressed：ec点是否压缩
 *
 * @signed_data:填写好了数据，编码,输入到signed_data.
 * @len_of_cert_chain:该证书连的长度。
 */
result sec_signed_data(struct sec_db* sdb,
                content_type type,string* data,
                string* exter_data,psid psid,
                string* ssp,
                bool set_generation_time, 
                time64_with_standard_deviation* generation_time,
                bool set_generation_location,
                three_d_location* location,
                bool set_expiryt_time,
                time64 expire_time,
                signer_identifier_type signer_type,
                s32 cert_chain_len,
                u32 cert_chain_max_len,
                enum sign_with_fast_verification fs_type,
                bool compressed,
                
                string* signed_data,
                u32 len_of_cert_chain);


;

/*
 * 加密数据，参数参考协议
 */
result sec_encrypted_data(struct sec_db* sdb,
                content_type type,
                string* data,
                struct certificate_chain* certs,
                bool compressed,
                u64 time,
                
                string* encrypted_data,
                struct certificate_chain* failed_certs);


result sec_secure_data_content_extration(struct sec_db* sdb,
                string* sdata,cmh cmh,
                
                content_type *type,
                content_type *inner_type,
                string* data,
                string* signed_data,
                psid *psid,
                struct cme_permissions  *permissions,
                bool* set_generation_time,
                time64_with_standard_deviation *generation_time,
                bool* set_expiry_time,
                time64 *expiry_time,
                bool* set_generation_location,
                three_d_location *location,
                certificate* send_cert);

/**
 * 验证数据的签名，
 * 参数参考协议.
 */
result sec_signed_data_verification(struct sec_db* sdb,
                cme_lsis lsis,
                psid psid,
                content_type type,
                string* signed_data,
                string* external_data,
                u32 max_cert_chain_len,
                bool detect_reply,
                bool check_generation_time,
                time64 validity_period,
                time64_with_standard_deviation* generation_time,
                float generation_threshold,
                time64 accepte_time,
                float accepte_threshold,
                bool check_expiry_time,
                time64 exprity_time,
                float exprity_threshold,
                bool check_generation_location,
                two_d_location* location,
                u32 validity_distance,
                three_d_location* generation_location,
                time64 overdue_crl_tolerance);
/**
 * crl的签名验证  时间单位秒
 * 
 */
result sec_crl_verification(struct sec_db* sdb,string* sec_data,time32 overdue_crl_tolerance,
                        
                time32* last_crl_time,
                time32* next_crl_time,
                certificate* cert);

enum transfer_type{
    EXPLICT = 1,
    IMPLICT = 2,
};
/*
 * 生成一个证书请求报,时间单位是秒
 * 参数请看协议
 */
result sec_get_certificate_request(struct sec_db* sdb,signer_identifier_type type,
                cmh cmh,
                holder_type cert_type,
                enum transfer_type transfer_type,
                struct cme_permissions* permissions,
                string* identifier,
                geographic_region* region,
                bool start_validity,
                bool life_time_duration,
                time32 start_time,
                time32 expiry_time,
                string* veri_pub_key,
                string* enc_pub_key,
                string* respon_enc_key,
                certificate* ca_cert,
                
                string* cert_request,
                certid10* request_hash);

result sec_certificate_response_processing(struct sec_db* sdb,
                cmh cmh,
                string* data,
                
                content_type* type,
                certid10 *request_hash,
                certificate_request_error_code* error,
                certificate* certificate,
                string* rec_value,
                bool* ack_request
                );

result sec_signed_wsa(struct sec_db* sdb,
                string* data,
                serviceinfo_array* permissions,
                time32 life_time,

                string* signed_wsa);

result sec_signed_wsa_verification(struct sec_db* sdb,
                string* wsa,
                
                string* wsa_data,
                psid_priority_ssp_array* permissions,
                time64_with_standard_deviation* generation_time,
                time64 *expiry_time,
                three_d_location* location,
                time64 *last_crl_time,
                time64 *next_crl_time,
                certificate* certificate);


result sec_check_certificate_chain_consistency(
                struct sec_db* sdb,
                struct certificate_chain* cert_chain,
                struct cme_permissions_array* permission_array,
                geographic_region* region);

result sec_check_chain_psids_consistency(struct sec_db* sdb,
                struct cme_permissions_array* permission_array);

result sec_check_chain_psid_priority_consistency(struct sec_db* sdb,
                struct cme_permissions_array* permission_array);

result sec_check_chain_geographic_consistency(struct sec_db* sdb,
                struct geographic_region_array* regions);

result sec_verify_chain_signature(struct sec_db* sdb,
                struct certificate_chain* cert_chain,
                bool* verified_array,u32 len,
                string* digest,
                signature* signature);

result sec_decrypt_data(struct sec_db* sdb,
                string* encrypted_data,
                cmh cmh,
                
                content_type* type,
                string* data);

result sec_certificate_request_error_verification(struct sec_db* sdb,
                tobe_encrypted_certificate_request_error* cert_req_error);

result sec_certificate_response_verification(struct sec_db* sdb,
                tobe_encrypted_certificate_response* cert_resp);

/***************这后面的函数都是certificate的一些帮助接口，方便获取证书的相关信息******/

static int inline hashedid8_compare(hashedid8* a,hashedid8* b){
    u8 *ptra,*ptrb;
    ptra = (u8*)(&a->hashedid8);
    ptrb = (u8*)(&b->hashedid8);
    int i;
    for(i=0;i<8;i++){
        if(*ptra != *ptrb)
            return 1;
        ptra++;
        ptrb++;
    }
    return 0;
}
static int inline hashedid8_cpy(hashedid8* dst,hashedid8* src){
    int i=0;
    for(i=0;i<8;i++){
        dst->hashedid8[i] = src->hashedid8[i];
    }  
}
#endif 
