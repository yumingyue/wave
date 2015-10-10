#ifndef PSSME_CB_H
#define PSSME_CB_H

#include"pssme.h"
#include"../data/data.h"
struct psid_priority_ssp{
    string psid;
    priority priority;
    string ssp;
};
struct psid_priority_ssp_array{
    struct psid_priority_ssp permission;
    struct list_head list;
}
struct alloc_lsis{
    struct list_head list;
    lsis lsis;
    struct psid_priority_ssp_array
            permissions;
};
struct lsis_chain{
    u32 num;
    struct list_head list;
};
struct lsis_cb{
    struct alloc_lsis alloc_lsis;
    struct lsis_chain lsises;
};
struct lsis_array{
    struct list_head list;
    lsis lsis;
};
struct local_cert{
    struct list_head list;
    certificate certificate;
    struct lsis_array lsis_array;
};
struct receive_cert{
    struct rb_head rb_head;//根据证书里面什么？？比较
    certificate certificate;
    u64 recent_time;//最新受到的这个证书的数据的时间
};
struct cert_cb{
    struct local_cert local_cert;
    struct receive_cert receive_cert;
};
struct pssme_cb{
    struct cert_cb cert_cb;
    struct lsis_cb lsis_cb;
}
#endif
