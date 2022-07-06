/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  OemErrorHandler.h

Abstract:
  This file will contain all definitions related to OEM hooks 

------------------------------------------------------------------*/
	
#ifndef _IIO_ERROR_HANDLER_H
#define _IIO_ERROR_HANDLER_H

#define R_MIERRSV                                 0x90                          // Misc Error Severity Register
#define R_PCIERRSV                                0x94                          // PCI-E Error Severity Map Register
#define   V_PCIERRSV0                               00                          // Error Severity 0
#define   V_PCIERRSV1                               01                          // Error Severity 1
#define   V_PCIERRSV2                               02                          // Error Severity 2
#define R_SYSMAP                                  0x9C                          // System Error Event Map Register
#define   V_SYSMAP_NoInband                         000                         // No inband message
#define   V_SYSMAP_SMI_PMI                          001                         // Generate SMI/PMI
#define   V_SYSMAP_NMI                              002                         // Generate NMI
#define   V_SYSMAP_CPEI                             003                         // Generate CPEI
#define   V_SYSMAP_MCA                              004                         // Generate MCA
#define R_ERRPINCTL                               0xA4
#define   V_ERRPINCTL_DIS                           00
#define   V_ERRPINCTL_DATA                          01
#define   V_ERRPINCTL_SEV                           02
#define   V_ERRPINCTL_RSVD                          03

//DMI settings
#define   R_DMI_DEVICE_CONTROL_OFFSET               0x60
#define   R_DMI_DEVICE_STATUS_OFFSET                0x62
#define   V_DMI_AER_CAPID                           0xB
#define   V_DMI_AER_VSEC_CAPID                      0x4

#define   V_NTB_AER_CAPID                           0xB
#define   V_NTB_AER_VSEC_CAPID                      0x4
#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)



#define PCIE_PORT_1A_DEV    0x01
#define PCIE_PORT_1A_FUNC   0x00
#define PCIE_PORT_1B_DEV    0x01
#define PCIE_PORT_1B_FUNC   0x01

#define PCIE_PORT_2A_DEV    0x02
#define PCIE_PORT_2A_FUNC   0x00
#define PCIE_PORT_2B_DEV    0x02
#define PCIE_PORT_2B_FUNC   0x01
#define PCIE_PORT_2C_DEV    0x02
#define PCIE_PORT_2C_FUNC   0x02
#define PCIE_PORT_2D_DEV    0x02
#define PCIE_PORT_2D_FUNC   0x03

#define PCIE_PORT_3A_DEV    0x03
#define PCIE_PORT_3A_FUNC   0x00
#define PCIE_PORT_3B_DEV    0x03
#define PCIE_PORT_3B_FUNC   0x01
#define PCIE_PORT_3C_DEV    0x03
#define PCIE_PORT_3C_FUNC   0x02
#define PCIE_PORT_3D_DEV    0x03
#define PCIE_PORT_3D_FUNC   0x03

#define   V_UNKNOWN 00
#define   V_SCI     01
#define   V_NMI     V_UNKNOWN
#define   MAX_NUM_ERR (MAX_SOCKET * NUMBER_PORTS_PER_SOCKET)


#ifndef EFI_DEADLOOP 
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif

typedef struct {
  UINT8   UrMaskSet;
  UINT8   BusNum;
  UINT8   Device;
  UINT8   Function;
  UINT8   NmiSciFlag;
  UINT32  HdrLog1;
} SRIOV_UR_ERR_DEV_INFO;


EFI_STATUS
ElogIohErrorHandler (
  VOID
  );

EFI_STATUS
NmiSciUpdateHandler (
  VOID
  );

BOOLEAN
CheckForIIOErrors (
  VOID
);


VOID
InitErrDevInfo (
  VOID
  );

VOID
InitIOHInfo(
  VOID
  );

VOID
ClearIIOErrorRegisterStatus (
  UINT8 SktId
  );



/*BOOLEAN
PciErrLibLERClearStatus(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  );*/

EFI_STATUS
ProcessIIOUncorrectedError(
  VOID
);

EFI_STATUS
IioDmiErrorEnable (
  VOID
  );

VOID
ClearIioPcieErrorsRegisterStatus(
VOID
);

VOID
EnableIIOExtndPcieErrors(
VOID
);
VOID
IioPcieErrorEnableOnNTB(
IN UINT8 Bus,
IN UINT8 Device,
IN UINT8 Function
);

VOID
IohPcieNtbErrorHandler(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  );
  
EFI_STATUS
WheaHESTCallBack(  
  CONST EFI_GUID  *Protocol,
  VOID            *Interface,
  EFI_HANDLE      Handle  
 );

BOOLEAN
IioPcieErrorReportingEnabled(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
);


#endif


