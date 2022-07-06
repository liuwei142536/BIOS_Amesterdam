//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file
  Acpi Debug driver.
  Enabling Acpi debugging.

Copyright (c) 2011, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include <PiDxe.h>
#include <Library\BaseLib.h>
#include <Library\BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\DebugLib.h>
#include <Library\PcdLib.h>
#include <Library\DxeServicesLib.h>
#include <Protocol\AcpiTable.h>
#include <IndustryStandard\Acpi.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Guid/StatusCodeDataTypeDebug.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Fix for Security Concern
#include <Library/AmiBufferValidationLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Fix for Security Concern
#define ACPI_DEBUG_STR "INTEL ACPI DEBUG"

//
// ASL NAME structure 
//
#pragma pack(1)
typedef struct {
  UINT8   NameOp;       //  Byte [0]=0x08:NameOp.
  UINT32  NameString;   //  Byte [4:1]=Name of object.
  UINT8   DWordPrefix;  //  Byte [5]=0x0C:DWord Prefix.
  UINT32  Value;        //  0   ; Value of named object.
} NAME_LAYOUT;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8  Signature[16];     // "INTEL ACPI DEBUG"
  UINT32 BufferSize;        // Total size of Acpi Debug buffer including header structure
  UINT32 Head;              // Current buffer pointer for SMM to print out
  UINT32 Tail;              // Current buffer pointer for ASL to input
  UINT8  SmiTrigger;        // Value to trigger the SMI via B2 port
  UINT8  Wrap;              // If current Tail < Head
  UINT16 Reserved;
} ACPI_DEBUG_HEAD;
#pragma pack()

#define AD_SIZE    sizeof(ACPI_DEBUG_HEAD) // This is 0x20

UINT32                          mBufferIndex;
UINT32                          mBufferEnd;
ACPI_DEBUG_HEAD                 *mAcpiDebug = NULL;


/**

  Prints a debug message to the debug output device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in PcdDebugPrintErrorLevel, then print 
  the message specified by Format and the associated variable argument list to 
  the debug output device.

  If Format is NULL, then ASSERT().

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      Format string for the debug message to print.

**/
VOID
EFIAPI
SmmDebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CHAR8        *Format,
  ...
  )
{
  UINT64          Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof (UINT64)];
  EFI_DEBUG_INFO  *DebugInfo;
  UINTN           TotalSize;
  UINTN           Index;
  VA_LIST         Marker;
  UINT64          *ArgumentPointer;
  CHAR8           *Temp;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  Temp = Format;
  //
  // Check driver Debug Level value and global debug level
  //
  //if ((ErrorLevel & PcdGet32(PcdDebugPrintErrorLevel)) == 0) {
  //  return;
  //}

  TotalSize = sizeof (EFI_DEBUG_INFO) + 12 * sizeof (UINT64) + AsciiStrLen (Temp) + 1;
  if (TotalSize > EFI_STATUS_CODE_DATA_MAX_SIZE) {
    return;
  }

  //
  // Then EFI_DEBUG_INFO
  //
  DebugInfo = (EFI_DEBUG_INFO *)Buffer;
  DebugInfo->ErrorLevel = (UINT32)ErrorLevel;

  //
  // 256 byte mini Var Arg stack. That is followed by the format string.
  //
  VA_START (Marker, Format);
  for (Index = 0, ArgumentPointer = (UINT64 *)(DebugInfo + 1); Index < 12; Index++, ArgumentPointer++) {
    WriteUnaligned64(ArgumentPointer, VA_ARG (Marker, UINT64));
  }
  VA_END (Marker);
 
  AsciiStrCpy ((CHAR8 *)ArgumentPointer, Format);

  REPORT_STATUS_CODE_EX (
    EFI_DEBUG_CODE,
    EFI_SOFTWARE_SMM_DRIVER,
    0,
    NULL,
    &gEfiStatusCodeDataTypeDebugGuid, 
    DebugInfo,
    TotalSize
    );   

}


