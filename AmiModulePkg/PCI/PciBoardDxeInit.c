//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//<AMI_FHDR_START>
//
// Name:  PciInitPolicy.c
//
// Description:	
//
//<AMI_FHDR_END>
//**********************************************************************

#include <AmiDxeLib.h>
#include <Token.h>
#include <PciBus.h>
#include <PciHostBridge.h>
#include <Setup.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/AmiBoardInitPolicy.h>
#include <Library/PciAccessCspLib.h>
#include <Library/AmiPciBusLib.h>
#include <Protocol\IioUds.h>
// AptioV server override Start: Skip SMBUS Controller
#include <PchRegs.h>
#include <Cpu/CpuRegs.h>  
// AptioV server override End

#if CRB_OUT_OF_RESOURCE_SUPPORT
#include <Guid/SetupVariable.h>

#define	MAX_NUMBER_OFFBOARD_VGA 4

EFI_IIO_UDS_PROTOCOL      *mIohUds;
SYSTEM_CONFIGURATION      *SystemConfiguration = NULL;
//BOOLEAN     			  gPciOutOfResHit;
BOOLEAN                 AtleastOneVideoFound = FALSE;
BOOLEAN             	OnboardVideo = FALSE;
BOOLEAN             	OffboardVideo = FALSE;

typedef struct _OFFBOARDVGADEVICESTRIED {
    UINT16      VenId;
    UINT16	DevId;
    BOOLEAN	Enabled;
//    UINT64	Length;
} OFFBOARDVGADEVICESTRIED;

UINT8	MinimumResourceRequestedOffboarVGA;
UINT8	NumOffboardVideoPresent;
UINT8	ResourceTypeIndex;
UINTN	FailedResType;

OFFBOARDVGADEVICESTRIED OffboardVideoDevices[MAX_NUMBER_OFFBOARD_VGA];

EFI_STATUS
EnableDisableDevices (
  IN  PCI_DEV_INFO *dev,
  BOOLEAN	*PciOutOfResHit
);

typedef VOID (*DEVICE_PARSER)(
    PCI_DEV_INFO    *Device,
    BOOLEAN	    *PciOutOfResHit
    
);
#endif
//-------------------------------------------------------------------------

typedef struct _RBINDEXLOCATOR {
	CHAR8 *RbAslDevName;
	UINT8 Index;
}RBINDEXLOCATOR;

RBINDEXLOCATOR  rbindexloc[] = {
		{"PCI0",0},
		{"PCI1",1},
		{"PCI2",2},
		{"PCI3",3},
};
//-------------------------------------------------------------------------
//!!!!!!!!!!! PORTING REQUIRED !!!!!!!!!!! PORTING REQUIRED !!!!!!!!!!!*
//!!!!!!!!!!!!!!!! must be maintained for PCI devices!!!!!!!!!!!!!!!!!!*
//-------------------------------------------------------------------------

