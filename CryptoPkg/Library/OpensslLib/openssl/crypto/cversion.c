//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include "internal/cryptlib.h"

//APTIOV OVERRIDE - To Resolve Build error
#ifndef WIN32
#define NO_WINDOWS_BRAINDEATH 1
#endif

#include "MacOS/buildinf.h"
//APTIOV OVERRIDE - To Resolve Build error
unsigned long OpenSSL_version_num(void)
{
    return OPENSSL_VERSION_NUMBER;
}

const char *OpenSSL_version(int t)
{
    switch (t) {
    case OPENSSL_VERSION:
        return OPENSSL_VERSION_TEXT;
    case OPENSSL_BUILT_ON:
        return DATE;
    case OPENSSL_CFLAGS:
        return compiler_flags;
    case OPENSSL_PLATFORM:
        return PLATFORM;
    case OPENSSL_DIR:
#ifdef OPENSSLDIR
        return "OPENSSLDIR: \"" OPENSSLDIR "\"";
#else
        return "OPENSSLDIR: N/A";
#endif
    case OPENSSL_ENGINES_DIR:
#ifdef ENGINESDIR
        return "ENGINESDIR: \"" ENGINESDIR "\"";
#else
        return "ENGINESDIR: N/A";
#endif
    }
    return "not available";
}
