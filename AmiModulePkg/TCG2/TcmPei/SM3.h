/*zi203.h*/
#ifndef __SCH_H__
#define __SCH_H__


typedef struct {
    unsigned long	state[8];
	unsigned long	length;
	unsigned long	curlen;
    unsigned char	buf[64];
} SM3_STATE;

void SM3Hash(unsigned char *buf, unsigned int len, unsigned char *hash) ;
void SM3_init(SM3_STATE *md);
void SM3_done(SM3_STATE *md, unsigned char *hash);
void SM3_process(SM3_STATE * md, unsigned char *buf, unsigned int len);
#endif


