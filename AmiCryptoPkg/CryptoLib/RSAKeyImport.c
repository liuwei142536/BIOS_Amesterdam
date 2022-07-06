//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
/** @file

 Helper routines for PEI and DXE crypto library
Implements following auxiliary function:
                  -crypto_import_rsa2048_public_key
                  -crypto_public_key_import
                  -crypto_public_key_decrypt_pkcs1
                  -ASN.1 DER base64_decode
**/

#include "includes.h"
#include "common.h"
#include "crypto.h"
#include "bignum.h"
#include "rsa.h"

// Structure is defined in rsa.c
struct crypto_rsa_key {
    int private_key; /* whether private key is set */
    struct bignum *n; /* modulus (p * q) */
    struct bignum *e; /* public exponent */
    /* The following parameters are available only if private_key is set */
//    struct bignum *d; /* private exponent */
//    struct bignum *p; /* prime p (factor of n) */
//    struct bignum *q; /* prime q (factor of n) */
//    struct bignum *dmp1; /* d mod (p - 1); CRT exponent */
//    struct bignum *dmq1; /* d mod (q - 1); CRT exponent */
//    struct bignum *iqmp; /* 1 / q mod p; CRT coefficient */
};
//<AMI_PHDR_START>
//**********************************************************************
//
// Procedure:  crypto_import_rsa2048_public_key
//
// Description:    Imports RSA2048 public key binary from RAW format into internal crypto_rsa_key data structure
//
// Input:
//  KeyN      - Pointer to N public modulus buffer
//  KeyE      - Pointer to E public exponent buffer
//  LenN      - Size of KeyN buffer (must be 256)
//  LenE      - Size of KeyE buffer
//
// Output:      Pointer to Key data structure crypto_rsa_key 
//            NULL if failed to import Key
//
//**********************************************************************
//<AMI_PHDR_END>

struct crypto_rsa_key *
crypto_import_rsa2048_public_key(const u8 *KeyN, size_t lenN, const u8 *KeyE, size_t lenE)
{
    struct crypto_rsa_key *key;
    u8 *KeyNmod;
    
    if(!KeyN || !KeyE)
        return NULL;
    
    KeyNmod = (u8*)KeyN;
// !!! NOTE !!!!
// KeyGen adds a leading ZERO if the msb of the first byte of the n-modulus is ONE.
// This is to avoid this integer to be treated as negative value.
// If your calculations produce a number with the high bit set to 1,
// just increase the length by another byte and pad the beginning with 0x00
// to keep it positive.
    if(KeyN[0] >> 7 == 1)
    {
        KeyNmod = os_malloc(lenN+1);
        if (KeyNmod == NULL)
            return NULL;
        memcpy(KeyNmod+1, (void*)KeyN, lenN);
        KeyNmod[0] = 0;
        lenN++;
    }

    key = os_malloc(sizeof(*key));
    if (key == NULL)
        return NULL;

    key->n = bignum_init();
    key->e = bignum_init();
    if (key->n == NULL || key->e == NULL) {
        goto error;
    }

//         * RSA2048PublicKey ::= 
//         *     modulus INTEGER, -- n

    if(bignum_set_unsigned_bin(key->n, KeyNmod, lenN) < 0 )
        goto error;
    if(bignum_set_unsigned_bin(key->e, KeyE, lenE) < 0 )
        goto error;

    return key;

error:
    if(KeyNmod != (u8*)KeyN)
        os_free(KeyNmod);
    crypto_rsa_free(key);
    return NULL;
}
#ifdef CONFIG_INTERNAL_X509
//<AMI_PHDR_START>
//**********************************************************************
//
// Procedure:  crypto_public_key_import
//
// Description:    
//
// Input:
//
// Output:     
//
//**********************************************************************
//<AMI_PHDR_END>
struct crypto_public_key * crypto_public_key_import(const u8 *key, size_t len)
{
    return (struct crypto_public_key *)
        crypto_rsa_import_public_key(key, len);
}

void crypto_public_key_free(struct crypto_public_key *key)
{
    crypto_rsa_free((struct crypto_rsa_key *) key);
//    if (key) {
//        bignum_deinit(key->n);
//        bignum_deinit(key->e);
//    }
}

