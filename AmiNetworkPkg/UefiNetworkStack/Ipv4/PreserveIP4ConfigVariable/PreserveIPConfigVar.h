#ifndef _PRESERVE_IPCONFIGVAR_H_
#define _PRESERVE_IPCONFIGVAR_H_

#include "AmiDxeLib.h"
#include "AmiLib.h"
#include "Library/UefiLib.h"
#include <Library/MemoryAllocationLib.h>
#include <Token.h>
#include "Efi.h"
#include <Library/BaseMemoryLib.h>

/**
    Function to preserve IP4 Configuration variable data in a global variable of type UINT16*.

    @param IN VOID
    
    @retval OUT EFI_STATUS
*/ 
EFI_STATUS 
PreserveIP4ConfigurationVar (
  void
  );

/**
    Function to restore the IP4 Configuration variable data with values saved in global variable of type UINT16*.

    @param IN VOID
    
    @retval OUT EFI_STATUS
*/ 
EFI_STATUS 
RestoreIP4ConfigurationVar (
  void
  );

#define                 MAC_STRING_SIZE 24  // AABBCCDDEEFF
extern EFI_GUID MACNamesListVarDataGuid;

#endif