/*
//TEMPLATE PCI DEVICE INIT FUNCTION START
// <AMI_PHDR_START>
//-------------------------------------------------------------------------
//
// Procedure: DeviceXXXX_Init
//
// Description:
//  This function provide each initial routine in genericsio.c
//
// Input:
//  IN  AMI_SIO_PROTOCOL    *AmiSio - Logical Device's information
//  IN  EFI_PCI_IO_PROTOCOL *PciIo - Read/Write PCI config space
//  IN  SIO_INIT_STEP       InitStep - Initial routine step
//
// Output:
//  EFI_SUCCESS - Initial step sucessfully
//  EFI_INVALID_PARAMETER - not find the initial step
//
// Modified:    Nothing
//
// Referrals:   None
//
// Notes:
//-------------------------------------------------------------------------
// <AMI_PHDR_END>
EFI_STATUS PciDevXXX_Init(
AMI_BOARD_INIT_PROTOCOL		*This,
IN UINTN					*Function,
IN OUT VOID					*ParameterBlock
)

{
//Update Standard parameter block
	AMI_BOARD_INIT_PARAMETER_BLOCK	*Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
	PCI_INIT_STEP        			InitStep=(PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO   					*dev=(PCI_DEV_INFO*)Args->Param1;
    EFI_STATUS  					Status=EFI_UNSUPPORTED;
//---------------------------------
    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_PCI_PARAM_SIG) return EFI_INVALID_PARAMETER;
    if(InitStep>=isPciMaxStep) return EFI_INVALID_PARAMETER;

    switch (InitStep)
    {
	//-------------------------------------------------------------------------
		case isPciGetSetupConfig:
			DEBUG((DEBUG_INFO," (isPciGetSetupConfig); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciSkipDevice:
			DEBUG((DEBUG_INFO," (isPciSkipDevice); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciSetAttributes:
			DEBUG((DEBUG_INFO," (isPciSetAttributes); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciProgramDevice:
			DEBUG((DEBUG_INFO," (isPciProgramDevice); " ));

		break;
	//-------------------------------------------------------------------------
		case isPcieSetAspm:
			DEBUG((DEBUG_INFO," (isPcieSetAspm); " ));

		break;
	//-------------------------------------------------------------------------
		case isPcieSetLinkSpeed:
			DEBUG((DEBUG_INFO," (isPcieSetLinkSpeed); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciGetOptionRom:
			DEBUG((DEBUG_INFO," (isPciGetOptionRom); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciOutOfResourcesCheck:
			DEBUG((DEBUG_INFO," (isPciOutOfResourcesCheck); " ));

		break;
	//-------------------------------------------------------------------------
		case isPciReadyToBoot:
			DEBUG((DEBUG_INFO," (isPciReadyToBoot); " ));

		break;
	//-------------------------------------------------------------------------
    }//switch

    return Status;
}
//TEMPLATE PCI DEVICE INIT FUNCTION END
*/

#if CRB_OUT_OF_RESOURCE_SUPPORT
//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  DoesDeviceUseMmio
//
// Description: Check to see if this device uses this resource type
//
// Input: PCI_DEV_INFO          *Device        - Device to check
//
// Output:  BOOLEAN
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------

BOOLEAN DoesDeviceUseMmio(PCI_DEV_INFO *Device, PCI_BAR_TYPE Type)
{
    UINT8   BarIndex;

    for (BarIndex = 0;
        BarIndex <= PCI_MAX_BAR_NO;
        BarIndex++)
    {
        if (Device->Bar[BarIndex].Type == Type) return TRUE;
    }
    return FALSE;

}
//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  DisableDevResourceType
//
// Description: Disable this device's Resources of the given type
//
// Input: PCI_DEV_INFO          *Device        - Device to check
//
// Output:  None
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------

VOID DisableDevResourceType(PCI_DEV_INFO *Device, PCI_BAR_TYPE Type)
{
    UINT8   BarIndex;

    for (BarIndex = 0;
        BarIndex <= PCI_MAX_BAR_NO;
        BarIndex++)
    {
        if (Device->Bar[BarIndex].Type == Type)
        {
            Device->Bar[BarIndex].Type = tBarUnused;
            Device->Bar[BarIndex].DiscoveredType = tBarUnused;
            Device->Bar[BarIndex].Base = 0;
            Device->Bar[BarIndex].Length = 0;
        }
    }
}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  DisableDevOprom
//
// Description: Disable this device's OpROM BAR
//
// Input: PCI_DEV_INFO          *Device        - Device to check
//
// Output:  None
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------

