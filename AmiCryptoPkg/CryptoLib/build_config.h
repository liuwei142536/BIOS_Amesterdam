//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H
/** @file
  Fine tuning of wpa-supplicant crypto lib 
  Configuration defines, e.g., #define CONFIG_CRYPTO_INTERNAL
**/  
//#pragma warning( disable : 4002 4267 4090 /*4334*/)

/* Insert configuration defines, e.g., #define EAP_MD5, here, if needed. */
// build_config.h
#define PKCS12_FUNCS
#define INTERNAL_SHA256
//
#ifndef CONFIG_CRYPTLIB_TINY
//
#define INTERNAL_MD5
#define INTERNAL_SHA1
#define LTC_SHA512
#define CONFIG_CRYPTO_INTERNAL      // sha1/ md5
//
#ifndef CONFIG_NO_PEI_PKCS7 
//
#define CONFIG_INTERNAL_X509        // x509/PKCS7
#if CONFIG_X509_EXT_ALT_NAME_KEY_USAGE == 0
#define CONFIG_IGNORE_X509_EXT_ALT_NAME_KEY_USAGE
#endif
#if CONFIG_X509_CERTIFICATE_EXPIRATION == 0
#define CONFIG_IGNORE_X509_CERTIFICATE_EXPIRATION
#endif
//
#endif // CONFIG_NO_PEI_PKCS7
#endif // CONFIG_CRYPTLIB_TINY
//
#define CONFIG_INTERNAL_LIBTOMMATH  // bignum
#ifdef  MDEPKG_NDEBUG
#ifndef CONFIG_NO_STDOUT_DEBUG
#define CONFIG_NO_STDOUT_DEBUG
#endif
#endif
/* Hash fast generates a faster hash at a cost of ~20k in uncompressed code*/
/* Size penalty to compressed Crypto PPI and DXE driver binaries is ~4.5k*/
#if SHA256_FAST == 0
#define HASH_SMALL_CODE
#define LTC_SMALL_CODE
#endif

#define __BYTE_ORDER __BIG_ENDIAN

// bignum math
//#define MP_PREC                 64     /* default digits of precision */

#if defined(_WIN64)
//typedef unsigned __int64        size_t;
#else
#ifndef __size_t__
#define __size_t__
//typedef unsigned int            size_t;
typedef UINTN            size_t;
#endif // __size_t__
#endif

void *amicrypt_malloc(size_t Size);
void amicrypt_free(void *ptr);
void *amicrypt_realloc(void *OldPtr, size_t NewSize);
void *memset(void* pBuffer, int Value, size_t Size);
void *memcpy(void* pDestination, void* pSource, unsigned long Length);
int _stricmp( const CHAR8 *string1, const CHAR8 *string2 ); //for build error, conflicting types, Scrtlib.c : 106
char *_strdup(const CHAR8 *s);
// defines for x509v3.c
int AtoiEX(const char *s, UINT8 s_len, int* value);
int EFIAPI Snprintf(char *str, UINTN size, const char *format, ...);
//VOID AmiCrypt_ProcessFormatStringtoEdk2(const char *String);

// AMI override in scrtlib.c. MDE version process '%s' as unicode strings
//#define os_snprintf AsciiSPrint
#define os_snprintf Snprintf

#define os_malloc(s) amicrypt_malloc((s))
#define os_realloc(p, s) amicrypt_realloc((p), (s))
#define os_free(p) amicrypt_free((p))

#define _stricmp (int)AsciiStriCmp
#define strlen AsciiStrLen
#define memcmp CompareMem
#define memcpy CopyMem
#define memmove CopyMem

//#ifndef memset
#define memset(d, n, s) SetMem((d), (s), (n))
//#endif
#ifndef Atoi
#define Atoi (int)AsciiStrDecimalToUintn
#endif

// Define var arg macros via their EDKII counterparts.
// Unlike Aptio 4 definitions, EDKII macros support multiple tool chains.
#ifndef va_list
//typedef CHAR8   *va_list;
#define va_list VA_LIST
#endif
#ifndef va_start
//#define va_start(ap,v)  ( ap = (va_list)&(v) + _INTSIZEOF(v) )
#define va_start(ap,v)  ( VA_START(ap,v),ap )
#endif
#ifndef va_arg
//#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_arg VA_ARG
#endif
#ifndef va_end
//#define va_end(ap)      ( ap = (va_list)0 )
#define va_end VA_END
#endif
// end variable argument support
#endif /* BUILD_CONFIG_H */
