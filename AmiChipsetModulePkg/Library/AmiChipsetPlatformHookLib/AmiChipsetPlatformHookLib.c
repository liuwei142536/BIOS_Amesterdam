//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
    AmiChipsetPlatformHookLib
*/

#include "Token.h"
#include <Base.h>
#include <Library/PlatformHookLib.h>
#include "../../../GrantleySocketPkg/Pei/IioInit/IioDmiPeim.h"

VOID
IioSouthComplexUartInit(
  BOOLEAN EnableSCUart
);

/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not intiailzie the serial port hardware itself.  Instead, it initializes 
  hardware devices that are required for the CPU to access the serial port 
  hardware.  This function may be called more than once.

  @retval RETURN_SUCCESS       The platform specific initialization succeeded.
  @retval RETURN_DEVICE_ERROR  The platform specific initialization could not be completed.
 
**/
RETURN_STATUS
EFIAPI
PlatformHookSerialPortInitialize (
  VOID
  )
{
#ifdef DE_SKU
    IioSouthComplexUartInit(TRUE);
#endif
  return  RETURN_SUCCESS;
}

#ifdef DE_SKU

UINT8
GetUncoreBusNo (
  IN UINT8  IioBusNo
)
{
  CPUBUSNO_IIO_VTD_STRUCT          CpuBusNo;

  CpuBusNo.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (IioBusNo,
                                                      IIO_VTD_DEV,
                                                      IIO_VTD_FUN,
                                                      ONLY_REGISTER_OFFSET(CPUBUSNO_IIO_VTD_REG)));
  return (UINT8)CpuBusNo.Bits.bus1;
}

VOID
IioSouthComplexUartInit(
  BOOLEAN EnableSCUart
)
/*++

Routine Description:

  Enables/Disables SC UART 
  
Arguments:

  EnableSCUart  - TRUE for Enabling , FALSE for Disabling .
  
--*/
{
  IRP_MISC_DFX1_IIO_VTD_BDX_STRUCT          IrpMiscDfx1;
  UART_ENABLE_UBOX_MISC_STRUCT              UboxUartEnable;
  SC_RESET_STS_PCU_FUN1_STRUCT              SCResetSts;
  UINT8                                     UncoreBusNo;

  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IIO_BUS_NUM);

  // Steps to enable/disable SC Uart
  // 1. Check for SC Bypass Mode , if set return i.e. use LPC uart else continue enabling the SC uart
  // 2. Disable/Enable UART Range in IIO first
  // 3. Enable/Disable UART range in UBOX
  
  //
  // 1. Check the SC bypass mode 
  //
  SCResetSts.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                        PCU1_DEV_NUM,
                                                        PCU1_FUNC_NUM,
                                                        ONLY_REGISTER_OFFSET(SC_RESET_STS_PCU_FUN1_REG)));

  if(SCResetSts.Bits.sc_bypass)
    return; // continue with LPC UART if in SC bypass mode else enable the SC Uart
  
  // 
  // 2. Disable/Enable UART Range in IIO first
  //
  IrpMiscDfx1.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (IIO_BUS_NUM,
                                                         IIO_VTD_DEV,
                                                         IIO_VTD_FUN,
                                                         ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)));
  
  IrpMiscDfx1.Bits.disable_io_range_0x3f8_0x3ff = EnableSCUart;
  IrpMiscDfx1.Bits.disable_io_range_0x2f8_0x2ff = EnableSCUart;
 
  PciExpressWrite32 ( PCI_LIB_ADDRESS (IIO_BUS_NUM,
                                       IIO_VTD_DEV,
                                       IIO_VTD_FUN,
                                       ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)), IrpMiscDfx1.Data);

  // 
  // 3. Enable/Disable UART range in UBOX
  //
  UboxUartEnable.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                            UBOX_MISC_DEV_NUM,
                                                            UBOX_MISC_FUNC_NUM,
                                                            ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)));
  UboxUartEnable.Bits.enableuartport0 = EnableSCUart;
  UboxUartEnable.Bits.enableuartport1 = EnableSCUart;

  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                       UBOX_MISC_DEV_NUM,
                                       UBOX_MISC_FUNC_NUM,
                                       ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)),UboxUartEnable.Data);
}
#endif // DE_SKU

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************