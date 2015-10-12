#include "pssme.h"
#include<stdlib.h>
void serviceinfo_array_free(serviceinfo_array* point){
    if(point->serviceinfos == NULL)
        return;
    free(point->serviceinfos);
    point->serviceinfos = NULL;
    point->len = 0;
};

