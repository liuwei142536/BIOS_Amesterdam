//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  Root include file of C runtime library to support building the third-party
  cryptographic library.

Copyright (c) 2010 - 2021, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __CRT_LIB_SUPPORT_H__
#define __CRT_LIB_SUPPORT_H__

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#define OPENSSLDIR  ""
#define ENGINESDIR  ""

#define MAX_STRING_SIZE  0x1000

//
// We already have "no-ui" in out Configure invocation.
// but the code still fails to compile.
// Ref:  https://github.com/openssl/openssl/issues/8904
//
// This is defined in CRT library(stdio.h).
//
#ifndef BUFSIZ
#define BUFSIZ  8192
#endif

//
// OpenSSL relies on explicit configuration for word size in crypto/bn,
// but we want it to be automatically inferred from the target. So we
// bypass what's in <openssl/opensslconf.h> for OPENSSL_SYS_UEFI, and
// define our own here.
//
#ifdef CONFIG_HEADER_BN_H
  #error CONFIG_HEADER_BN_H already defined
#endif

#define CONFIG_HEADER_BN_H

#if !defined (SIXTY_FOUR_BIT) && !defined (THIRTY_TWO_BIT)
  #if defined (MDE_CPU_X64) || defined (MDE_CPU_AARCH64) || defined (MDE_CPU_IA64) || defined (MDE_CPU_RISCV64)
//
// With GCC we would normally use SIXTY_FOUR_BIT_LONG, but MSVC needs
// SIXTY_FOUR_BIT, because 'long' is 32-bit and only 'long long' is
// 64-bit. Since using 'long long' works fine on GCC too, just do that.
//
#define SIXTY_FOUR_BIT
  #elif defined (MDE_CPU_IA32) || defined (MDE_CPU_ARM) || defined (MDE_CPU_EBC)
#define THIRTY_TWO_BIT
  #else
    #error Unknown target architecture
  #endif
#endif

//
// Map all va_xxxx elements to VA_xxx defined in MdePkg/Include/Base.h
//
#if !defined (__CC_ARM) // if va_list is not already defined
// AMI OVERRIDE starts - To avoid redefinition build error
#ifndef va_list
#define va_list   VA_LIST
#endif
#ifndef va_arg
#define va_arg    VA_ARG
#endif
#ifndef va_start
#define va_start  VA_START
#endif
#ifndef va_end
#define va_end    VA_END
#endif
// AMI OVERRIDE ends - To avoid redefinition build error
#else // __CC_ARM
#define va_start(Marker, Parameter)  __va_start(Marker, Parameter)
#define va_arg(Marker, TYPE)         __va_arg(Marker, TYPE)
#define va_end(Marker)               ((void)0)
#endif

//
// Definitions for global constants used by CRT library routines
//
#define EINVAL        22              /* Invalid argument */
#define EAFNOSUPPORT  47              /* Address family not supported by protocol family */
#define INT_MAX       0x7FFFFFFF      /* Maximum (signed) int value */
#define LONG_MAX      0X7FFFFFFFL     /* max value for a long */
#define LONG_MIN      (-LONG_MAX-1)   /* min value for a long */
#define ULONG_MAX     0xFFFFFFFF      /* Maximum unsigned long value */
#define CHAR_BIT      8               /* Number of bits in a char */

//
// Address families.
//
#define AF_INET   2     /* internetwork: UDP, TCP, etc. */
#define AF_INET6  24    /* IP version 6 */

//
// Define constants based on RFC0883, RFC1034, RFC 1035
//
#define NS_INT16SZ    2   /*%< #/bytes of data in a u_int16_t */
#define NS_INADDRSZ   4   /*%< IPv4 T_A */
#define NS_IN6ADDRSZ  16  /*%< IPv6 T_AAAA */

//
// Basic types mapping
//
typedef UINTN   size_t;
typedef UINTN   u_int;
typedef INTN    ptrdiff_t;
typedef INTN    ssize_t;
typedef INT32   time_t;
typedef UINT8   __uint8_t;
typedef UINT8   sa_family_t;
typedef UINT8   u_char;
typedef UINT32  uid_t;
typedef UINT32  gid_t;
typedef CHAR16  wchar_t;

//
// File operations are not required for EFI building,
// so FILE is mapped to VOID * to pass build
//
typedef VOID *FILE;

