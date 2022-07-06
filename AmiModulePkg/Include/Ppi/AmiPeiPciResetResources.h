//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
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
  @brief Defines PPI that is used to notify that PCI Enumeration PEIM
  is about to reset PCI device resources.

  The PPI has NULL interface. It is installed by PCI enumeration PEIM
  prior to release of PCI resources. PCI device managing PEIMs can register callback
  on installation of this PPI to perform any required device specific processing prior
  to the PCI device disabling by the PCI enumeration PEIM.
**/

#ifndef __AMI_PEI_PCI_RESET_RESOURCES_PPI__H__
#define __AMI_PEI_PCI_RESET_RESOURCES_PPI__H__


#define AMI_PEI_PCI_RESET_RESOURCES_PPI_GUID \
  {0xfb232edf, 0x6164, 0x483a, {0x94, 0xf4, 0x19, 0xb7, 0xac, 0xf9, 0x21, 0xd2 } }

extern EFI_GUID gAmiPeiPciResetResourcesPpiGuid;

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
