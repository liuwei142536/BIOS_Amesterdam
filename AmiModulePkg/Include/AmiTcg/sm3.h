#ifndef _SM3_H_
#define _SM3_H_

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>

#ifndef __size_t__
#define __size_t__
typedef UINTN           size_t;
#endif // __size_t__

# define SM3_WORD unsigned int
# define SM3_CBLOCK      64
# define SM3_LBLOCK      (SM3_CBLOCK/4)

typedef struct SM3state_st {
   SM3_WORD A, B, C, D, E, F, G, H;
   SM3_WORD Nl, Nh;
   SM3_WORD data[SM3_LBLOCK];
   unsigned int num;
} SM3_CTX;

int sm3_init(SM3_CTX *c);
int sm3_update(SM3_CTX *c, const void *data, size_t len);
int sm3_final(unsigned char *md, SM3_CTX *c);

#endif
