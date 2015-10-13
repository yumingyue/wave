#include"cmp.h"
#include<pthread.h>
#include<stddef.h>
#include "../utils/common.h"
#include "../cme/cme_db.h"
#include <stdlib.h>
#include <time.h>
#define forward 60  //单位是s

struct crl_req_time{
    struct list_head list;
    time64 time;//单位ns
};
struct wsa_cert_db{
    struct list_head list;
    cmh cmh;
};

struct cmp_db{
    struct crl_req_time* crl_time;
    struct wsa_cert_db certs;
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

static void certificate_request_progress(struct cmp_db* cmdb){

}
static void crl_request_progress(struct cmp_db* cmdb){

}
static void data_recieve_progress(struct cmp_db* cmdb){

}
void cmp_run(){
    while(1){
        pthread_mutex_lock(&cmdb->lock);
        while(cmdb->pending == 0)
            pthread_cond_wait(&pending_cond,&cmdb->lock);

        if(cmdb->pending & CRL_REQUEST ){
            crl_request_progress(cmdb);
            cmdb->pending &= ~CRL_REQUEST;
        }
        if(cmdb->pending & CERTFICATE_REQUEST){
            certificate_request_progress(cmdb);
            cmdb->pending &= ~CERTFICATE_REQUEST;
        }
        if(cmdb->pending & RECIEVE_DATA){
            data_recieve_progress(cmdb);
            cmdb->pending &= ~RECIEVE_DATA;
        }
        pthread_mutex_unlock(&cmdb->lock);
    }
}