VOID DisableDevOprom(PCI_DEV_INFO *Device)
{
    UINT8   BarIndex;

    for (BarIndex = 0;
        BarIndex <= PCI_MAX_BAR_NO;
        BarIndex++)
    {
        if (Device->Bar[BarIndex].Offset == 0x30)
        {
            Device->Bar[BarIndex].Type = tBarUnused;
            Device->Bar[BarIndex].DiscoveredType = tBarUnused;
            Device->Bar[BarIndex].Base = 0;
            Device->Bar[BarIndex].Length = 0;
        }
    }
}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  ParseAllDevices
//
// Description: Execute passed in function on all devices behind
//              this bridge.
//
// Input: PCI_BRG_INFO          *Bridge        - Bridge
//        DEVICE_PARSER          Parser         - Parsing Function
//        VOID                  *Context       - any data the parsing
//                                               function needs
//
// Output:  None
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
VOID ParseAllDevices(
    PCI_BRG_INFO            *Bridge,
    BOOLEAN		    *PciOutOfResHit,
    DEVICE_PARSER            Parser
    )
{
    UINTN   i;

    for (i = 0 ; i < Bridge->Bridge.ChildCount ; i++)
    {
        switch (Bridge->Bridge.ChildList[i]->Type)
        {
            case    tPci2PciBrg:
            case    tPci2CrdBrg:
                //call recursively on next-level bridges
                    ParseAllDevices(
                    (PCI_BRG_INFO*)(Bridge->Bridge.ChildList[i]),
                    PciOutOfResHit,
                    Parser
                    );
           default:
                //call parse function on this device
                Parser(Bridge->Bridge.ChildList[i],PciOutOfResHit);
        }
    }

}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  GetRootBridge
//
// Description: Find Root Bridge this device is on
//
// Input: PCI_DEV_INFO          Devoce         - Concerned Device
//
// Output:  None
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
PCI_DEV_INFO *GetRootBridge(
    PCI_DEV_INFO    *Device
    )
{
    if (Device->Type == tPciRootBrg) return Device;

    return GetRootBridge(Device->ParentBrg);

}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  CheckNumberOfOffBoardVGADevices
//
// Description: Returns the Number of Offboard video devices
//
// Input: PCI_DEV_INFO          Devoce         - Concerned Device
//
// Output:  None
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
VOID CheckNumberOfOffBoardVGADevices(
    PCI_DEV_INFO    *Device,
    BOOLEAN	    *PciOutOfResHit
)
{
      EFI_STATUS                Status = EFI_SUCCESS;
      static UINT8		Count = 0 ;
      
      if ((Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x00) ||
          (Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x80) ||
          (Device->Class.BaseClassCode == 0x04 && Device->Class.SubClassCode == 0x00))
      	      {
	      // Offboard Video devices
	      	      if (!(Device->Attrib & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE))
	      	      {
	      		      DEBUG((DEBUG_INFO, "\n Offboard Video device : VENDDORID=%x DEVICEID=%x\n",Device->DevVenId.VenId,Device->DevVenId.DevId));		      
	      		      OffboardVideoDevices[Count].VenId = Device->DevVenId.VenId;
	      		      OffboardVideoDevices[Count].DevId = Device->DevVenId.DevId;
	      		      DEBUG((DEBUG_INFO, "\n Offboard Video device information : VENDDORID=%x DEVICEID=%x\n",OffboardVideoDevices[Count].VenId,OffboardVideoDevices[Count].DevId));		      
	      		      Count = Count + 1;
	      
	      	      }
      	      }
      
      NumOffboardVideoPresent = Count;

}


