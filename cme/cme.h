#ifndef CME_H
#define CME_H
#include"cme_db.h"
#include"../utils/common.h"

enum identifier_type{
    ID_CERTIFICATE = 0,
    ID_HASHEDID8 = 1,
    ID_CERTID10 = 2,
};
enum permissions_type{
    PSID = 0,
    PSID_PRIORITY = 1,
    PSID_SSP = 2,
    PSID_PRIORITY_SSP = 3,
    INHERITED_NOT_FOUND = 4,
};
struct cme_permissions{
    enum permissions_type type;
    union{
        ARRAY(psid,psid_array);
        ARRAY(psid_priority,psid_priority_array);
        ARRAY(psid_ssp,psid_ssp_array);
        ARRAY(psid_priority_ssp,psid_priority_ssp_array);
    }u;
};

result cme_lsis_request(struct cme_db* cdb,cme_lsis* lsis);

result cme_cmh_request(struct cme_db* cdb,cmh* cmh);

result cme_generate_keypair(struct cme_db* cdb,const cmh cmh,
                        const pk_algorithm algorithm,
                        string* pub_key);

result cme_store_keypair(struct cme_db* cdb,const cmh cmh,
                            const pk_algorithm algorithm,
                            const string* pub_key,
                            const string* pri_key);

result cme_store_cert(struct cme_db* cdb,const cmh cmh,
                            const certificate cert,
                            const u32 a,const u32 b);
result cme_store_cert_key(struct cme_db* cdb,const certificate cert,
                            const string* pri_key);



/**
 * 从cme中获取相关证书的信息
 * @type：证书索引的类型
 * @identifier：证书的索引标示。
 * @certificate:证书编码好的字节流。
 *
 * @permissions：相关权限内容
 * @scope：地理位置权限。
 * @last_crl_time:上次收到crl的时间。
 * @next_crl_time:希望下次收到crl的时间。
 * @trust_anchor:这个证书是否是信任卯（ca就是一个，就自己签发自己而且大家公认的）
 * @verified:证书是否验证过的
 */
result cme_certificate_info_request(struct cme_db* cdb, 
                    enum identifier_type type,
                    string *identifier,
                    string *certificate,

                    struct cme_permissions* permissions,
                    geographic_region* scope,
                    time64* last_crl_time,time64* next_crl_time,
                    bool* trust_anchor,bool* verified);

/**
 * 添加信任卯的证书.
 * @cert:需要添加的证书，但是这里记住，我们在这个函数内部，要复制这个证书，存起来，
 *              因为这个证书应该要有外部释放其内存的。
 */
result cme_add_trust_anchor(struct cme_db* cdb,certificate* cert);

/**
 *添加普通证书
 *@cert：添加的证书。
 *@verified：是否验证过。
 */
result cme_add_certificate(struct cme_db* cdb,
                            certificate* cert,bool verified);

result cme_delete_certificate(struct cme_db* cdb,cmh cmh);

/*
 * 添加撤销的消息
 * @identifier：相关的证书的certid10标示。
 * @ca_id:签发这个证书的certid8标示。
 * @series：对应证书的serires。
 * @expiry：过期的时间。
 */
result cme_add_certificate_revocation(struct cme_db* cdb,
                             string* identifier,
                             string* ca_id,
                             crl_series series,
                             time64 expiry);
/**
 * 增加或者更新一个crlinfo
 */
void cme_add_crlinfo(struct cme_db* cdb,
                        crl_type crl_type,
                        crl_series series,
                        string* ca_id,
                        u32 serial_number,
                        time64 start_period,
                        time64 issue_date,
                        time64 next_crl_time
                        );
/**
 * 提取相关的crl信息。
 */
result cme_get_crlinfo(struct cme_db* cdb,
                        crl_series series,
                        string* ca_id,
                        u32 serial_numberi,
                        
                        crl_type *type,
                        time64 *start_time,
                        time64 *issue_data,
                        time64 *next_crl_time);

#endif
