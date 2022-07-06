/**

Copyright (c) 2004-2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtAmHelper.c

@brief
  AT authetication module for using AT DXE driver.
  This driver uses the AT protocol, HECI Protocol and AT Platform Policy to implement Theft Deterrence Technology AM module.

**/

/**
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
**/

#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include "AtAmHelper.h"

const CHAR8  *PEMCodes   = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-!=";
const CHAR8  *LineBreak  = " \n\t";

EFI_STATUS
Base32Encode (
  UINT8 *encodedStr,
  UINTN *encodedLen,
  UINT8 *rawData,
  UINTN rawDataLen
  )
/**

  @brief
  Convert Hex values into Base32 values

  @param[in] encodedStr  Array of encoded BASE values.
  @param[in] encodedLen  Length of encoded values.
  @param[in] rawData     Array of Hex strings that needs to be encoded into BASE32.
  @param[in] rawDataLen  Length of Hex values.

  @retval EFI_BUFFER_TOO_SMALL   Buffer to store encoded string too small.
  @retval EFI_SUCCESS            Initialization complete.

**/
{
  UINTN i;
  UINTN j;
  UINT8 value;
  INTN  shift;
  INTN  shifttable[] = { 3, -2, 1, -4, -1, 2, -3, 0 };

  if (encodedLen && *encodedLen < (rawDataLen * 8) / 5) {
    DEBUG ((EFI_D_ERROR, "ATAm::Base32Encode:  Buffer to store encoded string too small"));
    return EFI_BUFFER_TOO_SMALL;
  }

  for (i = 0, j = 0; i < rawDataLen; j++) {
    shift = shifttable[j % 8];
    if (shift > 0) {
      value = (UINT8) (rawData[i] >> shift);
    } else {
      value = (UINT8) (rawData[i] << (-shift));
      if (i++ < rawDataLen - 1) {
        value |= (rawData[i] >> (8 + shift));
      }
    }

    value &= 0x1f;
    if (25 >= value) {
      value += 'A';
    } else {
      value += '2' - 26;
    }

    if (encodedStr && encodedLen) {
      encodedStr[j] = value;
    }
  }

  if (encodedStr && encodedLen) {
    *encodedLen   = j;
    encodedStr[j] = L'\0';
  }

  return EFI_SUCCESS;
}
///
/// RFC 3548 - http://rfc.net/rfc3548.html#p6
///
INTN
Base32Decode (
  UINT8 *encodedStr,
  UINT8 *decodedData
  )
/**

  @brief
  Convert Base32 values into Hex values.

  @param[in] encodedStr   Array of Decimal numbers.
  @param[out] decodedData Converted Hex values.

  @retval   Length of the Hex Strings that expected.

**/
{

  UINTN i;
  UINTN j;
  INTN  retVal;
  UINT8 value;
  UINTN decodedLen;
  INTN  shifttable[] = { 3, -2, 1, -4, -1, 2, -3, 0 };
  INTN  shift;
  UINTN encodedLen;
  retVal          = 1;
  decodedLen      = 0;
  encodedLen      = AsciiStrLen ((char *) encodedStr);

  decodedLen      = (encodedLen * 5) / 8;

  decodedData[0]  = 0;
  for (i = 0, j = 0; i < encodedLen; i++) {
    if ('A' <= encodedStr[i] && 'Z' >= encodedStr[i]) {
      value = encodedStr[i] - 'A';
    } else if ('a' <= encodedStr[i] && 'z' >= encodedStr[i]) {
      value = encodedStr[i] - 'a';
    } else if ('2' <= encodedStr[i] && '7' >= encodedStr[i]) {
      value = encodedStr[i] - '2' + 26;
    } else {
      retVal = 0;
      break;
    }

    shift = shifttable[i % 8];
    if (shift > 0) {
      decodedData[j] |= (value << shift);
    } else {
      decodedData[j] |= (value >> (-shift));
      ///
      /// Pack the bits that are going to fall off due to right shift in next byte
      /// (left justfied) unless decoding the last character. In case of last
      /// character, the extra bits are just padding to make the length of input
      /// bits a multiple of 5 while encoding - so ignore them
      ///
      if (i != encodedLen - 1) {
        decodedData[++j] = (UINT8) (value << (8 + shift));
      }
    }
  }

  return retVal;
}

VOID
DecimalToHexString (
  CHAR8 *decStr,
  UINT8 *u8Hex,
  UINTN hexIndex
  )