//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  checkVideoDevice
//
// Description: Find VideoDevice on the corresponding IOH
//
// Input: PCI_DEV_INFO          Devoce         - Concerned Device
//
// Output:  SUCCESS - SKIP THIS VIDEO DEVICE,UNSUPPORTED - DON"T SKIP THIS VIDEO
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
EFI_STATUS
checkVideoDevice (
IN  PCI_DEV_INFO *dev
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN               SocketNum;

  if (SystemConfiguration->TargetVga == 0)
  {
    SocketNum = 0 ;
  }

  if (SystemConfiguration->TargetVga == 1)
  {
    SocketNum = 1 ;
  }

  if (SystemConfiguration->TargetVga == 2)
  {
    SocketNum = 2 ;
  }

  if (SystemConfiguration->TargetVga == 3)
  {
    SocketNum = 3 ;
  }

  DEBUG((DEBUG_INFO,"\n Inside CheckVideoDevice() : Bus=%x Dev=%x Fun=%x\n",dev->Address.Addr.Bus,dev->Address.Addr.Device,dev->Address.Addr.Function));
  if ((dev->Address.Addr.Bus >= mIohUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusBase) && (dev->Address.Addr.Bus <= mIohUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusLimit))
  {
      if (dev->Attrib & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)
      {
          OnboardVideo = TRUE;
      }
      else
      {
          OffboardVideo = TRUE;
      }
      
      if (SystemConfiguration->VideoSelect == 1 && OnboardVideo) // Onboard
      {
          AtleastOneVideoFound = TRUE;
          DEBUG((DEBUG_INFO,"\n VGA Selection in Setup is :: Onboard\n"));
          return EFI_UNSUPPORTED; // Do not skip the device
      }

      if (SystemConfiguration->VideoSelect == 2 && OffboardVideo) // Offboard
      {
          AtleastOneVideoFound = TRUE;
          DEBUG((DEBUG_INFO,"\n VGA Selection in Setup :: Offboard\n"));
          return EFI_UNSUPPORTED; // Do not skip the device
      }
  }

  else
      return EFI_SUCCESS; // SKIP the device

  return Status;
}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  EnableDisableResources
//
// Description: Disable the current device's MMIO/IO if it is present and
// on a ROotBridge .
//
// Input:           Device          - current device (not necessarily problematic device)
//                  Context         - Device path of current slot
//
// Output:  EFI_STATUS
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
VOID EnableDisableResources(
    PCI_DEV_INFO    *Device,
    BOOLEAN	    *PciOutOfResHit
)
{

      EFI_STATUS                  Status = EFI_SUCCESS;
      UINT8		          OffboardVideoIndex = 0;

   
    if (*PciOutOfResHit) return;

    if (Device->Address.Addr.Bus == 0x00)
    {
	  DEBUG((DEBUG_INFO,"\nBUS 0 Devices won't be SKIPPED\n"));
	  return ;   // Do not skip the device
    }

    else
    {

      if (Device->Class.BaseClassCode == 0x06 ) // this is the bridge device
      {
          DEBUG((DEBUG_INFO,"\nBridge Device won't be SKIPPED\n"));
          return ;    // Do not skip the device
      }


      if (AtleastOneVideoFound == TRUE)
      {
        if ((Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x00) ||
            (Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x80) ||
            (Device->Class.BaseClassCode == 0x04 && Device->Class.SubClassCode == 0x00))
        {
              Status = EFI_SUCCESS; // SKIP THE VIDEO
              DEBUG((DEBUG_INFO, "\nDisabling the Video device : VENDDORID=%x DEVICEID=%x\n",Device->DevVenId.VenId,Device->DevVenId.DevId));

        }
      }

      else
      {
        if ((Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x00) ||
            (Device->Class.BaseClassCode == 0x03 && Device->Class.SubClassCode == 0x80) ||
            (Device->Class.BaseClassCode == 0x04 && Device->Class.SubClassCode == 0x00))
        {
        	      Status = checkVideoDevice(Device);
// If VGA selection is Offboard and even after enabling one offboard VGA card and disabled the other Offboard VGA cards
// System may not boot because the Enabled VGA card requests more resource.
// Will find one VGA card and try to boot to setup        	      
        	      if (OffboardVideo && (NumOffboardVideoPresent > 1))
        	      {
        			  for (OffboardVideoIndex = 0 ; OffboardVideoIndex < NumOffboardVideoPresent ; OffboardVideoIndex++)
        			  {
        		  		  if ((Device->DevVenId.VenId == OffboardVideoDevices[OffboardVideoIndex].VenId) && (Device->DevVenId.DevId == OffboardVideoDevices[OffboardVideoIndex].DevId) && OffboardVideoDevices[OffboardVideoIndex].Enabled == FALSE)
        				  {
        					  OffboardVideoDevices[OffboardVideoIndex].Enabled = TRUE;
        					  break;
        				  }
        				  else
        				  {
        					  Status = EFI_SUCCESS;  
        					  break;
        				  }
        				  
        			  }
        			  if (OffboardVideoIndex >= NumOffboardVideoPresent) 
        			  {
        				  ASSERT_EFI_ERROR (Status);
        				  while (1); // NOT ABLE TO DISABLE VIDEO CARDs which requested more IO. 
        			  }

        	      }
        }
      }
    }

      if (Status != EFI_SUCCESS)
      {
          DEBUG((DEBUG_INFO, "\nEnabling the PCI device : VENDDORID=%x DEVICEID=%x\n",Device->DevVenId.VenId,Device->DevVenId.DevId));
        return ;
      }

      // Does this device use  MMIO/IO?
      if (DoesDeviceUseMmio(Device, tBarMmio32) ||
          DoesDeviceUseMmio(Device, tBarMmio32pf)||
          DoesDeviceUseMmio(Device, tBarIo16))
      {
        // yes, disable its  MMIO/IO
          DEBUG((DEBUG_INFO, "\nDisabling the PCI device : VENDDORID=%x DEVICEID=%x\n",Device->DevVenId.VenId,Device->DevVenId.DevId));
          DisableDevResourceType(Device,tBarMmio32);
          DisableDevResourceType(Device,tBarMmio32pf);
          DisableDevResourceType(Device,tBarIo16);
          *PciOutOfResHit = TRUE;

      }

}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  EnableDisableDevices
//
// Description: If there was a resource overflow, implement the following
//
// Input:           PCI_DEV_INFO *Device - current device (not necessarily problematic device)
//                  UINTN Count          - count of already disabled devices
//                  UINTN LowResType     - MRES_TYPE - type of resource causing overflow
//
// Output:  Enable all devices on BUS0.In other BUS,enable bridge devices and  one video device.
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
EFI_STATUS EnableDisableDevices(
  IN  PCI_DEV_INFO *Device,
  IN  BOOLEAN      *PciOutOfResHit
)
{
    PCI_DEV_INFO                *RootBridge;
    EFI_HANDLE                  *Handle = NULL;

    RootBridge = GetRootBridge(Device);
    // execute MMIO/IO function on all devices behind this root bridge.
// if more than 3 Offboard VGA cards and current logic whichever video card comes first,    
// We are enabling that video card so even disabling the other cards/devices still OutOfResource Occurs.
// New Alogirthm : Find how many Offbaord VGA cards and whichever is requesting the less resources (MMIO/IO) we will enable that device.
    ParseAllDevices ((PCI_BRG_INFO*)RootBridge,PciOutOfResHit,CheckNumberOfOffBoardVGADevices);
    ParseAllDevices ((PCI_BRG_INFO*)RootBridge,PciOutOfResHit,EnableDisableResources);
    return EFI_UNSUPPORTED;

}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  GetSetupData
//
// Description: Reading the Setup Data
//
// Input:   None
//
// Output:  EFI_STATUS
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
EFI_STATUS
GetSetupData ()
{
//  EFI_GUID            SetupGuid = SETUP_GUID;
  UINTN               SetupDataVarSize = sizeof(SYSTEM_CONFIGURATION);
  EFI_STATUS          GetSetStatus=EFI_SUCCESS;

  GetSetStatus = GetEfiVariable(
                                L"IntelSetup",
                                &gEfiSetupVariableGuid,
                                NULL,
                                &SetupDataVarSize,
                                &SystemConfiguration
                               );

   ASSERT_EFI_ERROR (GetSetStatus);
   if (EFI_ERROR(GetSetStatus))
      return GetSetStatus;

    return GetSetStatus;

}