/**
  Software SMI callback for ACPI Debug which is called from ACPI method.

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context         Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS            The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
AcpiDebugSmmCallback (
  IN EFI_HANDLE                  DispatchHandle,
  IN CONST VOID                  *Context,
  IN OUT VOID                    *CommBuffer,
  IN OUT UINTN                   *CommBufferSize
  )
{
  UINT8       Buffer[64];
  CHAR8       *String = NULL;
  UINTN       StrSize = 0;
  UINTN       ExtraSize = 0;

// APTIOV_SERVER_OVERRIDE_RC_START : Fix for Security Concern
  EFI_STATUS  Status;
// APTIOV_SERVER_OVERRIDE_RC_END : Fix for Security Concern
  
// APTIOV_SERVER_OVERRIDE_RC_START : Fix for Security Concern
    Status = AmiValidateMemoryBuffer ((VOID*)mAcpiDebug, (PcdGet32 (PcdAcpiDebugBufferSize)));
    if(EFI_ERROR (Status)){
      DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer Status for mAcpiDebug is %r\n",__FUNCTION__,Status));
      return EFI_SUCCESS;
    }
    Status = AmiValidateMemoryBuffer ((VOID*)mAcpiDebug->Head, ((PcdGet32 (PcdAcpiDebugBufferSize)) - sizeof (ACPI_DEBUG_HEAD)));
    if(EFI_ERROR (Status)){
      DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer Status for mAcpiDebug->Head is %r\n",__FUNCTION__,Status));
      return EFI_SUCCESS;
    }
    Status = AmiValidateMemoryBuffer ((VOID*)mAcpiDebug->Tail, ((PcdGet32 (PcdAcpiDebugBufferSize)) - sizeof (ACPI_DEBUG_HEAD)));
    if(EFI_ERROR (Status)){
      DEBUG ((DEBUG_ERROR,"%a AmiValidateMemoryBuffer Status for mAcpiDebug->Tail is %r\n",__FUNCTION__,Status));
      return EFI_SUCCESS;
    }
// APTIOV_SERVER_OVERRIDE_RC_END : Fix for Security Concern

  if (!mAcpiDebug->Wrap && ((mAcpiDebug->Head >= (UINT32)((UINTN)mAcpiDebug + AD_SIZE)) 
    && (mAcpiDebug->Head < mAcpiDebug->Tail))){
    //
    // If curent ----- buffer + 020
    //                 ...
    //                 ... Head
    //                 ... Data for SMM print
    //                 ... Tail
    //                 ... Vacant for ASL input
    //           ----- buffer end
    //
    // skip NULL block 
    //
    while(((CHAR8 *)(UINTN)mAcpiDebug->Head == '\0') && (mAcpiDebug->Head < mAcpiDebug->Tail)) {
      mAcpiDebug->Head ++;
    } 
    
    if (mAcpiDebug->Head < mAcpiDebug->Tail){
      ZeroMem(Buffer, 64);
      String =  (CHAR8 *)(UINTN)mAcpiDebug->Head;    
      StrSize = AsciiStrLen(String) + 1; //plus \0 
     // APTIOV_SERVER_OVERRIDE_RC_START : To prevent buffer overflow - Fix for security vulnerability
      if(StrSize <= 64){
     // APTIOV_SERVER_OVERRIDE_RC_END : To prevent buffer overflow - Fix for security vulnerability
      CopyMem(Buffer, (VOID *)(UINTN)mAcpiDebug->Head, StrSize);
      
      SmmDebugPrint(EFI_D_ERROR, Buffer);
     // APTIOV_SERVER_OVERRIDE_RC_START : To prevent buffer overflow - Fix for security vulnerability
      }
     // APTIOV_SERVER_OVERRIDE_RC_END : To prevent buffer overflow - Fix for security vulnerability
      mAcpiDebug->Head += (UINT32)StrSize;

      if (mAcpiDebug->Head >= (mAcpiDebug->Tail)){
        //
        // When head == tail, we do nothing in handler
        //
        mAcpiDebug->Head = mAcpiDebug->Tail;
      }    
    }
  } else if (mAcpiDebug->Wrap && ((mAcpiDebug->Head > mAcpiDebug->Tail) 
    && (mAcpiDebug->Head < (UINT32)((UINTN)mAcpiDebug + mAcpiDebug->BufferSize)))){    
    //
    // If curent ----- buffer + 020
    //                 ...
    //                 ... Tail
    //                 ... Vacant for ASL input
    //                 ... Head
    //                 ... Data for SMM print    
    //           ----- buffer end
    //    
     
    while(((CHAR8 *)(UINTN)mAcpiDebug->Head == '\0') && (mAcpiDebug->Head < (UINT32)((UINTN)mAcpiDebug + mAcpiDebug->BufferSize))) {
      mAcpiDebug->Head ++;
    } 
    if (mAcpiDebug->Head < (UINT32)((UINTN)mAcpiDebug + mAcpiDebug->BufferSize)){
      ZeroMem(Buffer, 64);
      String =  (CHAR8 *)(UINTN)mAcpiDebug->Head;    
      StrSize = AsciiStrLen(String) + 1;  //plus \0  
      if ((mAcpiDebug->Head +(UINT32)StrSize) >= ((UINTN)mAcpiDebug + mAcpiDebug->BufferSize)) {
        //
        // We should chop extra string that is out of buffer
        //
        ExtraSize = (mAcpiDebug->Head +(UINT32)StrSize) - ((UINTN)mAcpiDebug + mAcpiDebug->BufferSize);
        StrSize -= ExtraSize;
      }
     // APTIOV_SERVER_OVERRIDE_RC_START : To prevent buffer overflow - Fix for security vulnerability
      if(StrSize <= 64){
     // APTIOV_SERVER_OVERRIDE_RC_END : To prevent buffer overflow - Fix for security vulnerability
      CopyMem(Buffer, (VOID *)(UINTN)mAcpiDebug->Head, StrSize);
      SmmDebugPrint(EFI_D_ERROR, Buffer);
     // APTIOV_SERVER_OVERRIDE_RC_START : To prevent buffer overflow - Fix for security vulnerability
      }
     // APTIOV_SERVER_OVERRIDE_RC_END : To prevent buffer overflow - Fix for security vulnerability
      mAcpiDebug->Head += (UINT32)StrSize;
      if (mAcpiDebug->Head == (UINT32)((UINTN)mAcpiDebug + mAcpiDebug->BufferSize)) {
        //
        // We met end of buffer
        //      
        mAcpiDebug->Wrap = 0;
        mAcpiDebug->Head = (UINT32)((UINTN)mAcpiDebug + AD_SIZE);
      }
    }
  }
  
  return EFI_SUCCESS;
}


/**
  Install ACPI table to system.
**/
VOID
LoadAcpiTables (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  UINTN                         Size;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableKey;
  UINT8                         *CurrPtr;
  UINT32                        *Signature;
  NAME_LAYOUT                   *NamePtr;
  UINT8                         UpdateCounter;

  Status = GetSectionFromFv (
             &gEfiCallerIdGuid,
             EFI_SECTION_RAW,
             0,
             &TableHeader,
             &Size
             );
  ASSERT_EFI_ERROR (Status);

  //
  // This is Acpi Debug SSDT. Acpi Debug should be enabled if we reach here so load the table.
  //
  ASSERT (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == SIGNATURE_64 ('A', 'D', 'e', 'b', 'T', 'a', 'b', 'l'));

  //
  // Patch some pointers for the ASL code before loading the SSDT.
  //

  //
  // count pointer updates so we can stop after all three pointers are patched 
  //
  UpdateCounter = 1;
  for (CurrPtr = (UINT8 *) TableHeader; CurrPtr <= ((UINT8 *) TableHeader + TableHeader->Length), UpdateCounter < 4; CurrPtr++) {
    Signature = (UINT32 *) (CurrPtr + 1);
    //
    // patch DPTR (address of Acpi debug memory buffer)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('D', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = (UINT32)(UINTN)mAcpiDebug;
      UpdateCounter++;
    }
    //
    // patch EPTR (end of Acpi debug memory buffer)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('E', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = (UINT32) mBufferEnd;
      UpdateCounter++;
    }
    //
    // patch CPTR (used as an index that starts after the buffer signature)
    //
    if ((*CurrPtr == AML_NAME_OP) && *Signature == SIGNATURE_32 ('C', 'P', 'T', 'R')) {
      NamePtr = (NAME_LAYOUT *) CurrPtr;
      NamePtr->Value  = (UINT32) mBufferIndex;
      UpdateCounter++;
    }
  }

  //
  // Add the table
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  ASSERT_EFI_ERROR (Status);

  TableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        TableHeader,
                        Size,
                        &TableKey
                        );
  ASSERT_EFI_ERROR (Status);

  return ;
}