/**

  @brief
  Decimal large (BASE10) into hex value.

  @param[in] decStr   Array of Decimal numbers.
  @param[in] u8Hex    Converted Hex values.
  @param[in] hexIndex Length of the Hex Strings that expected.

**/
{

  UINTN i;
  UINTN j;
  UINTN Remainder;
  CHAR8 Quotient;
  UINTN num;
  UINTN len;
  UINTN leadingZero;

  leadingZero = 1;

  len         = AsciiStrLen ((CHAR8 *) decStr);
  Remainder   = decStr[0] - '0';

  if (len > 1) {
    Remainder = Remainder * 10 + decStr[1] - '0';

  }

  if (len < 3) {
    u8Hex[hexIndex] = (UINT8) Remainder;
    return;
  }

  i = 2;
  j = 0;
  while (i < len) {
    num       = Remainder * 10 + decStr[i] - '0';
    Quotient  = (CHAR8) (num / 256);
    if (!leadingZero || Quotient) {
      decStr[j++] = '0' + Quotient;
      leadingZero = 0;
    }

    Remainder = num % 256;
    i++;

  }

  decStr[j]         = L'\0';
  u8Hex[hexIndex--] = (UINT8) Remainder;

  if (decStr[0] != L'\0') {
    DecimalToHexString (decStr, u8Hex, hexIndex);
  }

  return;
}

VOID
Uint8ToUnicode (
  IN  CHAR8     *AsciiString,
  OUT CHAR16    *UnicodeString
  )
/**

  @brief
  Convert the CHAR8 ASCII into CHAR16 Unicode strings.

  @param[in] AsciiString    Ascii String.
  @param[out] UnicodeString Buffer for converted Unicode string.

**/
{
  UINT8 Index;

  Index = 0;

  while (AsciiString[Index] != 0) {
    UnicodeString[Index] = (CHAR16) AsciiString[Index];
    Index++;
  }
}

UINT8
DecodePEMChar (
  UINT8 pemChar
  )
/**

  @brief
  Decode a PEM-encoded character.

  @param[in] pemChar  PEM-encoded character.

  @retval   Value orresponding to the character, or PEM_INVALID_CHARACTER if the input character could not be decoded.

**/
{
  if ((pemChar >= 'A') && (pemChar <= 'Z')) {
    return pemChar - 'A';
  }

  if ((pemChar >= 'a') && (pemChar <= 'z')) {
    return (pemChar - 'a') + 26;
  }

  if ((pemChar >= '0') && (pemChar <= '9')) {
    return (pemChar - '0') + 52;
  }

  if (pemChar == '-') {
    return 62;
  }

  if (pemChar == '!') {
    return 63;
  }

  if (pemChar == '=') {
    return PEM_PAD_CHAR;

  }

  DEBUG ((EFI_D_ERROR, "DecodePEMChar: Invalid  character %d\n", pemChar));
  return PEM_INVALID_CHAR;
}

UINT8
DecodeBase16Char (
  UINT8 base16char
  )
/**

  @brief
  TODO: Add function description

  @param[in] base16char TODO: Add argument description.

  @retval TODO: Add argument description.

**/
{
  if (base16char >= '0' && base16char <= '9') {
    return base16char - '0';
  } else if (base16char >= 'A' && base16char <= 'F') {
    return base16char - 'A' + 0xA;
  } else if (base16char >= 'a' && base16char <= 'f') {
    return base16char - 'a' + 0xA;
  }

  return 0;
}

BOOLEAN
Base16Decode (
  UINT8  *pString,
  UINT32 stringLen,
  UINT8  *pDecodedData,
  UINT32 *pBufLength
  )
