#ifndef COMMON_H
#define COMMON_H
#include <endian.h>

typedef enum result{
    SUCCESS = 0,
    FAILURE = 1,
    UNRECOGNIZED_LSIS = 2,
    INVAID_INPUT = 3,
    NOT_MATCH = 4,
    CERTIFICATE_NOT_FOUND = 6,
    CERTIFICATE_REVOKED = 7,
    CERTIFICATE_NOT_TRUSTED = 8,
    CERTIFICATE_BADLY_FORMED = 9,
    TOO_MANY_ENTRIES_IN_PERMISSON_ARRAY = 10,
    UNSUPPORTED_REGION_TYPE_IN_CERTIFICATE = 11,
    TOO_MANY_ENTRIES_IN_POLYGONAL_GEOGRAPHIC_SCOPE = 12,
    IMPLICT_CERTIFICATE_NOT_ACCEPTABLE_AS_TRUST_ANCHOR = 13,
    UNRECOGNIZED_IDENTIFIER = 14,
    EXPIRED = 15,
    NOT_ISSUED_YET = 16,
    MISSING = 17,
}result;

#define be_to_host16(n) be16toh(n)
#define be_to_host32(n) (__force u32)be32toh(n)//
#define be_to_host64(n) (__force u64)be64toh(n)
#define host_to_be16(n) (__force u16)htobe16(n)
#define host_to_be32(n) (__force u32)htobe32(n)
#define host_to_be64(n) (__force u64)htobe64(n)

/**这里的兼容行不好，后面改成系统的那写uint8**/
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef u8 bool;
#define false 0
#define true 1

#ifdef __GNUC__
#define STRUCT_PACKED __attribute__ ((packed))
#else
#define STRUCT_PACKED

#endif/*GNUC*/
#endif /*COMMON_H*/
