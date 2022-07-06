/** @file
  Functions in this library instance make use of MMIO functions in IoLib to
  access memory mapped PCI configuration space.

  All assertions for I/O operations are handled in MMIO functions in the IoLib
  Library.

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/PciExpressLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#ifndef EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif


/**
  Assert the validity of a PCI address. A valid PCI address should contain 1's
  only in the low 28 bits.

  @param  A The address to validate.

**/
#define ASSERT_INVALID_PCI_ADDRESS(A) \
  ASSERT (((A) & ~0xfffffff) == 0)

/**
  Registers a PCI device so PCI configuration registers may be accessed after 
  SetVirtualAddressMap().
  
  Registers the PCI device specified by Address so all the PCI configuration 
  registers associated with that PCI device may be accessed after SetVirtualAddressMap() 
  is called.
  
  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  
  @retval RETURN_SUCCESS           The PCI device was registered for runtime access.
  @retval RETURN_UNSUPPORTED       An attempt was made to call this function 
                                   after ExitBootServices().
  @retval RETURN_UNSUPPORTED       The resources required to access the PCI device
                                   at runtime could not be mapped.
  @retval RETURN_OUT_OF_RESOURCES  There are not enough resources available to
                                   complete the registration.

**/
RETURN_STATUS
EFIAPI
DbgPciExpressRegisterForRuntimeAccess (
  IN UINTN  Address
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return RETURN_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
PcdProtocolGuidNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi)
{
#ifndef EFIx64
    PEIDebugData_T *pData;
    DbgGblData_T *DbgData = NULL;
    pData = (PEIDebugData_T *)GetDbgDataBaseAddress();

    if(pData)
    	DbgData = (DbgGblData_T *)(pData->DbgrLocalVar);
#else
    PEIDebugData_Tx64 *pData;
    DbgGblData_Tx64 *DbgData = NULL;
    
    pData = (PEIDebugData_Tx64 *)GetDbgDataBaseAddress();
    if(pData)
        DbgData = (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif
    if(DbgData != NULL)
    {
    DbgData->gPciExpressBaseAddress = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
    }
    return EFI_SUCCESS;
}

static EFI_PEI_NOTIFY_DESCRIPTOR mgPcdProtocolGuidNotifyList = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPcdProtocolGuid,
    PcdProtocolGuidNotifyCallback
};

EFI_STATUS
EFIAPI
InitPeiDbgPciExpressLib(
        IN EFI_FFS_FILE_HEADER *FfsHeader,
        IN VOID **PeiServices
        )
{
    return (**(EFI_PEI_SERVICES**)PeiServices).NotifyPpi ((EFI_PEI_SERVICES **)PeiServices, &mgPcdProtocolGuidNotifyList);
}