//
// Structures Definitions
//
struct tm {
  int     tm_sec;    /* seconds after the minute [0-60] */
  int     tm_min;    /* minutes after the hour [0-59] */
  int     tm_hour;   /* hours since midnight [0-23] */
  int     tm_mday;   /* day of the month [1-31] */
  int     tm_mon;    /* months since January [0-11] */
  int     tm_year;   /* years since 1900 */
  int     tm_wday;   /* days since Sunday [0-6] */
  int     tm_yday;   /* days since January 1 [0-365] */
  int     tm_isdst;  /* Daylight Savings Time flag */
  long    tm_gmtoff; /* offset from CUT in seconds */
  char    *tm_zone;  /* timezone abbreviation */
};

struct timeval {
  long    tv_sec;   /* time value, in seconds */
  long    tv_usec;  /* time value, in microseconds */
};

//APTIOV OVERRIDE starts - Defined in Openssl.
/* 
struct sockaddr {
  __uint8_t    sa_len;        total length 
  sa_family_t  sa_family;     address family 
  char         sa_data[14];   actually longer; address value 
};
*/
//APTIOV OVERRIDE ends - Defined in Openssl.
//
// Global variables
//
extern int   errno;
extern FILE  *stderr;

//
// Function prototypes of CRT Library routines
//
//APTIOV OVERRIDE starts - Defined as local_malloc, local_realloc, local_free.
/*
void           *malloc     (size_t);
void           *realloc    (void *, size_t);
void           free        (void *);
*/
//APTIOV OVERRIDE ends -  Defined as local_malloc, local_realloc, local_free.
void           *
memset     (
  void *,
  int,
  size_t
  );

int
memcmp      (
  const void *,
  const void *,
  size_t
  );

int
isdigit     (
  int
  );

int
isspace     (
  int
  );

int
isxdigit    (
  int
  );

int
isalnum     (
  int
  );

int
isupper     (
  int
  );

int
tolower     (
  int
  );

int
strcmp      (
  const char *,
  const char *
  );

int
strncasecmp (
  const char *,
  const char *,
  size_t
  );

char           *
strchr     (
  const char *,
  int
  );

char           *
strrchr    (
  const char *,
  int
  );

unsigned long
strtoul     (
  const char *,
  char **,
  int
  );

long
strtol      (
  const char *,
  char **,
  int
  );

char           *
strerror   (
  int
  );

size_t
strspn      (
  const char *,
  const char *
  );

size_t
strcspn     (
  const char *,
  const char *
  );

int
printf      (
  const char *,
  ...
  );

int
sscanf      (
  const char *,
  const char *,
  ...
  );

FILE           *
fopen      (
  const char *,
  const char *
  );

size_t
fread       (
  void *,
  size_t,
  size_t,
  FILE *
  );

size_t
fwrite      (
  const void *,
  size_t,
  size_t,
  FILE *
  );

int
fclose      (
  FILE *
  );

int
fprintf     (
  FILE *,
  const char *,
  ...
  );

time_t
time        (
  time_t *
  );

struct tm      *
gmtime     (
  const time_t *
  );

uid_t
getuid      (
  void
  );

uid_t
geteuid     (
  void
  );

gid_t
getgid      (
  void
  );

gid_t
getegid     (
  void
  );

int
issetugid   (
  void
  );

void
qsort       (
  void *,
  size_t,
  size_t,
  int (*)(const void *, const void *)
  );

char           *
getenv     (
  const char *
  );

char           *
secure_getenv (
  const char *
  );

#if defined (__GNUC__) && (__GNUC__ >= 2)
void
abort       (
  void
  ) __attribute__ ((__noreturn__));

#else
void
abort       (
  void
  );

#endif
int
inet_pton   (
  int,
  const char *,
  void *
  );


// AMI OVERRIDE starts - Include Error codes needed in files like crypto\bio\bss_file.c.

#ifndef EOF
#define EOF     (-1)
#endif

// #defines from EFI Application Toolkit required to build Open SSL

/* Supported address families. */
#define AF_UNSPEC   0
#define AF_INET     2
/* Control Messages */
#define SCM_RIGHTS      1
/* Socket types. */
#define SOCK_STREAM 1
#define SOCK_DGRAM  2
#define SOCK_RAW    3
#define SOCK_RDM    4
#define SOCK_SEQPACKET  5
#define SOCK_PACKET 10

#define SOL_SOCKET      0xffff
#define SO_ERROR        0x1007
#define SO_KEEPALIVE    0x0008
#define SO_TYPE         0x1008
#define SO_REUSEADDR    0x0004

#define PROTNUM_TCP     6
#define PROTNUM_UDP     17

#define IPPROTO_TCP     (PROTNUM_TCP)
#define IPPROTO_UDP     (PROTNUM_UDP)

#ifndef INADDR_ANY
# define INADDR_ANY ((unsigned long int) 0x00000000)
#endif /* INADDR_ANY */