//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:  GetIohUdsStrcuture
//
// Description: Reading the IohUdsStrcuture
// Input :  None
// Output:  EFI_STATUS
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
EFI_STATUS
GetIohUdsStrcuture ()
{
//  EFI_GUID            gEfiIohUdsProtocolGuid = EFI_IOH_UDS_PROTOCOL_GUID;
  EFI_STATUS          GetSetStatus=EFI_SUCCESS;

  GetSetStatus = pBS->LocateProtocol(
                                    &gEfiIioUdsProtocolGuid,
                                    NULL,
                                    &mIohUds
                                    );

  ASSERT_EFI_ERROR (GetSetStatus);
  if (EFI_ERROR(GetSetStatus))
    return GetSetStatus;

  return  GetSetStatus;

}
//---------------------------------------------------------------------------
//<AMI_PHDR_START>
//
// Name:	CrbDefinedPciDeviceTable
//
// Description:	If there was a resource overflow, implement the following
//
// Input:           PCI_DEV_INFO *Device - current device (not necessarily problematic device)
//                  UINTN Count          - count of already disabled devices
//                  UINTN LowResType     - MRES_TYPE - type of resource causing overflow
//
// Output:	Enable all devices on BUS0.In other BUS,enable bridge devices and  one video device.
//
// Referrals:
//
//<AMI_PHDR_END>
//---------------------------------------------------------------------------
EFI_STATUS CrbDefinedPciDeviceTable(
    PCI_DEV_INFO *Device,
    UINTN Count,
    UINTN LowResType,
    BOOLEAN **PciOutOfResHit
    )
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    
    FailedResType = LowResType;

    if (SystemConfiguration == NULL)
    {
        GetSetupData();
    }

    if (mIohUds == NULL)
    {
        GetIohUdsStrcuture();
    }
    DEBUG((DEBUG_INFO,"\n  EnableDisableDevices LowResType%lx FailedResType=%lx\n",LowResType,FailedResType));
    Status =  EnableDisableDevices(Device,*PciOutOfResHit);

    AtleastOneVideoFound = FALSE;
    return Status;
}
#endif

