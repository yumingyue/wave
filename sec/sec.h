#ifndef SEC_H
#define SEC_H
#include"../pssme/pssme_db.h"
#include"../cme/cme_db.h"

struct sec_db{
    struct cme_db cme_db;
    struct pssme_db pssme_db;
};

#endif 