/**
  Initialize ACPI Debug. 
  
  @param ImageHandle   - Pointer to the loaded image protocol for this driver
  @param SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS   The driver initializes correctly.
**/
EFI_STATUS
EFIAPI
InitializeAcpiDebug (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  PHYSICAL_ADDRESS            BaseAddressMem;
  EFI_STATUS                  Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT    SwContext;
  EFI_HANDLE                     SwHandle;

  if (PcdGetBool (PcdAcpiDebugEnable)) {
    //
    // Reserve 64kb buffer of system memory to store Acpi Debug data.
    //
    BaseAddressMem = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (PcdGet32(PcdAcpiDebugBufferSize)),
                    &BaseAddressMem
                    );
    ASSERT_EFI_ERROR(Status);

    //
    // Clear the 64kb buffer
    //
    mBufferIndex = (UINT32) BaseAddressMem;
    mBufferEnd = mBufferIndex + PcdGet32(PcdAcpiDebugBufferSize);
    //
    // init ACPI DEBUG buffer to lower case 'x' 
    //
    SetMem ((VOID *)(UINTN)BaseAddressMem, PcdGet32(PcdAcpiDebugBufferSize), 0x78);
    //
    // Clear header of AD_SIZE bytes: signature /current head pointer / current tail pointer /reserved
    //
    SetMem ((VOID *)(UINTN)BaseAddressMem, AD_SIZE, 0);

    //
    // Write a signature to the first line of the buffer, "INTEL ACPI DEBUG".
    //
    mBufferIndex = (UINT32)BaseAddressMem;
    CopyMem ((VOID *)(UINTN)mBufferIndex, ACPI_DEBUG_STR, sizeof(ACPI_DEBUG_STR) - 1);

    //
    // leave the Index after the signature
    //
    mBufferIndex += sizeof (ACPI_DEBUG_HEAD);

    DEBUG ((EFI_D_INFO, "AcpiDebugAddress - 0x%08x\n", BaseAddressMem));
    mAcpiDebug = (ACPI_DEBUG_HEAD *)BaseAddressMem;

    //
    // Load the SSDT ACPI Tables.
    //
    LoadAcpiTables ();


    PcdSet32 (PcdAcpiDebugAddress, (UINT32)BaseAddressMem);
    //
    // Get the Sw dispatch protocol and register SMI callback functions.
    //
    SwDispatch = NULL;
    Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
    ASSERT_EFI_ERROR (Status);
    SwContext.SwSmiInputValue = (UINTN) -1;
    Status = SwDispatch->Register (SwDispatch, AcpiDebugSmmCallback, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    mAcpiDebug->SmiTrigger  = (UINT8) SwContext.SwSmiInputValue;
    mAcpiDebug->BufferSize  = PcdGet32(PcdAcpiDebugBufferSize);
    mAcpiDebug->Head = (UINT32) mBufferIndex;
    mAcpiDebug->Tail = (UINT32) mBufferIndex; 


  }

  return EFI_SUCCESS;
}
