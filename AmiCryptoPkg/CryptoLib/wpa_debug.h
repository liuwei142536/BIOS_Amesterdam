/*
 * wpa_supplicant/hostapd / Debug prints
 * Copyright (c) 2002-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_DEBUG_H
#define WPA_DEBUG_H

//#include "wpabuf.h"

/* Debugging function - conditional printf and hex dump. Driver wrappers can
 * use these for debugging purposes. */

enum { MSG_MSGDUMP, MSG_DEBUG, MSG_INFO, MSG_WARNING, MSG_ERROR, MSG_MISC }; // AMI>>>MSG_MISC

#define wpa_hexdump_buf(l,t,b) //do { } while (0)
#define wpa_hexdump_key(l,t,b,le) //do { } while (0)
#define wpa_hexdump_buf_key(l,t,b) //do { } while (0)
#define wpa_hexdump_ascii_key(l,t,b,le) //do { } while (0)
#define wpa_debug_open_file(p) //do { } while (0)
#define wpa_debug_close_file() //do { } while (0)

#ifdef CONFIG_NO_STDOUT_DEBUG

#define wpa_debug_print_timestamp() //do { } while (0)
//#define wpa_printf()// do { } while (0)
#define wpa_printf(l,arg,...)// do { } while (0)
#define wpa_hexdump(l,t,b,le)// do { } while (0)
#define wpa_hexdump_buf(l,t,b) //do { } while (0)
#define wpa_hexdump_ascii(l,t,b,le) //do { } while (0)
#define wpa_debug_print_string_oid(l, t, n) // do { } while (0)
#define wpa_debug_print_name(l, t, n) // do { } while (0)
#else /* CONFIG_NO_STDOUT_DEBUG */
/**
 * wpa_debug_printf_timestamp - Print timestamp for debug output
 *
 * This function prints a timestamp in seconds_from_1970.microseconds
 * format if debug output has been configured to include timestamps in debug
 * messages.
 */
void wpa_debug_print_timestamp(void);

/**
 * wpa_printf - conditional printf
 * @level: priority level (MSG_*) of the message
 * @fmt: printf format string, followed by optional arguments
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration.
 *
 * Note: New line '\n' is added to the end of the text when printing to stdout.
 */
//void wpa_printf(int level, const char *fmt, ...);
void EFIAPI wpa_printf(int level, const char *fmt, ...);
/**
 * wpa_hexdump - conditional hex dump
 * @level: priority level (MSG_*) of the message
 * @title: title of for the message
 * @buf: data buffer to be dumped
 * @len: length of the buf
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration. The contents of buf is printed out has hex dump.
 */
void wpa_hexdump(int level, const char *title, const UINT8 *buf, size_t len);
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
void wpa_hexdump_ascii(int level, const char *title, const UINT8 *buf, size_t len);

// AMI 
/**
 * wpa_debug_print_oid_strting - Print string with OID for debug output
 *
 * This function prints a string with OID
 * 
 */
void wpa_debug_print_string_oid(int level, const char *title, void *oid);
/**
 * wpa_debug_print_name - Print a string string with a x509_name for debug output
 *
 * This function prints a string with OID
 * 
 */
void wpa_debug_print_name(int level, const char *title, void *name);
// end AMI
#ifndef __func__
#define __func__ "__func__ not defined"
#endif

#endif /* CONFIG_NO_STDOUT_DEBUG */

#endif /* WPA_DEBUG_H */
