#ifndef SEC_H
#define SEC_H
#include"../pssme/pssme.h"
#include"../cme/cme.h"

struct sec_db{
    struct cme_db cme_db;
    struct pssme_db pssme_db;
};



#endif 
