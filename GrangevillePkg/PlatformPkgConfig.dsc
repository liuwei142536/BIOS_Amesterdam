#
# This file contains an 'Intel Peripheral Driver' and is      
# licensed for Intel CPUs and chipsets under the terms of your
# license agreement with Intel or your vendor.  This file may 
# be modified by the user, subject to additional terms of the 
# license agreement                                           
#
#/** @file
# platform configuration file for DEBUG build.
#
# Copyright (c) 2011-2013, Intel Corporation. All rights reserved.<BR>
# This software and associated documentation (if any) is furnished
# under a license and may only be used or copied in accordance
# with the terms of the license. Except as permitted by such
# license, no part of this software or documentation may be
# reproduced, stored in a retrieval system, or transmitted in any
# form or by any means without the express written consent of
# Intel Corporation.
#
#**/
  
#
# TRUE is ENABLE. FALSE is DISABLE.
#

# PCH BIOS build switches configuration
#   TRAD_BIOS_ENABLE  =  TRUE   &  ULT_BIOS_ENABLE  =  TRUE   =>  Single BIOS Image for Traditional and ULT
#   TRAD_BIOS_ENABLE  =  TRUE   &  ULT_BIOS_ENABLE  =  FALSE  =>  BIOS Image ONLY for Traditional  <-  Default
#   TRAD_BIOS_ENABLE  =  FALSE  &  ULT_BIOS_ENABLE  =  TRUE   =>  BIOS Image ONLY for and ULT
#   TRAD_BIOS_ENABLE  =  FALSE  &  ULT_BIOS_ENABLE  =  FALSE  =>  NOT Supported; DO NOT USE
#
  DEFINE TRAD_BIOS_ENABLE = TRUE
  DEFINE ULT_BIOS_ENABLE = FALSE
  DEFINE SUS_WELL_RESTORE_ENABLE = FALSE  # Only for Desktop, ULT and Mobile PCHs
  DEFINE ADSP_ENABLE = FALSE
  DEFINE SERIAL_IO_ENABLE = FALSE        # Only for ULT PCH version not for PCH Servers 
  DEFINE USB_PRECONDITION_ENABLE = TRUE
  DEFINE WDT_SUPPORT_ENABLE = TRUE
  
DEFINE CRB_FLAG_ENABLE                       = FALSE
  DEFINE SV_BIOS_ENABLE                      = FALSE
DEFINE SOFTSDV_BIOS_ENABLE                   = FALSE
DEFINE DEBUG_FLAGS_ENABLE                    = TRUE
DEFINE PERFORMANCE_ENABLE                    = FALSE
DEFINE SPLIT_RECOVERY_FV                     = FALSE
DEFINE RSTE_TEST                             = FALSE
DEFINE PCH_SERVER_BIOS_ENABLE                = FALSE # Enable or Disable Wellsburg RHS Features (EVA)
DEFINE SERVER_BIOS_ENABLE                    = TRUE 
DEFINE RAS_SUPPORT                           = TRUE
DEFINE PFAT_EC_ENABLE                        = FALSE

DEFINE BDX_DE_CPU_ENABLE                     = TRUE
DEFINE GRANGEVILLE_PO_ENABLE                 = FALSE
DEFINE GRANGEVILLE_PLATFORM_ENABLE           = TRUE
DEFINE VAMPIRO_SUPPORT_ENABLE                = FALSE
DEFINE SPARING_SCRATCHPAD_ENABLE             = TRUE
DEFINE NVDIMM_ENABLE                         = FALSE
DEFINE NVMEM_ENABLE                          = FALSE



DEFINE SPS_ENABLE                            = TRUE
DEFINE AMT_ENABLE                            = TRUE
!if $(AMT_ENABLE) == TRUE
DEFINE ICC_ENABLE                            = TRUE
!endif

DEFINE SECURE_BOOT_ENABLE                    = TRUE

#
# If any of ME applications built on ME Kernal is enabled
# enable the ME Kernel support with ME_ENABLE flag.
#

#
# If any of ME applications built on ME Kernal is enabled
# enable the ME Kernel support with ME_ENABLE flag.
#
!if $(AMT_ENABLE) == TRUE || $(SPS_ENABLE) == TRUE || $(SV_BIOS_ENABLE) == TRUE
  DEFINE ME_ENABLE = TRUE
!else
  DEFINE ME_ENABLE = FALSE
!endif

DEFINE SDP_PLATFORM = FALSE 
DEFINE USBDEBUGPORT = FALSE 
DEFINE STORAGE_FEATURE_EN = TRUE 
DEFINE PWP_FEATURE_EN = FALSE 
