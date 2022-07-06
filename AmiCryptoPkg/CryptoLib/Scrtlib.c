//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
 * Scrtlib.c
 * Standard C Run-time Library(CRT) Interfaces
 * Implements: _strdup; Atoi_ex; Snprintf
 */

#include "includes.h"

#ifdef CONFIG_INTERNAL_X509
/**

  The function shall return a pointer to a new string, 
  which is a duplicate of the string pointed to by s

  @param  s         Source string

  @retval status    pointer to duplicated string

**/
char * _strdup(const char *s)
{
    CHAR8 *src=(CHAR8*)s, *aa=NULL;
    int size=0, maxsize = 0x1000;

    while(*src++ && size++ < maxsize);
    if(size < maxsize) {
        aa = os_malloc(size);
        if(aa) {
            memset(aa, 0, size);
            memcpy((void*)aa, (void*)s, size);
        }
    }
    return aa;
}

/* formatted Atoi func. replaces sscanf*/
/**

  Convert a Null-terminated ASCII decimal string to a value of type UINTN.
  The input string size is limited by str len parameter
  This function returns a value of type UINTN by interpreting the contents
  of the ASCII string String as a decimal number. The format of the input
  ASCII string String is:

                    [spaces] [decimal digits].
                    
  @param  String         Pointer to a CHAR8 string that represents an integer number.
  @param  len            Limits the string length to parse.
  @param  Value          Output. Value translated from String.

  @retval status         non0 - value converted successfully
        
**/
int AtoiEX(const char *s, UINT8 len, int* value)
{
    char ch;
    char str[9];
    if(len>8) len=8; // max int size
    memcpy(str, s, len);
    ch = *(str+len);
    *(str+len) = 0;
    *value = Atoi(str);
    *(str+len) = ch;

    return *value;
}

/**
  This function parses the input string for lowercase s and capital G and replaces them with
  a and g respectively.  The Debug functions treat s as S and they don't recognize G

  @param String to modify the format specifiers

  @return VOID
 */
#if 0
VOID AmiCrypt_ProcessFormatStringtoEdk2(const char *String)
{
    char *StrPtr;

    StrPtr = (char*)String;

    while (*StrPtr != 0)
    {
        // Check for a string modifier first.  if no '%' sign then move to the next character
        if (*StrPtr != '%')
        {
            StrPtr++;
            continue;
        }
        
        StrPtr++;
        // Percent character found.  Check for next character
        if (*StrPtr == 's')
        {
            *StrPtr = 'a';
        }
        else if (*StrPtr == 'G')
        {
            *StrPtr = 'g';
        }
        StrPtr++;
    } // end of while (StrPtr != '%')
}
#endif
/**
   Write formatted data to a string
  
  @param  str             A pointer to the output buffer for the produced Null-terminated 
                          ASCII string.
  @param  size            The size, in bytes, of the output buffer specified by StartOfBuffer.
  @param  format          Null-terminated Unicode format string.
  
  @return int             number of bytes stored in buffer, not counting the terminating null character
 */
int EFIAPI Snprintf(char *str, UINTN size, const char *format, ...)
{
	VA_LIST Marker;
	UINTN   NumberOfPrinted;

	if (str==NULL || format==NULL || size==0) return 0;

	//AMI mod - MDE lib version process '%s' as unicode strings
//	AmiCrypt_ProcessFormatStringtoEdk2(format);
    VA_START (Marker, format);
    NumberOfPrinted = AsciiVSPrint (str, size, format, Marker);
    VA_END (Marker);

	return (int)NumberOfPrinted;
}
#endif /* CONFIG_INTERNAL_X509 */
