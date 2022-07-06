/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  2009 -2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AmtStatusCodePei.c

Abstract:
  Processes ASF messages

--*/

#include <PiPei.h>
#include "Ppi/ReportStatusCodeHandler.h"
#include "Ppi/AmtStatusCode.h"
#include "Library/MeTypeLib.h"

EFI_GUID mEfiPeiRscHandlerPpiGuid = EFI_PEI_RSC_HANDLER_PPI_GUID;

EFI_STATUS
AmtQueuePetMessage (
    IN  EFI_PEI_SERVICES        **PeiServices,
    IN  EFI_STATUS_CODE_TYPE    Type,
    IN  EFI_STATUS_CODE_VALUE   Value
    )
/*++
Routine Description:

  This routine puts PET message to MessageQueue, which will be sent later.

Arguments:

  PeiServices - PeiServices pointer.
  Type        - StatusCode message type.
  Value       - StatusCode message value.

Returns:

  EFI_SUCCESS      - The function completed successfully

--*/
{
    AMT_PET_QUEUE_HOB     *PETQueueHob;
    EFI_STATUS            Status;

    // Create PET queue hob
    Status = (**PeiServices).CreateHob ( PeiServices, \
                                         EFI_HOB_TYPE_GUID_EXTENSION, \
                                         sizeof(AMT_PET_QUEUE_HOB), \
                                         &PETQueueHob );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    PETQueueHob->EfiHobGuidType.Name = gAmtPetQueueHobGuid;
    PETQueueHob->Type = Type;
    PETQueueHob->Value = Value;

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmtStatusCode(
  IN       EFI_PEI_SERVICES            **PeiServices,
  IN       EFI_STATUS_CODE_TYPE        Type,
  IN       EFI_STATUS_CODE_VALUE       Value,
  IN       UINT32                      Instance,
  IN       EFI_GUID                    *CallerId,
  IN       EFI_STATUS_CODE_DATA        *Data
)
/*++
Routine Description:

  Provides an interface that a software module can call to report an ASF PEI status code.

Arguments:

  PeiServices - PeiServices pointer.

  Type     - Indicates the type of status code being reported.

  Value    - Describes the current status of a hardware or software entity.
             This included information about the class and subclass that is
             used to classify the entity as well as an operation.

  Instance - The enumeration of a hardware or software entity within
             the system. Valid instance numbers start with 1.

  CallerId - This optional parameter may be used to identify the caller.
             This parameter allows the status code driver to apply different
             rules to different callers.

  Data     - This optional parameter may be used to pass additional data.

Returns:

  None

--*/
{
    PEI_AMT_STATUS_CODE_PPI         *AmtStatusCode;
    EFI_STATUS                      Status;

    Status = (*PeiServices)->LocatePpi( PeiServices, \
                                        &gPeiAmtStatusCodePpiGuid, \
                                        0, \
                                        NULL, \
                                        &AmtStatusCode );
    if ( EFI_ERROR (Status) ) {
        if (((Type & EFI_STATUS_CODE_TYPE_MASK) != EFI_PROGRESS_CODE) &&
            ((Type & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE)) {
            return EFI_SUCCESS;
        }

        // Register to Hob

        // Create PET queue hob
        AmtQueuePetMessage (PeiServices, Type, Value);

        return EFI_SUCCESS;
    }

    AmtStatusCode->ReportStatusCode (
                     PeiServices,
                     AmtStatusCode,
                     Type,
                     Value,
                     Instance,
                     CallerId,
                     Data
                     );

    return EFI_SUCCESS;
}

EFI_STATUS
AmtStatusInit(
    IN EFI_FFS_FILE_HEADER    *FfsHeader,
    IN CONST EFI_PEI_SERVICES **PeiServices
)
/*++
Routine Description:

  Init routine for PEI ASF StatusCode.

Arguments:

  FfsHeader   - FfsHeader pointer.
  PeiServices - PeiServices pointer.

Returns:

  EFI_SUCCESS      - The function completed successfully

--*/
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  Status = (*PeiServices)->LocatePpi (
             PeiServices,
             &mEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = RscHandlerPpi->Register ((EFI_PEI_RSC_HANDLER_CALLBACK) AmtStatusCode);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
