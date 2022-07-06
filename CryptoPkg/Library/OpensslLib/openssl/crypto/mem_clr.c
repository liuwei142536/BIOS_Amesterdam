//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/*
 * Copyright 2002-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include <openssl/crypto.h>

/*
 * Pointer to memset is volatile so that compiler must de-reference
 * the pointer and can't assume that it points to any function in
 * particular (such as memset, which it then might further "optimize")
 */
//APTIOV Override starts - To avoid build error with 32-bit build
//typedef void *(*memset_t)(void *, int, size_t); 

//static volatile memset_t memset_func = memset;	
//APTIOV Override ends
void OPENSSL_cleanse(void *ptr, size_t len)
{
    //memset_func(ptr, 0, len);	      //APTIOV Override starts - To avoid build error with 32-bit build          
	SetMem(ptr,len,0);	               //APTIOV Override starts - To avoid build error with 32-bit build              
}