int crypto_public_key_decrypt_rsa(struct crypto_public_key *key,
                    const u8 *crypt, size_t crypt_len,
                    u8 *plain, size_t *plain_len)
{

    return crypto_rsa_exptmod(crypt, crypt_len, plain, plain_len,
                            (struct crypto_rsa_key *) key, 0);
}
//<AMI_PHDR_START>
//**********************************************************************
//
// Procedure:  crypto_public_key_decrypt_pkcs1
//
// Description:    
//
// Input:
//
// Output:     
//
//**********************************************************************
//<AMI_PHDR_END>
int crypto_public_key_decrypt_pkcs1(struct crypto_public_key *key,
                    const u8 *crypt, size_t crypt_len,
                    u8 *plain, size_t *plain_len)
{
    size_t len;
    u8 *pos;

    len = *plain_len;
    if (crypto_rsa_exptmod(crypt, crypt_len, plain, &len,
                   (struct crypto_rsa_key *) key, 0) < 0)
        return -1;

    /*
     * PKCS #1 v1.5, 8.1:
     *
     * EB = 00 || BT || PS || 00 || D
     * BT = 00 or 01
     * PS = k-3-||D|| times (00 if BT=00) or (FF if BT=01)
     * k = length of modulus in octets
     */
    wpa_hexdump(MSG_MSGDUMP, "PKCS1: Sig Decrypted ", plain, len);

    if (len < 3 + 8 + 16 /* min hash len */ ||
        plain[0] != 0x00 || (plain[1] != 0x00 && plain[1] != 0x01)) {
        wpa_printf(MSG_INFO, "PKCS1: Invalid signature EB structure");
        return -1;
    }

    pos = plain + 3;
    if (plain[1] == 0x00) {
        /* BT = 00 */
        if (plain[2] != 0x00) {
            wpa_printf(MSG_INFO, "PKCS1: Invalid signature PS (BT=00)");
            return -1;
        }
        while (pos + 1 < plain + len && *pos == 0x00 && pos[1] == 0x00)
            pos++;
    } else {
        /* BT = 01 */
        if (plain[2] != 0xff) {
            wpa_printf(MSG_INFO, "PKCS1: Invalid signature PS (BT=01)");
            return -1;
        }
        while (pos < plain + len && *pos == 0xff)
            pos++;
    }

    if (pos - plain - 2 < 8) {
        /* PKCS #1 v1.5, 8.1: At least eight octets long PS */
        wpa_printf(MSG_INFO, "PKCS1: Too short signature padding");
        return -1;
    }

    if (pos + 16 /* min hash len */ >= plain + len || *pos != 0x00) {
        wpa_printf(MSG_INFO, "PKCS1: Invalid signature EB structure (2)");
        return -1;
    }
    pos++;
    len -= pos - plain;

    /* Strip PKCS #1 header */
    os_memmove(plain, pos, len);
    *plain_len = len;

    return 0;
}
#endif //#ifdef CONFIG_INTERNAL_X509
//**********************************************************************
//**********************************************************************

#if defined(ASN1_BASE64_DECODE) && ASN1_BASE64_DECODE == 1

static const unsigned char base64_table[65] =
 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//<AMI_PHDR_START>
//**********************************************************************
//
// Procedure:  base64_decode
//
// Description:    Do in-place decoding of base-64 data of LENGTH in BUFFER.
//
// Input:
//  buffer       - Data to be decoded
//  length       - Length of the data to be decoded
//
// Output:      New length of the buffer bytes of decoded data,
//              or -1 n failure
//
//**********************************************************************
//<AMI_PHDR_END>
size_t
base64_decode (char *src, size_t len)
{
    unsigned char dtable[256], *out, *pos, in[4], block[4], tmp;
    size_t i, count;

    memset(dtable, 0x80, 256);
    for (i = 0; i < sizeof(base64_table); i++)
        dtable[base64_table[i]] = (unsigned char)i;
    dtable['='] = 0;

    count = 0;
    for (i=0; i < len; i++) {
        if (dtable[src[i]] != 0x80)
            count++;
    }

    if (count == 0 /*|| count % 4*/)
        return 0;

    for (i=0; i < len; i++) {

        if (src[i] == '-' && len > 11 && !memcmp (&src[i], "-----BEGIN ", 11))
        {
            for (; i < len && src[i] != '\n'; i++) {;}
            break;
        }
    }
    count = 0;
    pos = out = (unsigned char*)src;
    for (; i < len; i++) {

        if (src[i] == '-' && len > 9 && !memcmp (&src[i], "-----END ", 9))
            break;

        tmp = dtable[src[i]];
        if (tmp == 0x80)
            continue;

        in[count] = src[i];
        block[count] = tmp;
        count++;
        if (count == 4) {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
        }
    }

    if (pos > out) {
        if (in[2] == '=')
            pos -= 2;
        else if (in[3] == '=')
            pos--;
    }

    return pos - out;
}
#endif