/**

  @brief
  TODO: Add function description

  @param[in] pString        TODO: add argument description
  @param[in] stringLen      TODO: add argument description
  @param[in] pDecodedData   TODO: add argument description
  @param[in] pBufLength     TODO: add argument description

  @retval TODO: add return values

**/
{
  UINT32 i;
  if (stringLen % 2 != 0) {
    DEBUG ((EFI_D_ERROR, "Cannot decode hexadecimal string -- invalid length\n"));
  } else if (*pBufLength < stringLen / 2) {
    DEBUG ((EFI_D_ERROR, "Base16 decode - output buffer too small\n"));
  } else {
    for (i = 0; i < (UINT32) stringLen / 2; i++) {
      pDecodedData[i] = DecodeBase16Char (pString[2 * i]) << 4;
      pDecodedData[i] |= DecodeBase16Char (pString[2 * i + 1]);
    }

    *pBufLength = i;
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
Base16Encode (
  UINT8  *pData,
  UINT32 dataLength,
  UINT8  *pEncodedData,
  UINT32 *pBufLength
  )
/**

  @brief
  TODO: Add function description

  @param[in] pData          TODO: add argument description
  @param[in] dataLength     TODO: add argument description
  @param[in] pEncodedData   TODO: add argument description
  @param[in] pBufLength     TODO: add argument description

  @retval TODO: add return values

**/
{
  UINT32  i;
  if (*pBufLength < 2 * dataLength) {
    DEBUG ((EFI_D_ERROR, "Cannot encode data -- output buffer too small\n"));
    return FALSE;
  }

  DEBUG ((EFI_D_ERROR, "Base16Encode dataLength : %d\n", dataLength));

  for (i = 0; i < dataLength; i++) {
    AsciiSPrint ((UINT8 *) &pEncodedData[2 * i], 3 * sizeof (UINT8), "%02x", pData[i] & 0xFF);
  }

  DEBUG ((EFI_D_ERROR, "Base16Encode AsciiSPrint : %s\n", (char *) pEncodedData));
  DEBUG ((EFI_D_ERROR, "Base16Encode Sprint : %s\n", (char *) pEncodedData));

  *pBufLength = 2 * dataLength;

  return TRUE;
}

BOOLEAN
PEMSMSDecode (
  UINT8  *pPEMString,
  UINT32 lineLength,
  UINT8  *pDecodedData,
  UINT32 *pBufLength
  )
/**

  @brief
  Decode PEM-Encoded data.

  @param[in] pPEMString   Pointer to PEM-Encoded buffer.
  @param[in] lineLength   TODO: add argument description
  @param[in] pDecodedData Pointer to buffer to receive decoded bytes.
  @param[in] pBufLength   Pointer to the size of the output buffer.  Upon return this will contain the length of the decoded data.

  @retval Returns status code indicating outcome of operation.

**/
{
  UINT8   *pStr;
  UINT32  curOutLen;
  UINT8   *pOut;
  UINT32  i;
  UINT32  padBytes;
  UINT8   val1;
  UINT8   val2;
  UINT8   decodedValue[4];
  UINT32  ii;
  UINT32  unitCount;
  UINT32  decodedBytes;

  pStr      = pPEMString;
  curOutLen = *pBufLength;
  pOut      = pDecodedData;
  padBytes  = 0;

  if (pStr != NULL) {
    if ((lineLength % 4) != 0) {
      DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- invalid length -1\n"));
      return FALSE;
    }

    unitCount = lineLength / 4;

    for (i = 0; (i < unitCount) && (padBytes == 0); i++) {
      val1  = DecodePEMChar (*pStr++);
      val2  = DecodePEMChar (*pStr++);

      if ((val1 == PEM_INVALID_CHAR) || (val2 == PEM_INVALID_CHAR)) {
        DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- invalid character -2\n"));
        return FALSE;
      }

      decodedValue[0] = ((val1 << 2) | (val2 >> 4));
      val1            = DecodePEMChar (*pStr++);
      if (val1 == PEM_INVALID_CHAR) {
        DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- invalid character -3\n"));
        return FALSE;
      }

      if (val1 != PEM_PAD_CHAR) {
        decodedValue[1] = ((val2 << 4) | (val1 >> 2));
        val2            = DecodePEMChar (*pStr++);
        if (val2 == PEM_INVALID_CHAR) {
          DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- invalid character -4\n"));
          return FALSE;
        }

        if (val2 == PEM_PAD_CHAR) {
          padBytes = 1;
        } else {
          decodedValue[2] = ((val1 << 6) | val2);
        }
      } else {
        val2 = DecodePEMChar (*pStr++);
        if (val2 != PEM_PAD_CHAR) {
          DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- invalid encoding -4\n"));
          return FALSE;
        }

        padBytes = 2;
      }

      decodedBytes = (3 - padBytes);
      if (curOutLen < decodedBytes) {
        DEBUG ((EFI_D_ERROR, "Cannot decode PEM string -- out buffer too small-5\n"));
        return FALSE;
      }

      for (ii = 0; ii < decodedBytes; ii++) {
        pOut[ii] = decodedValue[ii];

      }

      curOutLen -= decodedBytes;
      pOut += decodedBytes;
    }
  }

  *pBufLength = *pBufLength - curOutLen;
  return TRUE;
}

BOOLEAN
PEMSMSEncode (
  UINT8  *pData,
  UINT32 dataLength,
  UINT8  *pPEMString,
  UINT32 *pBufLength
  )
/**

  @brief
  TODO: add argument description

  @param[in] pData       TODO: add argument description
  @param[in] dataLength  TODO: add argument description
  @param[in] pPEMString  TODO: add argument description
  @param[in] pBufLength  TODO: add argument description

  @retval  TODO: add return values

**/
{
  UINT32  encodeUnits;
  UINT32  paddedInputLength;
  UINT32  padBytes;
  UINT32  outLength;
  UINT8   encodeBuf[3];
  UINT32  i;
  UINT8   *pOctet;
  UINT32  ipos;
  UINT32  opos;
  UINT32  index;

  encodeUnits       = (dataLength + 2) / 3;
  paddedInputLength = encodeUnits * 3;
  padBytes          = paddedInputLength - dataLength;
  outLength         = encodeUnits * 4;
  pOctet            = pData;
  ipos              = 0;
  opos              = 0;

  outLength         = encodeUnits * 4;
  if (outLength % 64) {
    outLength += ((outLength / 64) + 1);
  } else {
    outLength += (outLength / 64);

  }

  outLength++;
  DEBUG ((EFI_D_ERROR, "ATAM: Pbuflength: %d OutLength: %d \n", *pBufLength, outLength));
  if (*pBufLength < outLength) {
    DEBUG ((EFI_D_ERROR, "ATAM: Failed \n"));
    return FALSE;
  }

  for (i = 0; i < encodeUnits; i++) {
    if ((i == encodeUnits - 1) && (padBytes > 0)) {
      ///
      /// Encode last piece, with padding
      ///
      encodeBuf[0]  = *pOctet++;
      encodeBuf[1]                                  = (padBytes == 1) ? encodeBuf[1] = *pOctet++ : 0;
      encodeBuf[2] = 0;
      ipos += (3 - padBytes);

      index               = encodeBuf[0] >> 2;
      pPEMString[opos++]  = PEMCodes[index];
      index               = ((encodeBuf[0] & 0x03) << 4) | (encodeBuf[1] >> 4);
      pPEMString[opos++]  = PEMCodes[index];

      if (padBytes == 1) {
        index               = ((encodeBuf[1] & 0x0f) << 2) | (encodeBuf[2] >> 6);
        pPEMString[opos++]  = PEMCodes[index];
        pPEMString[opos++]  = '=';
      } else {
        pPEMString[opos++]  = '=';
        pPEMString[opos++]  = '=';
      }
    } else {
      ///
      /// Encode next 3 bytes
      ///
      encodeBuf[0]  = *pOctet++;
      encodeBuf[1]  = *pOctet++;
      encodeBuf[2]  = *pOctet++;
      ipos += 3;
      index               = encodeBuf[0] >> 2;
      pPEMString[opos++]  = PEMCodes[index];
      index               = ((encodeBuf[0] & 0x03) << 4) | (encodeBuf[1] >> 4);
      pPEMString[opos++]  = PEMCodes[index];
      index               = ((encodeBuf[1] & 0x0f) << 2) | (encodeBuf[2] >> 6);
      pPEMString[opos++]  = PEMCodes[index];
      index               = encodeBuf[2] & 0x3f;
      pPEMString[opos++]  = PEMCodes[index];
    }
  }

  pPEMString[opos]  = 0;
  *pBufLength       = opos;

  return TRUE;
}

#ifdef EFI_DEBUG

VOID
ShowBuffer (
  UINT8  *Message,
  UINT32 Length
  )
/**

  @brief
  This routine displays the debug message in ASCII
  @param[in] Message              Message to be displayed
  @param[in] Length               Length of the message


**/
{
  UINT32  LineBreak;
  UINT32  Index;
  LineBreak = 0;
  Index     = 0;

  while (Length-- > 0) {
    if (LineBreak == 0) {
      DEBUG ((EFI_D_ERROR, "%02x: ", (Index & 0xF0)));
    }

    DEBUG ((EFI_D_ERROR, "%02x ", Message[Index++]));
    LineBreak++;
    if (LineBreak == 16) {
      DEBUG ((EFI_D_ERROR, "\n"));
      LineBreak = 0;
    }

    if (LineBreak == 8) {
      DEBUG ((EFI_D_ERROR, "- "));
    }
  }

  DEBUG ((EFI_D_ERROR, "\n"));
  return ;
}

#endif /// End Of EFI_DEBUG
