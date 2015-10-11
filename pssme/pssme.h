#ifndef PSSME_H
#define PSSME_H

#include "pssme_db.h"
#include "../cme/cme_db.h"
typedef enum result{
    SUCCESS = 0,
    FAILURE = 1,
    UNRECOGNIZED_LSIS = 2,
    INVAID_INPUT = 3,
    NOT_MATCH = 4,
}result;

typedef enum action{
    ADD = 1,
    DELETE = 2,
}action;


typedef struct serviceinfo{
    pssme_lsis lsis;
    string psid;
    priority max_priority;
    string ssp;
}serviceinfo;

typedef struct serviceinfo_array{
    serviceinfo* serviceinfos;
    u32 len;
}serviceinfo_array;

typedef struct lsis_array{
    pssme_lsis* lsis;
    u32 len;
}lsis_array;

/**
 * 申请一个pssme lsis;
 */
pssme_lsis pssme_lsis_request(struct pssme_db* pdb;);
/**
 * 向pssme注册或者撤销一个服务相关的东西;
 * @lsis：该实体标示。
 * @action：是删除还是添加
 * @psid：该实体的psid
 * @priority：该实体的priority
 * @ssp：该实体的ssp；
 * return 成功或者失败
 */
result pssme_secure_provider_serviceinfo(struct pssme_db* pdb,pssme_lsis lsis,action action,
                   string* psid,priority priority,string* ssp);
/**
 *获取该lsis实体的服务信息
 *@lsis：该实体的表示，如果为-1 表示获取所有实体的。
 *@se_array:如果成功，把相关的信息填写进去;
 *@max_len:这个se_array最多可以放多少个serviceinfo
 *return：服务信息的集合。
 *注意：这个函数不能进行内存分配，我们认为这个数组已经分配好了。我们之负责填充
 */
result pssme_get_serviceinfo(struct pssme_db* pdb,
                    cme_lsis lsis,serviceinfo_array* se_array,u32 max_len);

/**
 * 将证书和lsis绑定起来
 * @cmh：证书索引。
 * @liss_array:实体标示集合。
 **/
result pssme_cryptomaterial_handle_storage(struct pssme_db* pdb,
                cmh cmh,lsis_array* lsis_array);

/**
 * 检测这个证书，受到的time是否是最新。
 * @generation_time:wsa受到的时间。
 * @cert：对应的证书.
 */
result pssme_outoforder(struct pssme_db* pdb,
                u64 generation_time,certificate* cert);



#endif

