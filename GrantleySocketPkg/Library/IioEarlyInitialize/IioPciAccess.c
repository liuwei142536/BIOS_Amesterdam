/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014   Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioPciAccess.c

Abstract:

  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

--*/

#include "PciAccess.h"
#include "MmioAccess.h"
#include "IioMisc.h"
#include "SysFunc.h"
#ifndef IA32
#include <Library/PciExpressLib.h>
#include <Library/S3BootScriptLib.h>
#endif


/*++

Routine Description:
  Update 32-bits PCIe address memory with new Data 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite32(
  IIO_GLOBALS       *IioGlobalData,
  UINT64            Address,
  UINT32            Data
  )
{
#ifdef IA32
  IioMmioWrite32(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address), Data);
#else
  PciExpressWrite32(Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
#endif
}

/*++

Routine Description:
  Update 16-bits PCIe address memory with new Data 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite16(
  IIO_GLOBALS       *IioGlobalData,
  UINT64            Address,
  UINT16            Data
  )
{
#ifdef IA32
  IioMmioWrite16(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address), Data);
#else
  PciExpressWrite16(Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
#endif
}

/*++

Routine Description:
  Update 8-bits PCIe address memory with new Data 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioPciExpressWrite8(
   IIO_GLOBALS    *IioGlobalData,
   UINT64         Address,
   UINT8          Data
  )
{
#ifdef IA32
  IioMmioWrite8(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address), Data);
#else
  PciExpressWrite8(Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint8, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
#endif
}

/*++

Routine Description:
  Reads 32-bits PCIe address memory 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  
Returns:
  Data               - Data value

--*/
UINT32
IioPciExpressRead32(
  IIO_GLOBALS       *IioGlobalData,
  UINT64            Address
  )
{
  UINT32 Data;
#ifdef IA32
  Data = IioMmioRead32(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address));
#else
  Data = PciExpressRead32 (Address);
#endif

  return Data;
}

/*++

Routine Description:
  Reads 16-bits PCIe address memory 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  
Returns:
  Data               - Data value

--*/
UINT16
IioPciExpressRead16(
  IIO_GLOBALS       *IioGlobalData,
  UINT64            Address
  )
{
  UINT16 Data;

#ifdef IA32
  Data = IioMmioRead16(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address));
#else
 Data = PciExpressRead16 (Address);
#endif

  return Data;
}

/*++

Routine Description:
  Reads 8-bits PCIe address memory 

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS 
  Address            - Address that needs to be updated
  
Returns:
  Data               - Data value

--*/
UINT8
IioPciExpressRead8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64            Address
  )
{
  UINT8 Data;
#ifdef IA32
  Data = IioMmioRead8(IioGlobalData, (UINT32)(IioGlobalData->IioVData.PciExpressBase + Address));
#else
  Data = PciExpressRead8 (Address);
#endif
 return Data;
}

UINTN
IioGetCpuCsrAddress (
  IIO_GLOBALS    *IioGlobalData,
  UINT8          SocId,
  UINT8          BoxInst,
  UINT32         Offset,
  UINT8          *Size
  )
{
  UINTN    Data;
#ifdef IA32
  Data = (UINT32)GetCpuPciCfgAddress(
                                      IioGlobalData->IioOutData.host,
                                      SocId,
                                      BoxInst,
                                      Offset,
                                      Size
                                    );
#else
  Data = IioGlobalData->IioOutData.CpuCsrAccess->GetCpuCsrAddress( SocId, BoxInst, Offset, Size);
#endif // IA32

  return Data;
}

