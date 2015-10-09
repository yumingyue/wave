#ifndef STRING_H
#define STRING_H
#include "common.h"
struct string{
    u8 *buf;
    u16 len;
}
/**
 * 分配一个string的指针，并且buf分配指定长度
 */
string * string_malloc(u16 len);
/**
 * 释放一个string的指针
 */
void string_free(string *str);
#endif /*STRING_H*/
