/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioUniphy.c

Abstract:

  Houses all code related to Programm Uniphy recipe

Revision History:

--*/

#include "IioEarlyInitialize.h"
#ifdef DE_SKU
  #include "IioEVRecipeEntry_BDX_DE.h"
#else
  #include "IioEVRecipeEntry_BDX_EP.h"
#endif
//
// BIT 31 of PortBit field indicates IIO Global Dfx Register
//
#define GLOBAL_DFX_BIT              0x80000000

BOOLEAN
IsValidCpuType (
  IIO_GLOBALS       *IioGlobalData,
  UINT32            CpuType, 
  UINT32            RecipeCpuType
)
/*++

Routine Description:
  Function to map the Processor Types in EvRecipe and CpuPciAccess.h. 

Arguments:
   CpuType  - Processor type of the host
   RecipeCpuType - Processor type of the Register being updated in the EV Recipe

Returns:
  True/False - Returns True if the host CPU type matches the CPU type of the EV Recipe Entry

--*/
{
  UINT8 Result = FALSE;
  switch(CpuType) {
  case CPU_BDX:
    if (RecipeCpuType == BDX_SOCKET){
      Result = TRUE;
    }  
  case CPU_HSX:
    if (RecipeCpuType == HSX_SOCKET){
      Result = TRUE;
    }
  break;
  case CPU_IVT:
    if (RecipeCpuType == IVT_SOCKET){
      Result = TRUE;
    }
  break;
  default:
    break;
  }
  return Result;
}

BOOLEAN
IsValidSkuType (
  UINT16            SkuType, 
  UINT16            RecipeSkuType
)
/*++

Routine Description:
  Function to map the Processor SKUs in EvRecipe and CpuPciAccess.h. 

Arguments:
   SkuType  - Processor sku type of the host whether its an EX or EP or DE
   RecipeSkuType - Processor sku type of the Register being updated in the EV Recipe

Returns:
  True/False - Returns True if the host CPU sku type matches the SKU of the EV Recipe Entry

--*/
{
  UINT8   Result = FALSE;
  if (SkuType == SKU_EXA) {
    if ( RecipeSkuType == (1 << EX_SKU_TYPE)) {
      Result = TRUE;
    }
  } else if (SkuType == SKU_DE || SkuType == SKU_NS) {
    if (RecipeSkuType == (1 << DE_SKU_TYPE)) {
      Result = TRUE;
    }
  } else {
    if (RecipeSkuType == (1 << EP_SKU_TYPE)) {
      Result = TRUE;
    }
  }
  return Result;
}

