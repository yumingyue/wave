#ifndef DEBUG_H
#define DEBUG_H


#define HASHEDID8_FORMAT %x %x %x %x %x %x %x %x
#define HASHEDID8_VALUE(n) n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7] 
enum{
  MSG_DEBUG,MSG_INFO,MSG_WARNING,MSG_ERROR  
};

extern int wave_debug_level;
void wave_printf(int level,const char *fmt,...);
#endif