EFI_STATUS PcieRootPortInitCallback (
AMI_BOARD_INIT_PROTOCOL		*This,
IN UINTN					*Function,
IN OUT VOID					*ParameterBlock
)
{
	AMI_BOARD_INIT_PARAMETER_BLOCK	*Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
	PCI_INIT_STEP        			InitStep=(PCI_INIT_STEP)Args->InitStep;
	PCI_DEV_INFO   					*dev=(PCI_DEV_INFO*)Args->Param1;
	EFI_PCI_IO_PROTOCOL		*PciIo = &dev->PciIo ;		//PCI IO Protocol Instance
	EFI_STATUS            Status;
	UINT16                       SltSts;
	UINT16                       SltCnt;	
#if CRB_OUT_OF_RESOURCE_SUPPORT		
	UINTN			     *Count;
	UINTN                        *LowResType;
	BOOLEAN     		      *PciOutOfResHit;
#endif
	
	if(Args->Signature != AMI_PCI_PARAM_SIG) return EFI_INVALID_PARAMETER;
	if(InitStep>=isPciMaxStep) return EFI_INVALID_PARAMETER;
	
	    switch (InitStep)
	    {
		//-------------------------------------------------------------------------
			case isPciProgramDevice:
				DEBUG((DEBUG_INFO," (isPciProgramDevice); " ));
				if ((dev->PciExpress->PcieCap.SlotImpl) && (dev->PciExpress->SlotCap.HpCapable))  { // check whether slot is implemented and hot plug capable.

				     // enable interrupt sources
				     //
				     // Get slot control information and update interrupt sources
				     //
				     // Enable all Hot Plug Interrupts except MRL sensor bit 2 and  Hot Plug Interrupt HPINTEN bit-5. Command completion interrupt is enabled in ASL.
				     //
				     Status = PciIo->Pci.Read(
				         PciIo,
				         EfiPciIoWidthUint16,
				         0xA8,  // offset
				         1,  // width
				         &SltCnt);
				     ASSERT_EFI_ERROR(Status);     
				     DEBUG((DEBUG_INFO," slot control value read %x \n ", SltCnt ));           
				     SltCnt |= (BIT3 | BIT1 | BIT0);  // Presence detect change | Power fault detected | attention button pressed
				     SltCnt |=   (BIT6 | BIT7);       // turn of attention led
				     //           
				     // if no device connected, then turn off power and power indicator bits         
				     //
 				     Status = PciIo->Pci.Read(
				         PciIo,
				         EfiPciIoWidthUint16,
				         0xAA,  // offset
				         1,  // width
				         &SltSts);
				     ASSERT_EFI_ERROR(Status);       
				     DEBUG((DEBUG_INFO," slot status value read %x \n ", SltSts ));                      
				     if ((SltSts & BIT6) == 0) { // if no device connected, then turn off power and power indicator bits 
					    DEBUG((DEBUG_INFO," No device connected, remove power to HP slot \n "));     
				           SltCnt |= (BIT8 | BIT9 | BIT10); //Bit 8:9: Power indicator control (11: Led Off,   01: LEd On)      Bit 10: Power controller control (1: Power off)
				     }
				     Status = PciIo->Pci.Write(
				         PciIo,
 				         EfiPciIoWidthUint16,
				         0xA8,  // offset
				         1,  // width
				         &SltCnt);   
           
				     Status = PciIo->Pci.Read(
   				         PciIo,
				         EfiPciIoWidthUint16,
				         0xA8,  // offset
				         1,  // width
				         &SltCnt);
				     ASSERT_EFI_ERROR(Status);
				     DEBUG((DEBUG_INFO," slot control value after write %x \n ", SltCnt ));               
           
				     Status =  EFI_SUCCESS;       
				     break;	
				}
				Status=EFI_UNSUPPORTED;				
				break;	
				
			case isPciOutOfResourcesCheck:
				DEBUG((DEBUG_INFO," (isPciOutOfResourcesCheck); " ));
#if CRB_OUT_OF_RESOURCE_SUPPORT	
				Count        = (UINTN*)Args->Param2;
				LowResType   = (UINTN*)Args->Param3;
				PciOutOfResHit = (BOOLEAN*)Args->Param4;
			
				Status = CrbDefinedPciDeviceTable(dev,*Count,*LowResType,&PciOutOfResHit);
# else	
				Status=EFI_UNSUPPORTED;
#endif						
				break;
							
			default:
				Status=EFI_UNSUPPORTED;
				DEBUG((DEBUG_INFO," (!!!isPciMaxStep!!!); " ));
	    }//switch
	    
	    return Status;
}