BOOLEAN
IsValidStepping (
  IIO_GLOBALS      *IioGlobalData,
  UINT8            Iio,
  UINT32           CpuStepping, 
  UINT64           RecipeCpuStepping
)
/*++

Routine Description:
  Function to map the Processor Stepping in EvRecipe and Syshost.h. 

Arguments:
   CpuStepping  - Processor stepping of the host
   RecipeCpuStepping - Processor stepping of the Register being updated in the EV Recipe

Returns:
  True/False - Returns True if the host and EV Recipe Entry steppings match

--*/
{
  BOOLEAN Result = FALSE;

  if( RecipeCpuStepping == WILDCARD_64) { //-1 Indicates Valid for all Steppings
    Result = TRUE;
  } else if(IioGlobalData->IioVData.CpuType == CPU_HSX) {
    if(CpuStepping == A0_REV_HSX) {
      if (RecipeCpuStepping & (1 << A0_STEP)) {
        Result = TRUE;
      }
    } else if(CpuStepping == B0_REV_HSX) {
      if (RecipeCpuStepping & (1 << B0_STEP)) {
        Result = TRUE;
      }
    } else if(CpuStepping == C0_REV_HSX) {
      if (RecipeCpuStepping & (1 << C0_STEP)) {
        Result = TRUE;
      }
    } 
  } else if(IioGlobalData->IioVData.CpuType == CPU_BDX) {
    if(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE) {
      if(CpuStepping == A0_REV_BDX) {  // For BDX-DE , Recipe stepping T0 = Bios stepping A0  
        if (RecipeCpuStepping & ((UINT64)1 << T0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping
      } // A0_REV_BDX

      if(CpuStepping == B0_REV_BDX) {  // For BDX-DE , Recipe stepping U0 = Bios stepping B0  
        if (RecipeCpuStepping & ((UINT64)1 << U0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping
      } // B0_REV_BDX

      if(CpuStepping == C0_REV_BDX) {  // For BDX-DE , Recipe stepping V0/V1 = Bios stepping C0
        if (RecipeCpuStepping & ((UINT64)1 << V0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping
      } // C0_REV_BDX
  
      if((CpuStepping == D0_REV_BDX) || (CpuStepping == A0_REV_BDX_NS)) {  // For BDX-DE , Recipe stepping V2 = Bios stepping D0
                                                                           // For BDX-DE NS, Recipe setting follows V2 
        if (RecipeCpuStepping & ((UINT64)1 << V2_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping
      } // D0_REV_BDX

      if(CpuStepping == Y0_REV_BDX) {  // For BDX-DE , Recipe stepping Y0 = Bios stepping Y0
        if (RecipeCpuStepping & ((UINT64)1 << Y0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping
      } // Y0_REV_BDX
    } else { // CPU_FAMILY BDX- EP/EX
      if(CpuStepping == A0_REV_BDX) {  
        if (RecipeCpuStepping & (1 << A0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping 
      } // A0_REV_BDX

      if(CpuStepping == B0_REV_BDX) {  
        if (RecipeCpuStepping & (1 << B0_STEP)) {
          Result = TRUE;
        } // RecipeCpuStepping 
      } // B0_REV_BDX
    } // CPU_FAMILY BDX-EP
  } // CPU_BDX
  
  return Result;
}

BOOLEAN
IsGlobalDfxEnable(
  UINT32 PortBitMap
)
/*++

Routine Description:
  Function to Check if Global DFX register needs to be updated. 

Arguments:
   PortBitMap - PortBitMap Mask   

Returns:
  True/False - Returns True if Global Dfx is selected or not. Bit 1 of PortBitMask is set.

--*/
{
    BOOLEAN Result = FALSE;

    // Check if BIT31 of PortBitMask is set, if so Global Dfx is selected. 
    if(PortBitMap & GLOBAL_DFX_BIT) {
        Result = TRUE;
    }

    return Result;
}

BOOLEAN
IsValidCpuEntry(
  UINT32     SocBitMap,
  UINT8      Iio
)
/*++

Routine Description:
  Function to check if Recipe Entry is valid/applicable for this cpu. 

Arguments:
   SocBitMap - SocBitMap Mask   
   Iio - Iio    

Returns:
  True/False - Returns True if recipe entry is applicable for cpu/socket or not.

--*/
{
    BOOLEAN Result = FALSE;
    UINT8   CpuMask;

    CpuMask = 1 << Iio; 

    if((SocBitMap == WILDCARD_32) || (CpuMask & SocBitMap)) {
      Result = TRUE;
    }

    return Result;
}

VOID IioRxRecipeSettingsAuto (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   Iio
  )
/*++

Routine Description:
   This function loops through each entry in the IioUniphyRecipe  
   and updates the Registers based on Processor type, subtype, Bitrates and Stepping conditions. 
   This function is called for all ports of all sockets.

Arguments:
   IioGlobalData - IioGlobalData pointer
   Iio - IIO number
   BusNumber - Iio Bus Base 
Returns:
   Status

--*/
{
  UINT32      RegAddr;
  UINT32      Data = 0;
  UINT32      TableIndex;
  UINT32      NumEntries;
  BOOLEAN     SteppingCmp;
  BOOLEAN     CpuTypeCmp;
  BOOLEAN     SkuTypeCmp;
  UINT8       j;
  UINT8       BusNumber;
  UINT8       IioDfxDev;
  UINT8       IioDfxFunc;
  UINT8       Port;
  UINT8       StartDevNo;
  BOOLEAN     GlobalDfxFlag;
  BOOLEAN     ValidCpuEntry;
  UINT32      IioEvRecipeTableSize = 0;
  EV_RECIPE_HEADER *IioEvHdrTablePtr = NULL;
  EV_RECIPE_ENTRY  *IioEvRecipeTablePtr = NULL;

  // Point to the right EV Recipe Table
  //
  if (IioGlobalData->IioVData.CpuType == CPU_BDX) { // BDX-DE/EP
    IioEvHdrTablePtr = (EV_RECIPE_HEADER*) &IioUniphyRecipeVer;
    IioEvRecipeTablePtr = (EV_RECIPE_ENTRY*) &IioUniphyRecipe;
    IioEvRecipeTableSize = sizeof(IioUniphyRecipe);
//Grangeville AptioV Server override - CPP check
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Program Uniphy recipe Revision %a\n",IioEvHdrTablePtr->Revision);
  } else {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Automated uniphy EV recipe not supported for CPUs other BDX-EP and BDX-DE \n");
    IIO_ASSERT(IioGlobalData, FALSE, IioGlobalData->IioVData.CpuType); // halt
  }
//Grangeville AptioV Server override - CPP check
//  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Program Uniphy recipe Revision %a\n",IioEvHdrTablePtr->Revision);
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Program RX Recipe values Start.\n");
  BusNumber = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];  

  // Loop though all the recipe entries in IioEvRecipeTablePtr and apply the recipe if applicable
  NumEntries = IioEvRecipeTableSize/sizeof(EV_RECIPE_ENTRY);

  for (TableIndex = 0; TableIndex < NumEntries; TableIndex++) {

    //Verify if the Processor Type, SubType (SKU) and Stepping match with the EV Recipe Entry
    CpuTypeCmp = IsValidCpuType(IioGlobalData, IioGlobalData->IioVData.CpuType, IioEvRecipeTablePtr[TableIndex].ProcessorType);
    SkuTypeCmp = IsValidSkuType(IioGlobalData->IioVData.SkuType, IioEvRecipeTablePtr[TableIndex].Skus);
    SteppingCmp = IsValidStepping(IioGlobalData, Iio, IioGlobalData->IioVData.CpuStepping, IioEvRecipeTablePtr[TableIndex].Steppings);
    GlobalDfxFlag = IsGlobalDfxEnable(IioEvRecipeTablePtr[TableIndex].PortBitmap);
    ValidCpuEntry = IsValidCpuEntry(IioEvRecipeTablePtr[TableIndex].SocBitmap,Iio);

    // Skip Entry if CPU type, SKU and stepping doesn;t apply to the current CPU settigns.
    if (!(CpuTypeCmp && SkuTypeCmp && SteppingCmp && ValidCpuEntry )){
      continue;
    }
    //get the start device number from the uniphy recipe table entry
    StartDevNo = (IioEvRecipeTablePtr[TableIndex].RegisterAddress >> 15) & 0x1F;

    for(j = 0; j <= NUMBER_PORTS_PER_SOCKET; j++) {

      // Skip port enabling check if GlobalDfx is set
      if (!GlobalDfxFlag){
        // Skip port if port is not selected
        if (!((IioEvRecipeTablePtr[TableIndex].PortBitmap >> j) & BIT0)){
          continue;
        }
      }
                  
      // Calculate the Port number
      if(j >= 2) {
         Port = j - 1;
      } else {
        Port = j;  
      }
            
      // Check if Global Dfx register needs to be updated. 
      if (GlobalDfxFlag) {
         IioDfxDev = PCIE_PORT_GLOBAL_DFX_DEV;
         IioDfxFunc = PCIE_PORT_GLOBAL_DFX_FUNC;
         j = NUMBER_PORTS_PER_SOCKET + 1; // break the loop to continue with the next item in the list.
      } else {
        if(StartDevNo >= PCIE_PORT_0_DFX_DEV){
          IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxDevice;
          IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxFunction;
        }
        else{
          IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device;
          IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function;
        }
      }

      //Compute the register address
      RegAddr = PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, (IioEvRecipeTablePtr[TableIndex].RegisterAddress & 0xFFF));
      //
      // Check register size and read data
      //
      switch (IioEvRecipeTablePtr[TableIndex].RegisterSize) {
      case sizeof (UINT32):
        Data = IioPciExpressRead32 (IioGlobalData, RegAddr);
        break;
      case sizeof (UINT16):
        Data = (UINT32)IioPciExpressRead16 (IioGlobalData, RegAddr);
        break;
      case sizeof (UINT8):
        Data = (UINT32)IioPciExpressRead8 (IioGlobalData, RegAddr);
        break;
      default:
        break;
      } 
 
      //Update the data with the values from the EV Recipe
      Data = ((Data & IioEvRecipeTablePtr[TableIndex].FieldMask) |IioEvRecipeTablePtr[TableIndex].Data);

      //Write the data
      switch (IioEvRecipeTablePtr[TableIndex].RegisterSize) {
      case sizeof (UINT32):
        IioPciExpressWrite32 (IioGlobalData, RegAddr, Data);
        break;

      case sizeof (UINT16):
        IioPciExpressWrite16 (IioGlobalData, RegAddr, (UINT16)Data);
        break;

      case sizeof (UINT8):
        IioPciExpressWrite8 (IioGlobalData, RegAddr, (UINT8)Data);
        break;

      default:
        break;
      }
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  B%x D%x F%x O%xh = 0x%x\n", BusNumber, IioDfxDev, IioDfxFunc, RegAddr, Data);
    }
  }
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Program RX Recipe values End.\n");
  return;
}
/************* IIO Uniphy recipe *****************/