/**

  Funtion to write CPU CSR with 32-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
VOID
IioWriteCpuCsr32 (
  IIO_GLOBALS       *IioGlobalData,
  UINT8             SocId,
  UINT8             BoxInst,
  UINT32            RegOffset,
  UINT32            Data
  )
{
#ifdef IA32
  WriteCpuPciCfgEx(
                    IioGlobalData->IioOutData.host,
                    SocId,
                    BoxInst,
                    RegOffset,
                    Data
                  );
#else
  UINTN    RegAddr;
  UINT8     Size;

  RegAddr = IioGetCpuCsrAddress (IioGlobalData, SocId, BoxInst, RegOffset, &Size);

  IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr ( SocId, BoxInst, RegOffset, Data);

  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );
#endif
}

// TBD, the IioWriteCpuCsr16/IioWriteCpuCsr8 routine may not be needed since CSR should indicate size itself.
/**

  Function to write CPU CSR with 16-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
/*VOID IioWriteCpuCsr16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Csr,
  UINT16            Data
  )
{
#ifdef IA32
  WriteCpuCsr(IioGlobalData,IioIndex, BoxInst, Csr, Data);
#else
  UINT64    RegAddr;
  UINT8     Size;
  IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(IioIndex, BoxInst, Csr, Data);
  RegAddr = IioGlobalData->IioOutData.CpuCsrAccess->GetCpuCsrAddress (IioIndex, BoxInst, Csr, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, RegAddr, 1, &Data );
#endif
}

/**

  Function to write CPU CSR with 8-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
/*VOID IioWriteCpuCsr8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Csr,
  UINT8             Data
  )
{
#ifdef IA32
  WriteCpuCsr(IioGlobalData,IioIndex, BoxInst, Csr, Data);
#else
  UINT64    RegAddr;
  UINT8     Size;
  IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(IioIndex, BoxInst, Csr, Data);
  RegAddr = IioGlobalData->IioOutData.CpuCsrAccess->GetCpuCsrAddress (IioIndex, BoxInst, Csr, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint8, RegAddr, 1, &Data );
#endif
}

/**

  Funtion to write CPU CSR with 32-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
UINT32
IioReadCpuCsr32 (
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Offset
  )
{
  UINT32            Data;
#ifdef IA32
  Data = ReadCpuPciCfgEx(IioGlobalData->IioOutData.host, IioIndex, BoxInst, Offset);
#else
  Data = IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(IioIndex, BoxInst, Offset);
#endif
 return Data;
}

// TBD, the IioWriteCpuCsr16/IioWriteCpuCsr8 routine may not be needed since CSR should indicate size itself.
/**

  Function to write CPU CSR with 16-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
/*UINT16
IioReadCpuCsr16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Csr
  )
{
  UINT16 Data;
#ifdef IA32
  Data = (UINT16)ReadCpuCsr(IioGlobalData,IioIndex, BoxInst, Csr);
#else
  Data = (UINT16)IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(IioIndex, BoxInst, Csr);
#endif
  return Data;
}

/**

  Function to write CPU CSR with 8-bit data in IIO module.

  @param IIO_GLOBALS -  IioGlobalData structure with CPU CSR access routine pointers
  @param BoxInst     -  Instance in Box type to be accessed
  @param Csr         -  CPU CSR offset to be access
  @param Data        -  Input data to be written to CPU CSR

  @retval VOID

**/
/*UINT8
IioReadCpuCsr8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Csr
  )
{
  UINT8 Data;
#ifdef IA32
  Data = (UINT8)ReadCpuCsr(IioGlobalData,IioIndex, BoxInst, Csr);
#else
  Data = (UINT8)IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(IioIndex, BoxInst, Csr);
#endif
 return Data;
}*/

UINT32
IioBios2PcodeMailBoxWrite(
  IIO_GLOBALS *IioGlobalData,
  UINT8 Iio,
  UINT32 MailBoxCommand,
  UINT32 MailboxData
){
#ifdef IA32
    MailboxData = SendMailBoxCmdToPcode(IioGlobalData->IioOutData.host, Iio, MailBoxCommand, MailboxData);
#else
    MailboxData = IioGlobalData->IioOutData.CpuCsrAccess->Bios2PcodeMailBoxWrite(Iio, MailBoxCommand, MailboxData);
#endif
 return MailboxData;
}
