//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2008 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  LegacyRegion.h

Abstract:
  This code supports a the private implementation 
  of the Legacy Region protocol.

--*/

#ifndef LEGACY_REGION_H_
#define LEGACY_REGION_H_

#include <PiDxe.h>
#include <IndustryStandard\Pci.h>
#include <Iio\IioRegs.h>
#include <Setup\IioUniversalData.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/IioUds.h>
#include <Protocol/LegacyRegion2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>


#define LEGACY_REGION_INSTANCE_SIGNATURE  SIGNATURE_32 ('R', 'E', 'G', 'N')

//
// Compile time calculation of the number of elements in the mPamData[]
//
#define PAM_DATA_NO (sizeof (mPamData) / sizeof (mPamData[0]))

#define LEGACY_REGION_INSTANCE_FROM_THIS(this) \
  CR ( \
  this, \
  LEGACY_REGION_INSTANCE, \
  LegacyRegion, \
  LEGACY_REGION_INSTANCE_SIGNATURE \
  )


//
// Three valued logical type.
// Yes, logical systems based on more than two (Boolean) values exist.
//
typedef enum {
  Bool3False    = FALSE,
  Bool3True     = TRUE,
  Bool3DontKnow = TRUE + TRUE,  // Guarantees to be different from TRUE and FALSE
} BOOLEAN3;


#pragma pack(1)

//
// Structure holding
//

typedef struct {
  UINT32  Start;      // Address of the range
  UINT32  Length;     // End Addr of the range
  UINT64  RegOffset;  // PCI config space offset of the controlling register
  UINT8   Read;       // Read Enable bits to write into the RegOffset
  UINT8   Write;      // Write Enable bits to write into the RegOffset
} PAM_DATA;

#pragma pack()

typedef struct {
  UINT32                          Signature;
  EFI_HANDLE                      Handle;
  EFI_LEGACY_REGION2_PROTOCOL      LegacyRegion;
  EFI_HANDLE                      ImageHandle;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT8                           SocketPopulated[MAX_SOCKET];
  UINT32                          CpuSocketId[MAX_SOCKET];
} LEGACY_REGION_INSTANCE;



EFI_STATUS
EFIAPI
LegacyRegionInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description:
  Install Driver to produce Legacy Region protocol.

Arguments:

  ImageHandle  -  The image handle.
  SystemTable  -  The system table.

Returns:

  EFI_SUCCESS  -  Legacy Region protocol installed.
  Other        -  No protocol installed, unload driver.

--*/
;

#endif
