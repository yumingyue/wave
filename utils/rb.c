#include"rb.h"
#include<stdlib.h>
struct a{
    int value;
    struct rb_head tree;
};
int compera(struct rb_head* a,struct rb_head* b){
    int  aa,bb;
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
    printf("%d %d\n",aa,b);
    if(aa < b)
        return -1;
    if(aa == b)
        return 0;
    return 1;
}
struct a* init(int value){
    struct a* a = (struct a*)malloc(sizeof( struct a));
    a->value = value;
    rb_init(&a->tree,compera,equal);
    return a;
}
void main(){
    struct a* root;
    root = NULL;
    int count = 1;
    int i;
    for(i=0;i<20;i++){
        struct a* b;
        b = init(count++);
        root = rb_entry( rb_insert(root==NULL?NULL:&root->tree,&b->tree),struct a,tree);
    }
    int value = 5;
    struct a* node = rb_entry( rb_find( &(root->tree),&value),struct a,tree);
    printf("node %d\n",node->value);
    root = rb_entry( rb_delete(&root->tree,&node->tree),struct a,tree);
    printf("root :%ld\n",&root->tree);
    struct rb_head* point = rb_find( &(root->tree),&value);
    
    //node = rb_entry( rb_find( &(root->tree),&value),struct a,tree);
   printf("node %ld\n",point);

}
