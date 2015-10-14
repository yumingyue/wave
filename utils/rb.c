#include"rb.h"

struct a{
    int value;
    struct rb_head tree;
};
int compera(struct rb_head* a,struct rb_head* b){
    int value aa,bb;
    aa = rb_entry(a,struct a,tree)->value;
    bb = rb_entry(b,struct a,tree)->value;
    if(aa < bb)
        return -1;
    if(aa == bb)
        return 0;
    return 1;
}
int equal(struct rb_head* a,void* value){
    int b = *(int*)value;
    int aa = rb_entry(a,struct a,tree)->value;
    if(aa < b)
        return -1;
    if(aa == b)
        return 0;
    return 1;
}
struct a* init(int value){
    struct a* a = (struct a*)malloc(struct a);
    a->value = value;
    rb_init(&a->tree,)
}
void main(){
    struct a* root;
    root = NULL;
    int count = 1;
    root = (struct a*)root
}
