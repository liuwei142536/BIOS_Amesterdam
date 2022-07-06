//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/*
 * wpa_supplicant/hostapd / Debug prints
 * Copyright (c) 2002-2013, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

/** @file
 * wpa_debug.c - Debug related functions
 */

#include "includes.h"
#include "common.h"
#include "x509v3.h"

UINT8 crypto_trace_level = CRYPTO_trace_level; // default - no traces

/**
 * wpa_set_trace_level - sets the debug message print mode
 * @level: print level: 0 - no messages, 1 - minimal, 2 - full
 */
void wpa_set_trace_level(UINT8 level)
{
    crypto_trace_level = level;
}

#ifndef CONFIG_NO_STDOUT_DEBUG

void EFIAPI wpa_printf(int level, const char *fmt, ...)
{
    VA_LIST   ArgList;
    CHAR8 Buffer[256];

    if (fmt==NULL || 
        crypto_trace_level==0) return;

    //AMI mod - MDE lib version process '%s' as unicode strings
//    AmiCrypt_ProcessFormatStringtoEdk2(fmt);

    VA_START(ArgList,fmt);
    AsciiVSPrint (Buffer, sizeof (Buffer), fmt, ArgList);
    VA_END(ArgList);
    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, Buffer));

    if (level != MSG_MISC) DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "\n"));
}

void wpa_hexdump(int level, const char *title, const UINT8 *buf,
             size_t len)
{
    size_t i;
    size_t max_print_len;

    switch(crypto_trace_level) {
        case 0: return;
        case 1: max_print_len = 16; break;
        case 2: max_print_len = len; break;
        default: max_print_len = len;
    }

    wpa_printf(MSG_MISC, "%a - hexdump(len=%x):", title, (unsigned long) len);
    if (buf == NULL) {
        wpa_printf(MSG_MSGDUMP," [NULL]");
    } else {
        for (i = 0; i < len && i < max_print_len; i++) {
            wpa_printf(MSG_MISC, " %02x", buf[i]);
        }
        if(len>max_print_len)
            wpa_printf(MSG_MISC,"...>>>");
    }
    wpa_printf(MSG_MISC,"\n");
}

#ifdef CONFIG_INTERNAL_X509

/**
 * wpa_debug_printf_timestamp - Print timestamp for debug output
 *
 * This function prints a timestamp in seconds_from_1970.microseconds
 * format if debug output has been configured to include timestamps in debug
 * messages.
 */
void wpa_debug_print_timestamp(void)
{
    struct os_time tv;

    os_get_time(&tv);
    wpa_printf(MSG_INFO, "Current Time: %x sec\n", (long) tv.sec);
}

/*
 * For the standard ASCII character set, control characters are those between 
 * ASCII codes 0x00 (NUL) and 0x1f (US), plus 0x7f (DEL). 
 * Therefore, printable characters are all but these, although specific compiler 
 * implementations for certain platforms may define additional control characters 
 * in the extended character set (above 0x7f).
*/
//int isprint(UINT8 ch)
//{
//    int i=0;
//    i= (ch > 0x1f) && (ch < 0x7f)?1:0;
//    return i;
//}
/**
 * wpa_hexdump_ascii - conditional hex dump
 * @level: priority level (MSG_*) of the message
 * @title: title of for the message
 * @buf: data buffer to be dumped
 * @len: length of the buf
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration. The contents of buf is printed out has hex dump with both
 * the hex numbers and ASCII characters (for printable range) are shown. 16
 * bytes per line will be shown.
 */
void wpa_hexdump_ascii(int level, const char *title, const UINT8 *buf,
                   size_t len)
{
    size_t i;
    size_t llen;

    const UINT8 *pos = buf;
    const size_t line_len = 32;

    //    wpa_debug_print_timestamp();
    if (crypto_trace_level == 0) return;

    if (buf == NULL) {
        wpa_printf(MSG_MISC,"%a - hexdump_ascii(len=%x): [NULL]\n", title, (unsigned long) len);
        return;
    }
    wpa_printf(MSG_MISC, "%a - hexdump_ascii(len=%x):\n", title, (unsigned long) len);

    while (len) {
        llen = len > line_len ? line_len : len;
        wpa_printf(MSG_MISC, "\tHEX  :");
        for (i = 0; i < llen; i++)
            wpa_printf(MSG_MISC," %02x", pos[i]);
//        for (i = llen; i < line_len; i++)
//            wpa_printf(MSG_MISC,"===");
        wpa_printf(MSG_MISC, "\n\tASCII: ");
        for (i = 0; i < llen; i++) {
            if ((pos[i] > 0x1f) && (pos[i] < 0x7f)) // isprint
                wpa_printf(MSG_MISC, "%c", pos[i]);
            else
                wpa_printf(MSG_MISC,"_");
        }
//        for (i = llen; i < line_len; i++)
//            wpa_printf(MSG_MISC,"=");
        wpa_printf(MSG_MISC,"\n");
        pos += llen;
        len -= llen;
    }
}

///AMI///
/**
 * wpa_debug_print_oid_strting - Print string with OID for debug output
 *
 * This function prints a string with OID
 * 
 */
void wpa_debug_print_string_oid(int level, const char *title, void *oid)
{
    char sbuf[128];

    asn1_oid_to_str((const struct asn1_oid *)oid, sbuf, sizeof(sbuf));
    wpa_printf(MSG_INFO, "%a: %a", title, sbuf);
}
/**
 * wpa_debug_print_name - Print a string string with a x509_name for debug output
 *
 * This function prints a string with OID
 * 
 */
void wpa_debug_print_name(int level, const char *title, void *name)
{
    char sbuf[128];

    x509_name_string((struct x509_name *)name, sbuf, sizeof(sbuf));
    wpa_printf(MSG_INFO, "%a: %a", title, sbuf);
}
#endif /* CONFIG_INTERNAL_X509 */
#endif /* CONFIG_NO_STDOUT_DEBUG */