/**
  Gets the base address of PCI Express.
  
  This internal functions retrieves PCI Express Base Address via a PCD entry
  PcdPciExpressBaseAddress.
  
  @return The base address of PCI Express.

**/
VOID*
DbgGetPciExpressBaseAddress (
  VOID
  )
{
#ifndef EFIx64
    PEIDebugData_T *pData;
    DbgGblData_T *DbgData = NULL;
    
    pData = (PEIDebugData_T *)GetDbgDataBaseAddress();
    if(pData)
        DbgData = (DbgGblData_T *)(pData->DbgrLocalVar);
#else
    PEIDebugData_Tx64 *pData;
    DbgGblData_Tx64 *DbgData =NULL;
    
    pData = (PEIDebugData_Tx64 *)GetDbgDataBaseAddress();
    if(pData)
        DbgData = (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif
   
    if((DbgData != NULL)&&(DbgData->gPciExpressBaseAddress)) {
        // This If block should handle the Dynamic Type PCD
        return (VOID*) DbgData->gPciExpressBaseAddress;
    } else {
  		return (VOID*)(UINTN) PcdGet64 (PcdPciExpressBaseAddress);
    }
}

/**
  Reads an 8-bit PCI configuration register.

  Reads and returns the 8-bit PCI configuration register specified by Address.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.

  @return The read value from the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressRead8 (
  IN      UINTN                     Address
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioRead8 ((UINTN) DbgGetPciExpressBaseAddress () + Address);
}

/**
  Writes an 8-bit PCI configuration register.

  Writes the 8-bit PCI configuration register specified by Address with the
  value specified by Value. Value is returned. This function must guarantee
  that all PCI read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  Value   The value to write.

  @return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioWrite8 ((UINTN) DbgGetPciExpressBaseAddress () + Address, Value);
}

/**
  Performs a bitwise OR of an 8-bit PCI configuration register with
  an 8-bit value.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 8-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  OrData  The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressOr8 (
  IN      UINTN                     Address,
  IN      UINT8                     OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioOr8 ((UINTN) DbgGetPciExpressBaseAddress () + Address, OrData);
}

/**
  Performs a bitwise AND of an 8-bit PCI configuration register with an 8-bit
  value.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 8-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressAnd8 (
  IN      UINTN                     Address,
  IN      UINT8                     AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAnd8 ((UINTN) DbgGetPciExpressBaseAddress () + Address, AndData);
}

/**
  Performs a bitwise AND of an 8-bit PCI configuration register with an 8-bit
  value, followed a  bitwise OR with another 8-bit value.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 8-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressAndThenOr8 (
  IN      UINTN                     Address,
  IN      UINT8                     AndData,
  IN      UINT8                     OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAndThenOr8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           AndData,
           OrData
           );
}

/**
  Reads a bit field of a PCI configuration register.

  Reads the bit field in an 8-bit PCI configuration register. The bit field is
  specified by the StartBit and the EndBit. The value of the bit field is
  returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If StartBit is greater than 7, then ASSERT().
  If EndBit is greater than 7, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().

  @param  Address   The PCI configuration register to read.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..7.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..7.

  @return The value of the bit field read from the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressBitFieldRead8 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldRead8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit
           );
}

/**
  Writes a bit field to a PCI configuration register.

  Writes Value to the bit field of the PCI configuration register. The bit
  field is specified by the StartBit and the EndBit. All other bits in the
  destination PCI configuration register are preserved. The new value of the
  8-bit register is returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If StartBit is greater than 7, then ASSERT().
  If EndBit is greater than 7, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If Value is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..7.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..7.
  @param  Value     The new value of the bit field.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressBitFieldWrite8 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT8                     Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldWrite8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           Value
           );
}

/**
  Reads a bit field in an 8-bit PCI configuration, performs a bitwise OR, and
  writes the result back to the bit field in the 8-bit port.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 8-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized. Extra left bits in OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If StartBit is greater than 7, then ASSERT().
  If EndBit is greater than 7, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..7.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..7.
  @param  OrData    The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressBitFieldOr8 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT8                     OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldOr8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           OrData
           );
}

/**
  Reads a bit field in an 8-bit PCI configuration register, performs a bitwise
  AND, and writes the result back to the bit field in the 8-bit register.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 8-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized. Extra left bits in AndData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If StartBit is greater than 7, then ASSERT().
  If EndBit is greater than 7, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..7.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..7.
  @param  AndData   The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressBitFieldAnd8 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT8                     AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAnd8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData
           );
}

/**
  Reads a bit field in an 8-bit port, performs a bitwise AND followed by a
  bitwise OR, and writes the result back to the bit field in the
  8-bit port.

  Reads the 8-bit PCI configuration register specified by Address, performs a
  bitwise AND followed by a bitwise OR between the read result and
  the value specified by AndData, and writes the result to the 8-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized. Extra left bits in both AndData and
  OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If StartBit is greater than 7, then ASSERT().
  If EndBit is greater than 7, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..7.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..7.
  @param  AndData   The value to AND with the PCI configuration register.
  @param  OrData    The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT8
EFIAPI
DbgPciExpressBitFieldAndThenOr8 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT8                     AndData,
  IN      UINT8                     OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAndThenOr8 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData,
           OrData
           );
}

/**
  Reads a 16-bit PCI configuration register.

  Reads and returns the 16-bit PCI configuration register specified by Address.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.

  @return The read value from the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressRead16 (
  IN      UINTN                     Address
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioRead16 ((UINTN) DbgGetPciExpressBaseAddress () + Address);
}

/**
  Writes a 16-bit PCI configuration register.

  Writes the 16-bit PCI configuration register specified by Address with the
  value specified by Value. Value is returned. This function must guarantee
  that all PCI read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  Value   The value to write.

  @return The value written to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioWrite16 ((UINTN) DbgGetPciExpressBaseAddress () + Address, Value);
}

/**
  Performs a bitwise OR of a 16-bit PCI configuration register with
  a 16-bit value.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 16-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  OrData  The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressOr16 (
  IN      UINTN                     Address,
  IN      UINT16                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioOr16 ((UINTN) DbgGetPciExpressBaseAddress () + Address, OrData);
}

/**
  Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit
  value.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 16-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressAnd16 (
  IN      UINTN                     Address,
  IN      UINT16                    AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAnd16 ((UINTN) DbgGetPciExpressBaseAddress () + Address, AndData);
}

/**
  Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit
  value, followed a  bitwise OR with another 16-bit value.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 16-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressAndThenOr16 (
  IN      UINTN                     Address,
  IN      UINT16                    AndData,
  IN      UINT16                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAndThenOr16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           AndData,
           OrData
           );
}

/**
  Reads a bit field of a PCI configuration register.

  Reads the bit field in a 16-bit PCI configuration register. The bit field is
  specified by the StartBit and the EndBit. The value of the bit field is
  returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().
  If StartBit is greater than 15, then ASSERT().
  If EndBit is greater than 15, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().

  @param  Address   The PCI configuration register to read.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..15.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..15.

  @return The value of the bit field read from the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressBitFieldRead16 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldRead16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit
           );
}

/**
  Writes a bit field to a PCI configuration register.

  Writes Value to the bit field of the PCI configuration register. The bit
  field is specified by the StartBit and the EndBit. All other bits in the
  destination PCI configuration register are preserved. The new value of the
  16-bit register is returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().
  If StartBit is greater than 15, then ASSERT().
  If EndBit is greater than 15, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If Value is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..15.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..15.
  @param  Value     The new value of the bit field.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressBitFieldWrite16 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT16                    Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldWrite16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           Value
           );
}

/**
  Reads a bit field in a 16-bit PCI configuration, performs a bitwise OR, and
  writes the result back to the bit field in the 16-bit port.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 16-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized. Extra left bits in OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().
  If StartBit is greater than 15, then ASSERT().
  If EndBit is greater than 15, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..15.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..15.
  @param  OrData    The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressBitFieldOr16 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT16                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldOr16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           OrData
           );
}

/**
  Reads a bit field in a 16-bit PCI configuration register, performs a bitwise
  AND, and writes the result back to the bit field in the 16-bit register.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 16-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized. Extra left bits in AndData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().
  If StartBit is greater than 15, then ASSERT().
  If EndBit is greater than 15, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..15.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..15.
  @param  AndData   The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressBitFieldAnd16 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT16                    AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAnd16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData
           );
}

/**
  Reads a bit field in a 16-bit port, performs a bitwise AND followed by a
  bitwise OR, and writes the result back to the bit field in the
  16-bit port.

  Reads the 16-bit PCI configuration register specified by Address, performs a
  bitwise AND followed by a bitwise OR between the read result and
  the value specified by AndData, and writes the result to the 16-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized. Extra left bits in both AndData and
  OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().
  If StartBit is greater than 15, then ASSERT().
  If EndBit is greater than 15, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..15.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..15.
  @param  AndData   The value to AND with the PCI configuration register.
  @param  OrData    The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT16
EFIAPI
DbgPciExpressBitFieldAndThenOr16 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT16                    AndData,
  IN      UINT16                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAndThenOr16 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData,
           OrData
           );
}

/**
  Reads a 32-bit PCI configuration register.

  Reads and returns the 32-bit PCI configuration register specified by Address.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.

  @return The read value from the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressRead32 (
  IN      UINTN                     Address
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioRead32 ((UINTN) DbgGetPciExpressBaseAddress () + Address);
}

/**
  Writes a 32-bit PCI configuration register.

  Writes the 32-bit PCI configuration register specified by Address with the
  value specified by Value. Value is returned. This function must guarantee
  that all PCI read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  Value   The value to write.

  @return The value written to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioWrite32 ((UINTN) DbgGetPciExpressBaseAddress () + Address, Value);
}

/**
  Performs a bitwise OR of a 32-bit PCI configuration register with
  a 32-bit value.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 32-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  OrData  The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioOr32 ((UINTN) DbgGetPciExpressBaseAddress () + Address, OrData);
}

/**
  Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit
  value.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 32-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressAnd32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAnd32 ((UINTN) DbgGetPciExpressBaseAddress () + Address, AndData);
}

/**
  Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit
  value, followed a  bitwise OR with another 32-bit value.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 32-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address The address that encodes the PCI Bus, Device, Function and
                  Register.
  @param  AndData The value to AND with the PCI configuration register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressAndThenOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData,
  IN      UINT32                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioAndThenOr32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           AndData,
           OrData
           );
}

/**
  Reads a bit field of a PCI configuration register.

  Reads the bit field in a 32-bit PCI configuration register. The bit field is
  specified by the StartBit and the EndBit. The value of the bit field is
  returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().
  If StartBit is greater than 31, then ASSERT().
  If EndBit is greater than 31, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().

  @param  Address   The PCI configuration register to read.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..31.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..31.

  @return The value of the bit field read from the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressBitFieldRead32 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldRead32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit
           );
}

/**
  Writes a bit field to a PCI configuration register.

  Writes Value to the bit field of the PCI configuration register. The bit
  field is specified by the StartBit and the EndBit. All other bits in the
  destination PCI configuration register are preserved. The new value of the
  32-bit register is returned.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().
  If StartBit is greater than 31, then ASSERT().
  If EndBit is greater than 31, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If Value is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..31.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..31.
  @param  Value     The new value of the bit field.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressBitFieldWrite32 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    Value
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldWrite32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           Value
           );
}

/**
  Reads a bit field in a 32-bit PCI configuration, performs a bitwise OR, and
  writes the result back to the bit field in the 32-bit port.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 32-bit PCI configuration register
  specified by Address. The value written to the PCI configuration register is
  returned. This function must guarantee that all PCI read and write operations
  are serialized. Extra left bits in OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().
  If StartBit is greater than 31, then ASSERT().
  If EndBit is greater than 31, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..31.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..31.
  @param  OrData    The value to OR with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressBitFieldOr32 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldOr32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           OrData
           );
}

/**
  Reads a bit field in a 32-bit PCI configuration register, performs a bitwise
  AND, and writes the result back to the bit field in the 32-bit register.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 32-bit PCI configuration register specified by
  Address. The value written to the PCI configuration register is returned.
  This function must guarantee that all PCI read and write operations are
  serialized. Extra left bits in AndData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().
  If StartBit is greater than 31, then ASSERT().
  If EndBit is greater than 31, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..31.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..31.
  @param  AndData   The value to AND with the PCI configuration register.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressBitFieldAnd32 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    AndData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAnd32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData
           );
}

/**
  Reads a bit field in a 32-bit port, performs a bitwise AND followed by a
  bitwise OR, and writes the result back to the bit field in the
  32-bit port.

  Reads the 32-bit PCI configuration register specified by Address, performs a
  bitwise AND followed by a bitwise OR between the read result and
  the value specified by AndData, and writes the result to the 32-bit PCI
  configuration register specified by Address. The value written to the PCI
  configuration register is returned. This function must guarantee that all PCI
  read and write operations are serialized. Extra left bits in both AndData and
  OrData are stripped.

  If Address > 0x0FFFFFFF, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().
  If StartBit is greater than 31, then ASSERT().
  If EndBit is greater than 31, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().
  If AndData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().
  If OrData is larger than the bitmask value range specified by StartBit and EndBit, then ASSERT().

  @param  Address   The PCI configuration register to write.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..31.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..31.
  @param  AndData   The value to AND with the PCI configuration register.
  @param  OrData    The value to OR with the result of the AND operation.

  @return The value written back to the PCI configuration register.

**/
UINT32
EFIAPI
DbgPciExpressBitFieldAndThenOr32 (
  IN      UINTN                     Address,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    AndData,
  IN      UINT32                    OrData
  )
{
  ASSERT_INVALID_PCI_ADDRESS (Address);
  return MmioBitFieldAndThenOr32 (
           (UINTN) DbgGetPciExpressBaseAddress () + Address,
           StartBit,
           EndBit,
           AndData,
           OrData
           );
}

/**
  Reads a range of PCI configuration registers into a caller supplied buffer.

  Reads the range of PCI configuration registers specified by StartAddress and
  Size into the buffer specified by Buffer. This function only allows the PCI
  configuration registers from a single PCI function to be read. Size is
  returned. When possible 32-bit PCI configuration read cycles are used to read
  from StartAdress to StartAddress + Size. Due to alignment restrictions, 8-bit
  and 16-bit PCI configuration read cycles may be used at the beginning and the
  end of the range.

  If StartAddress > 0x0FFFFFFF, then ASSERT().
  If ((StartAddress & 0xFFF) + Size) > 0x1000, then ASSERT().
  If Size > 0 and Buffer is NULL, then ASSERT().

  @param  StartAddress  The starting address that encodes the PCI Bus, Device,
                        Function and Register.
  @param  Size          The size in bytes of the transfer.
  @param  Buffer        The pointer to a buffer receiving the data read.

  @return Size read data from StartAddress.

**/
UINTN
EFIAPI
DbgPciExpressReadBuffer (
  IN      UINTN                     StartAddress,
  IN      UINTN                     Size,
  OUT     VOID                      *Buffer
  )
{
  UINTN   ReturnValue;

  ASSERT_INVALID_PCI_ADDRESS (StartAddress);
  ASSERT (((StartAddress & 0xFFF) + Size) <= 0x1000);

  if (Size == 0) {
    return Size;
  }

  ASSERT (Buffer != NULL);

  //
  // Save Size for return
  //
  ReturnValue = Size;

  if ((StartAddress & 1) != 0) {
    //
    // Read a byte if StartAddress is byte aligned
    //
    *(volatile UINT8 *)Buffer = DbgPciExpressRead8 (StartAddress);
    StartAddress += sizeof (UINT8);
    Size -= sizeof (UINT8);
    Buffer = (UINT8*)Buffer + 1;
  }

  if (Size >= sizeof (UINT16) && (StartAddress & 2) != 0) {
    //
    // Read a word if StartAddress is word aligned
    //
    WriteUnaligned16 ((UINT16 *) Buffer, (UINT16) DbgPciExpressRead16 (StartAddress));

    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + 1;
  }

  while (Size >= sizeof (UINT32)) {
    //
    // Read as many double words as possible
    //
    WriteUnaligned32 ((UINT32 *) Buffer, (UINT32) DbgPciExpressRead32 (StartAddress));

    StartAddress += sizeof (UINT32);
    Size -= sizeof (UINT32);
    Buffer = (UINT32*)Buffer + 1;
  }

  if (Size >= sizeof (UINT16)) {
    //
    // Read the last remaining word if exist
    //
    WriteUnaligned16 ((UINT16 *) Buffer, (UINT16) DbgPciExpressRead16 (StartAddress));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + 1;
  }

  if (Size >= sizeof (UINT8)) {
    //
    // Read the last remaining byte if exist
    //
    *(volatile UINT8 *)Buffer = DbgPciExpressRead8 (StartAddress);
  }

  return ReturnValue;
}

/**
  Copies the data in a caller supplied buffer to a specified range of PCI
  configuration space.

  Writes the range of PCI configuration registers specified by StartAddress and
  Size from the buffer specified by Buffer. This function only allows the PCI
  configuration registers from a single PCI function to be written. Size is
  returned. When possible 32-bit PCI configuration write cycles are used to
  write from StartAdress to StartAddress + Size. Due to alignment restrictions,
  8-bit and 16-bit PCI configuration write cycles may be used at the beginning
  and the end of the range.

  If StartAddress > 0x0FFFFFFF, then ASSERT().
  If ((StartAddress & 0xFFF) + Size) > 0x1000, then ASSERT().
  If Size > 0 and Buffer is NULL, then ASSERT().

  @param  StartAddress  The starting address that encodes the PCI Bus, Device,
                        Function and Register.
  @param  Size          The size in bytes of the transfer.
  @param  Buffer        The pointer to a buffer containing the data to write.

  @return Size written to StartAddress.

**/
UINTN
EFIAPI
DbgPciExpressWriteBuffer (
  IN      UINTN                     StartAddress,
  IN      UINTN                     Size,
  IN      VOID                      *Buffer
  )
{
  UINTN                             ReturnValue;

  ASSERT_INVALID_PCI_ADDRESS (StartAddress);
  ASSERT (((StartAddress & 0xFFF) + Size) <= 0x1000);

  if (Size == 0) {
    return 0;
  }

  ASSERT (Buffer != NULL);

  //
  // Save Size for return
  //
  ReturnValue = Size;

  if ((StartAddress & 1) != 0) {
    //
    // Write a byte if StartAddress is byte aligned
    //
    DbgPciExpressWrite8 (StartAddress, *(UINT8*)Buffer);
    StartAddress += sizeof (UINT8);
    Size -= sizeof (UINT8);
    Buffer = (UINT8*)Buffer + 1;
  }

  if (Size >= sizeof (UINT16) && (StartAddress & 2) != 0) {
    //
    // Write a word if StartAddress is word aligned
    //
    DbgPciExpressWrite16 (StartAddress, ReadUnaligned16 ((UINT16*)Buffer));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + 1;
  }

  while (Size >= sizeof (UINT32)) {
    //
    // Write as many double words as possible
    //
    DbgPciExpressWrite32 (StartAddress, ReadUnaligned32 ((UINT32*)Buffer));
    StartAddress += sizeof (UINT32);
    Size -= sizeof (UINT32);
    Buffer = (UINT32*)Buffer + 1;
  }

  if (Size >= sizeof (UINT16)) {
    //
    // Write the last remaining word if exist
    //
    DbgPciExpressWrite16 (StartAddress, ReadUnaligned16 ((UINT16*)Buffer));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + 1;
  }

  if (Size >= sizeof (UINT8)) {
    //
    // Write the last remaining byte if exist
    //
    DbgPciExpressWrite8 (StartAddress, *(UINT8*)Buffer);
  }

  return ReturnValue;
}