#ifndef INADDR_LOOPBACK
# define INADDR_LOOPBACK    ((unsigned long int) 0x7f000001)
#endif /* INADDR_LOOPBACK */

/*
 * Error codes - Taken from reference file bsd/sys/errno.h
 */
#define EPERM       1
#define ENOENT      2
#define ESRCH       3
#define EINTR       4
#define EIO         5
#define ENXIO       6
#define E2BIG       7
#define ENOEXEC     8
#define EBADF       9
#define ECHILD      10
#define EDEADLK     11
#define ENOMEM      12
#define EACCES      13
#define EFAULT      14
#ifndef _POSIX_SOURCE
#define ENOTBLK     15
#endif
#define EBUSY       16
#define EEXIST      17
#define EXDEV       18
#define ENODEV      19
#define ENOTDIR     20
#define EISDIR      21
#define ENFILE      23
#define EMFILE      24
#define ENOTTY      25
#ifndef _POSIX_SOURCE
#define ETXTBSY     26
#endif
#define EFBIG       27
#define ENOSPC      28
#define ESPIPE      29
#define EROFS       30
#define EMLINK      31
#define EPIPE       32

/* math software */
#define EDOM        33
#define ERANGE      34

/* non-blocking and interrupt i/o */
#define EAGAIN      35
#ifndef _POSIX_SOURCE
#define EWOULDBLOCK EAGAIN
#define EINPROGRESS 36
#define EALREADY    37

/* ipc/network software -- argument errors */
#define ENOTSOCK    38
#define EDESTADDRREQ    39
#define EMSGSIZE    40    
#define EPROTOTYPE  41    
#define ENOPROTOOPT 42    
#define EPROTONOSUPPORT 43
#define ESOCKTNOSUPPORT 44
#endif /* ! _POSIX_SOURCE */
#define ENOTSUP 45      
#ifndef _POSIX_SOURCE
#define EOPNOTSUPP   ENOTSUP
#define EPFNOSUPPORT    46
#define EAFNOSUPPORT    47
#define EADDRINUSE      48    
#define EADDRNOTAVAIL   49

/* ipc/network software -- operational errors */
#define ENETDOWN        50
#define ENETUNREACH     51
#define ENETRESET       52
#define ECONNABORTED    53
#define ECONNRESET      54
#define ENOBUFS         55
#define EISCONN         56
#define ENOTCONN        57
#define ESHUTDOWN       58
#define ETOOMANYREFS    59
#define ETIMEDOUT       60
#define ECONNREFUSED    61

#define ELOOP           62
#endif /* _POSIX_SOURCE */
#define ENAMETOOLONG    63

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define EHOSTDOWN       64
#define EHOSTUNREACH    65
#endif /* _POSIX_SOURCE */
#define ENOTEMPTY       66

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define EPROCLIM        67
#define EUSERS          68
#define EDQUOT          69

/* Network File System */
#define ESTALE          70
#define EREMOTE         71
#define EBADRPC         72
#define ERPCMISMATCH    73
#define EPROGUNAVAIL    74
#define EPROGMISMATCH   75
#define EPROCUNAVAIL    76
#endif /* _POSIX_SOURCE */

#define ENOLCK          77
#define ENOSYS          78

#ifndef _POSIX_SOURCE
#define EFTYPE          79
#define EAUTH           80
#define ENEEDAUTH       81
#define EOVERFLOW       84
#define ELAST           84
#endif /* _POSIX_SOURCE */

/* Intelligent device errors */
#define EPWROFF         82
#define EDEVERR         83

/* Program loading errors */
#define EBADEXEC        85
#define EBADARCH        86
#define ESHLIBVERS      87
#define EBADMACHO       88
                
#define INT_MIN      (-2147483647-1)  
       
#define LOG_DAEMON   (3<<3)           
#define LOG_EMERG    0                
#define LOG_ALERT    1                
#define LOG_CRIT     2                
#define LOG_ERR      3                
#define LOG_WARNING  4                
#define LOG_NOTICE   5                
#define LOG_INFO     6                
#define LOG_DEBUG    7                
#define LOG_PID      0x01             
#define LOG_CONS     0x02             
//
// Basic types from EFI Application Toolkit required to build Open SSL
//
typedef INT64          off_t;
typedef UINT16         mode_t;
typedef unsigned long  clock_t;
typedef UINT32         ino_t;
typedef UINT32         dev_t;
typedef UINT16         nlink_t;
typedef int            pid_t;
typedef void           *DIR;
typedef void           __sighandler_t (int);

struct dirent {
  UINT32  d_fileno;
  UINT16  d_reclen;
  UINT8   d_type;
  UINT8   d_namlen;
  char    d_name[255 + 1];
};

