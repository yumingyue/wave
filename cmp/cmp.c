#include"cmp.h"
#include<pthread.h>
#include<stddef.h>
#include "../utils/common.h"
#include "../cme/cme.h"
#include "../pssme/pssme.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define forward 60  //单位是s
#define INIT(m) memset(&m,0,sizeof(m))
struct crl_req_time{
    struct list_head list;
    time64 time;//单位ns
};
struct wsa_cert_db{
    struct list_head list;
    cmh cert_cmh;
    cmh key_pair_cmh;
};

struct cmp_db{
    struct crl_req_time* crl_time;
    struct wsa_cert_db certs;
    cmh req_cert_cmh;
    cmh req_cert_enc_cmh;
    cmh ca_cmh;
    certificate ca_cert;
    string identifier;
    geographic_region geographic_region;
    u32 life_time;//单位day
    u32 pending;
    pthread_mutex_t lock;
};

enum pending_flags{
    CRL_REQUEST = 1<<0,
    CERTFICATE_REQUEST = 1<<1,
    RECIEVE_DATA = 1<<2,
};

pthread_cond_t pending_cond = PTHREAD_COND_INITIALIZER;
struct cmp_db* cmdb = NULL;

static void pending_crl_request(struct cmp_db* cmdb){
    pthread_mutex_lock(&cmdb->lock);
    cmdb->pending |= CRL_REQUEST;
    pthread_mutex_unlcok(&cmdb->lock);

    pthread_cond_signal(&pending_cond);
}
static void crl_alarm_handle(int signo){
    if(signo == SIGALARM){
        set_crl_request_alarm(cmdb)
        pending_crl_request(cmdb);
    }
}
static void set_crl_request_alarm(struct cmp_db* cmdb){
    time_t now,diff;
    time64 next_time;
    struct crl_req_time* crl;
    time(&now);
    pthread_mutex_lock(&cmdb->lock);
    crl = cmdb->crl_time;
    do{
        if(crl == NULL){
            pthread_mutex_unlock(&cmdb->lock);
            return;
        }
        next_time = crl->time /1000000;
        //删除节点
    }while(next_time -forward < now )
    pthread_mutex_unlokc(&cmdb->lock);

    signal(SIGALRM,crl_alarm_handle);
    alarm(now - next_time + forward);
} 
static void pending_certificate_request(struct cmp_db* cmdb){
    pthread_mutex_lock(&cmdb->lock);
    cmdb->pending |= CERTFICATE_REQUEST;
    pthread_mutex_unlock(&cmdb->lock);

    pthread_cond_signal(&pending_cond);
}
static void pending_recieve_data(struct cmp_db* cmdb){
    pthread_mutex_lock(&cmdb->lock);
    cmdb->pending |= RECIEVE_DATA;
    pthread_mutex_unlcok(&cmdb->lock);

    pthread_cond_signal(&pending_cond);
}
void cmp_do_certificate_applycation(){
    pending_certificate_request(cmdb);
}
void cmp_do_recieve_data(){
    pending_recieve_data(cmdb);
}

