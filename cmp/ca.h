#ifndef CA_H
#define CA_H

void ca_write(u8* buf,u32 len);
u32 ca_try_read(u8* buf,u32 len);
u32 ca_read(u8* buf,u32 len);
#endif