struct stat {
  dev_t    st_dev;
  ino_t    st_ino;
  mode_t   st_mode;
  nlink_t  st_nlink;
  uid_t    st_uid;
  gid_t    st_gid;
  dev_t    st_rdev;
  time_t   st_atime;
  long     st_atimensec;
  time_t   st_mtime;
  long     st_mtimensec;
  time_t   st_ctime;
  long     st_ctimensec;
  off_t    st_size;
  INT64    st_blocks;
  UINT32   st_blksize;
  UINT32   st_flags;
  UINT32   st_gen;
  INT32    st_lspare;
  INT64    st_qspare[2];
};

void           *local_malloc     (size_t);
void           *local_realloc    (void *, size_t);
void           local_free        (void *);
time_t         local_time (time_t *timer);
struct tm * local_gmtime (const time_t *timer);


void           *memcpy     (void *, const void *, size_t);
void           *memchr     (const void *, int, size_t);
void           *memmove    (void *, const void *, size_t);

int            strncmp     (const char *, const char *, size_t);
char           *strcpy     (char *, const char *);
char           *strncpy    (char *, const char *, size_t);
size_t         strlen      (const char *);
char           *strcat     (char *, const char *);
char           *strchr     (const char *, int);
int            open        (const char *, int, ...);
int            chmod       (const char *, mode_t);
int            stat        (const char *, struct stat *);
off_t          lseek       (int, off_t, int);
ssize_t        read        (int, void *, size_t);
ssize_t        write       (int, const void *, size_t);
int            close       (int);
char           *fgets      (char *, int, FILE *);
int            fputs       (const char *, FILE *);
int            vfprintf    (FILE *, const char *, VA_LIST);
int            fflush      (FILE *);
DIR            *opendir    (const char *);
struct dirent  *readdir    (DIR *);
int            closedir    (DIR *);
void           openlog     (const char *, int, int);
void           closelog    (void);
void           syslog      (int, const char *, ...);
char           *strdup     (const char *str);
int            feof        (FILE *fp);
int            fseek       (FILE *fp, long l_offset, int whence);
int            ferror      (FILE *fp);
long           ftell       (FILE *fp);

struct tm      *localtime  (const time_t *);
struct tm      *gmtime_r   (const time_t *, struct tm *);
void           exit        (int);

__sighandler_t *signal     (int, __sighandler_t *);

//
// Global variables from EFI Application Toolkit required to build Open SSL
//
extern FILE  *stdin;
extern FILE  *stdout;

#define strcmp                            AsciiStrCmp
#define gettimeofday(tvp,tz)              do { (tvp)->tv_sec = local_time(NULL); (tvp)->tv_usec = 0; } while (0)
#define gmtime_r(timer,result)            (result = NULL)
//APTIOV OVERRIDE ends

//
// Macros that directly map functions to BaseLib, BaseMemoryLib, and DebugLib functions
//
#define memcpy(dest, source, count)         CopyMem(dest,source,(UINTN)(count))
#define memset(dest, ch, count)             SetMem(dest,(UINTN)(count),(UINT8)(ch))
#define memchr(buf, ch, count)              ScanMem8(buf,(UINTN)(count),(UINT8)ch)
#define memcmp(buf1, buf2, count)           (int)(CompareMem(buf1,buf2,(UINTN)(count)))
#define memmove(dest, source, count)        CopyMem(dest,source,(UINTN)(count))
#define strlen(str)                         (size_t)(AsciiStrnLenS(str,MAX_STRING_SIZE))
#define strcpy(strDest, strSource)          AsciiStrCpyS(strDest,MAX_STRING_SIZE,strSource)
#define strncpy(strDest, strSource, count)  AsciiStrnCpyS(strDest,MAX_STRING_SIZE,strSource,(UINTN)count)
#define strcat(strDest, strSource)          AsciiStrCatS(strDest,MAX_STRING_SIZE,strSource)
#define strncmp(string1, string2, count)    (int)(AsciiStrnCmp(string1,string2,(UINTN)(count)))
#define strcasecmp(str1, str2)              (int)AsciiStriCmp(str1,str2)
#define sprintf(buf, ...)                   AsciiSPrint(buf,MAX_STRING_SIZE,__VA_ARGS__)
#define localtime(timer)                    NULL
#define assert(expression)
#define offsetof(type, member)  OFFSET_OF(type,member)
#define atoi(nptr)              AsciiStrDecimalToUintn(nptr)
//#define gettimeofday(tvp,tz)              do { (tvp)->tv_sec = time(NULL); (tvp)->tv_usec = 0; } while (0) //APTIOV OVERRIDE -Re-Defined as local_time()

#endif