EFI_STATUS RootBrgInit (
AMI_BOARD_INIT_PROTOCOL		*This,
IN UINTN					*Function,
IN OUT VOID					*ParameterBlock
)
{
//Update Standard parameter block
	UINTN 				Index;
	AMI_BOARD_INIT_PARAMETER_BLOCK	*Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
	PCI_INIT_STEP        			InitStep=(PCI_INIT_STEP)Args->InitStep;
	PCI_ROOT_BRG_DATA   		*rb=(PCI_ROOT_BRG_DATA*)Args->Param1;
	EFI_STATUS  					Status=EFI_UNSUPPORTED;
	UINTN   		                                RbIndex;
	ASLR_QWORD_ASD                    *res = (ASLR_QWORD_ASD*)Args->Param3;
//---------------------------------
 	EFI_IIO_UDS_PROTOCOL  		*IIoUds;	
 	for (Index =0; Index < MAX_SOCKET; Index++)
 	{
	    if (Strcmp(rb->RbAslName,rbindexloc[Index].RbAslDevName) == 0 )
	    {
		    RbIndex = rbindexloc[Index].Index;
	    }
 	}
 	
 	Status = pBS->LocateProtocol(
 							&gEfiIioUdsProtocolGuid,
 							NULL,
 							&IIoUds);
 	ASSERT_EFI_ERROR (Status);
 	if (EFI_ERROR(Status))
 		return Status;
 	
    switch (InitStep)
    {
	//-------------------------------------------------------------------------
		case isRbCheckPresence:
			DEBUG((DEBUG_INFO," (isRbCheckPresence); " ));
			rb->NotPresent = !IIoUds->IioUdsPtr->PlatformData.IIO_resource[RbIndex].Valid;
		break;
	//-------------------------------------------------------------------------
		case isRbBusUpdate:
			DEBUG((DEBUG_INFO," (isRbBusUpdate); " ));
			res->_MIN = IIoUds->IioUdsPtr->PlatformData.IIO_resource[RbIndex].BusBase;
			res->_MAX =IIoUds->IioUdsPtr->PlatformData.IIO_resource[RbIndex].BusLimit;
			res->_LEN = (res->_MAX - res->_MIN) + 1;				
		break;
	//-------------------------------------------------------------------------
    }//switch
//---------------------------------
    return Status;
}