u32 cmp_init(){
    cmdb = (struct cmp_db*)malloc(sizeof(struct cmp_db));
    if(cmdb == NULL)
        return -1;
    cmdb->pending = 0;
    pthread_mutex_init(&cmdb->lock,NULL); 
    //还有两个没从初始化
    
    return 0;
}
static int generate_cert_request(struct sec_db* sdb,struct cmp_db* cmdb,cme_lsis lsis,
                            string* veri_pk,string* enc_pk,string* res_pk,
                            string* data,certid10* request_hash){
    serviceinfo_array serviceinfos;
    struct cme_permissions permissions;
    psid_priority_ssp* pps;
    int i;
    INIT(serviceinfos);
    INIT(permissions);

    if(pssme_get_serviceinfo(&sdb->pssme_db,lsis,&serviceinfos))
        goto fail;

    permissions.type = PSID_PRIORITY_SSP;
    permissions.u.psid_priority_ssp_array.psid_priority_ssp =
        (struct psid_priority_ssp*)malloc(sizeof(struct psid_priority_ssp) * serviceinfos.len);
    permissions.psid_priority_ssp_array.len = serviceinfos.len;
    pps = permissions.u.psid_priority_ssp_array.psid_priority_ssp;

    if(pps == NULL)
        goto fail;
    for(i=0;i<serviceinfos.len;i++){
        (pps +i)->psid =
            (serviceinfos.serviceinfos + i)->psid;
        (pps +i)->max_priority = 
            (serviceinfos.serviceinfos + i)->max_priority;
        (pps+i)->service_specific_permissions.len = 
            (serviceinfos.serviceinfos + i)->len;

        (pps +i)->service_specific_permissions.buf = 
            (u8*)malloc( (serviceinfos.serviceinfos+i)->len) ;
        if((pps + i)->service_specific_permissions.buf == NULL)
            goto fail;
        memcpy(pps +i)->service_specific_permissions.buf
                (serviceinfos.serviceinfos+i)->buf,(serviceinfos.serviceinfos+i)->len);
    }
    
    time_t now;
    time(&now);
    time32 expire  = now + cmdb.life_time*24*60*60;
    pthread_mutex_lock(&cmdb->lock);
    if(sec_get_certificate_request(&sdb->cme_db,CERTIFICATE,cmdb->ca_cmh,WSA,
                            IMPLICT,&permissions,&cmdb->identifier,&cmdb->geographic_region,
                           true,true,now,expire,veri_pk,enc_pk,res_pk,data,request_hash)){
        pthread_mutex_unlock(&cmdb_lock);
        goto fail;
    }
    pthread_mutex_lock(&cmdb->lock);
    cme_permissions_free(&permissions);
    serviceinfo_array_free(&permissions);
    return 0;
fail:
    cme_permissions_free(&permissions);
    serviceinfo_array_free(&permissions);
    return -1;
}
static void certificate_request_progress(struct cmp_db* cmdb,struct sec_db* sdb){
    cmh cert_cmh,key_pair_cmh;
    string cert_pk,keypair_pk;
    cme_lsis lsis = -1;
    string data;
    certid10 resquest_hash;
    int i;
    INIT(cert_pk);
    INIT(keypair_pk);
    INIT(data);
    INIT(resquest_hash);

    if(cme_cmh_request(&sdb->cme_db,&cert_cmh) || cme_cmh_request(&sdb->cme_db,&key_pair_cmh))
        goto fail;
    if(cme_generate_keypair(&sdb->cme_db,cert_cmh,ECDSA_NISTP256_WITH_SHA256,&cert_pk)||
            cme_generate_keypair(&sdb->cme_db,key_pair_cmh,ECIES_NISTP256,&keypair_pk))
        goto fail;
    if(generate_cert_request(sdb,cmdb,lsis,&cer_pk,NULL,&keypair_pk,&data,&resquest_hash))
        goto fail;
    ca_write(data->buf,data->len);
    pthread_mutex_lock(&cmdb.lock);
    cmdb.req_cert_cmh = cert_cmh;
    cmdb.req_cert_enc_cmh  = key_pair_cmh;
    pthread_mutex_unlock(&cmdb.lock);
    
    string_free(&cert_pk);
    string_free(&keypair_pk);
    string_free(&data);
    certid10_free(&resquest_hash);
    return;

fail:
    string_free(&cert_pk);
    string_free(&keypair_pk);
    string_free(&data);
    certid10_free(&resquest_hash);
    return ;
    
}
static void crl_request_progress(struct cmp_db* cmdb){

}
static void data_recieve_progress(struct cmp_db* cmdb){

}
void cmp_run(struct sec_db* sdb){
    while(1){
        pthread_mutex_lock(&cmdb->lock);
        while(cmdb->pending == 0)
            pthread_cond_wait(&pending_cond,&cmdb->lock);

        if(cmdb->pending & CRL_REQUEST ){
            crl_request_progress(cmdb);
            cmdb->pending &= ~CRL_REQUEST;
        }
        if(cmdb->pending & CERTFICATE_REQUEST){
            certificate_request_progress(cmdb,sdb);
            cmdb->pending &= ~CERTFICATE_REQUEST;
        }
        if(cmdb->pending & RECIEVE_DATA){
            data_recieve_progress(cmdb);
            cmdb->pending &= ~RECIEVE_DATA;
        }
        pthread_mutex_unlock(&cmdb->lock);
    }
}