// <AMI_PHDR_START>
//-------------------------------------------------------------------------
//
// Procedure: XHCI_B0_D14_F0_Init
//
// Description:
//  This function provide each initial routine in genericsio.c
//
// Input:
//  IN  AMI_SIO_PROTOCOL    *AmiSio - Logical Device's information
//  IN  EFI_PCI_IO_PROTOCOL *PciIo - Read/Write PCI config space
//  IN  SIO_INIT_STEP       InitStep - Initial routine step
//
// Output:
//  EFI_SUCCESS - Initial step sucessfully
//  EFI_INVALID_PARAMETER - not find the initial step
//
// Modified:    Nothing
//
// Referrals:   None
//
// Notes:
//-------------------------------------------------------------------------
// <AMI_PHDR_END>
EFI_STATUS XHCI_B0_D14_F0_Init(
AMI_BOARD_INIT_PROTOCOL         *This,
IN UINTN                                        *Function,
IN OUT VOID                                     *ParameterBlock
)

{
        //Update Standard parameter block
        AMI_BOARD_INIT_PARAMETER_BLOCK  *Args=(AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
        PCI_INIT_STEP                           InitStep=(PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO                                        *dev=(PCI_DEV_INFO*)Args->Param1;
    EFI_STATUS                                          Status=EFI_UNSUPPORTED;
//---------------------------------
    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_PCI_PARAM_SIG) return EFI_INVALID_PARAMETER;
    if(InitStep>=isPciMaxStep) return EFI_INVALID_PARAMETER;

    //XHCI driver of Grangeville fails if it uses address above 4G.
    //So force XHCI device to use only Below 4G Address Space...
    if(InitStep==isPciGetSetupConfig){
      if((dev->Address.Addr.Bus == 0) && ( dev->Address.Addr.Device == 0x14) && ( dev->Address.Addr.Function == 0))
        dev->DevSetup.Decode4gDisable=TRUE;
      Status=EFI_SUCCESS;
    }
    
    return Status;
    
}

// AptioV Server Override Start: Skip SMBUS Controller
/**
    Skips the specified device from PCI enumeration. Bus, device and 
    function numbers of the device are used to skip device.
    
    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS        When device is called with skip device and matches
                                                   specific bus, device and function numbers of device.
                                EFI_UNSUPPORTED    When device is not called with skip device or is
                                                   not matched with specific bus, device and function numbers of device.

**/
EFI_STATUS 
EFIAPI
PciPortSkipDeviceInit (
  IN      AMI_BOARD_INIT_PROTOCOL     *This,
  IN      UINTN                       *Function,
  IN OUT  VOID                        *ParameterBlock
)
{
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    PCI_DEV_INFO                    *Dev = (PCI_DEV_INFO*)Args->Param1;
    UINT64    MsrData;
    
    if(Args->InitStep == isPciSkipDevice) {

        // Skip SMBUS device from PCI Bus enumeration.
        if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && \
            (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SMBUS) && \
            (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SMBUS)) {
           
             MsrData = AsmReadMsr64(EFI_PLATFORM_INFORMATION);
             if (MsrData & BIT63) {
                  DEBUG((DEBUG_ERROR, "skip smbus device\n"));
                  return EFI_SUCCESS;
             }
        }       
    }
    return EFI_UNSUPPORTED;
}
// AptioV Server Override End: 
