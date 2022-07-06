#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************

#**********************************************************************
# $Header: /AptioV/SRC/Debugger/Debugger.mak 5     1/11/16 7:19p Sudhirv $
#
# $Revision: 5 $
#
# $Date: 1/11/16 7:19p $
#**********************************************************************
# Revision History
# ----------------
# $Log: /AptioV/SRC/Debugger/Debugger.mak $
# 
# 
#**********************************************************************


#<AMI_FHDR_START>
#-----------------------------------------------------------------------
#
# Name:			CommonDbg.mak
#
# Description:	
#				
#
#-----------------------------------------------------------------------
#<AMI_FHDR_END>
#**********************************************************************

#CURRENTDBG = "AMI_DEBUGGER_SUPPORT"
export EXTERNAL_GENFW_FLAGS:=--keepexceptiontable

FILE_PLATFORM_DSC_COMMON = $(shell $(CAT) $(BUILD_DIR)\Platform.dsc)

CommonRule:
	$(ECHO) "This is a test message from common mak"
	$(ECHO) $(GLOBALWARNING)
	
CommonTokenChecks:
	$(ECHO) "-------------------------------------------------$(EOL)\
 DEBUGGER Enabled - Checking Token Configuration$(EOL)\
------------------------------------------------"
ifeq ("$(Usb3Statuscode_Support)", "1")
$(error  DEBUGGER - requires the Usb3Statuscode_Support SDL token to be 0)
endif
ifeq ("$(OPTIMIZATION)", "1")
$(warning  DEBUGGER WARNING#0x100001 - OPTIMIZATION OFF preferable, OPTIMIZATION ON will provide limited functionality related to variables and source level debugging)
NEEDWARNING = 1
SHOWOPTIMIZATIONWARN = 1
endif
ifneq ("$(DEBUG_MODE)", "1")
$(warning  DEBUGGER ERROR#0x100002 - DEBUG_MODE ON is necessary to enable the source level debugging)
NEEDWARNING = 1
SHOWDEBUGMODEWARN = 1
endif
ifneq ("$(DEBUG_CODE)", "1")
$(warning  DEBUGGER WARNING#0x100003 - DEBUG_CODE ON is necessary to enable the target message redirection)
NEEDWARNING = 1
SHOWDEBUGCODEWARN = 1
endif


AmiDebuggerCommonPortingCheck: 
	$(ECHO) " ----------------------------------------------"
	$(ECHO) " DEBUGGER Enabled - Checking Common Build dependencies"
	$(ECHO) " ----------------------------------------------"
ifneq ("$(SHELL_DEBUGGER_SUPPORT)", "1")
ifneq ("$(USB_3_DEBUG_SUPPORT)", "1")
ifeq ($(findstring DebugPortingLib, $(FILE_PLATFORM_DSC_COMMON)), DebugPortingLib)
	$(ECHO) "Library mapping identified - DebugPortingLib"
else
	$(ECHO) "$(EOL)|  DEBUGGER WARNINGS$(EOL)| --------------------------$(EOL)"
	$(ECHO) "Library Mapping Not Found - DebugPortingLib"
	$(ECHO) "Refer Debugger Porting Guide Document for more information on porting"
	$(error "DEBUGGER ERROR #0x100004- Library Mapping Not Found - DebugPortingLib. Refer Debugger Porting Guide Document for more information on porting. Refer https://cp.ami.com/Shared%20Documents/Aptio%20V/Porting%20Guides/AMI_Debug_for_UEFI_AptioV_Porting_Guide_NDA.pdf") 
endif
endif
ifneq ("$(DYNAMIC_MMCFG_BASE_SUPPORT)", "1")
ifeq ($(findstring PcdPciExpressBaseAddress, $(FILE_PLATFORM_DSC_COMMON)), PcdPciExpressBaseAddress)
	$(ECHO) " | DYNAMIC_MMCFG_BASE_SUPPORT - Turned OFF                                    |"
	$(ECHO) " | Checking PCD availablility - PcdPciExpressBaseAddress available            |"
	@type $(BUILD_DIR)\Platform.dsc | Find "PcdPciExpressBaseAddress"
else
	$(ECHO) "$(EOL)|  DEBUGGER WARNINGS$(EOL)| --------------------------$(EOL)"
	$(ECHO) " | Checking PCD availablility - PcdPciExpressBaseAddress Not Found            |"
	$(ECHO) "DEBUGGER WARNING - DYNAMIC_MMCFG_BASE_SUPPORT OFF requires PcdPciExpressBaseAddress to be defined"
	$(error "DEBUGGER ERROR #0x100005 - DYNAMIC_MMCFG_BASE_SUPPORT OFF requires PcdPciExpressBaseAddress to be defined") 
endif
else
	$(ECHO) " | DYNAMIC_MMCFG_BASE_SUPPORT turned ON"
	$(ECHO) " | Please ensure the following tokens are set in accordance to platform"
	$(ECHO) " |  DBG_CORE05_BUS_NUM        |"
	$(ECHO) " |  DBG_CORE05_DEV_NUM        |"
	$(ECHO) " |  DBG_CORE05_FUNC_NUM       |"
	$(ECHO) " |  DBG_R_IIO_MMCFG_B0        |"
	$(ECHO) " ------------------------------"
SHOWDYNMMIOWARN = 1
NEEDWARNING = 1
endif
endif

CommonEndMessage: End
ifneq ("$(DYNAMIC_MMCFG_BASE_SUPPORT)", "1")
ifeq ($(findstring PcdPciExpressBaseAddress, $(FILE_PLATFORM_DSC_COMMON)), PcdPciExpressBaseAddress)
	$(ECHO) " | INFO 0x100006 -- DYNAMIC_MMCFG_BASE_SUPPORT - Turned OFF                   |"
	$(ECHO) " | Checking PCD availablility - PcdPciExpressBaseAddress available            |"
	$(ECHO) -n " | "
	@type $(BUILD_DIR)\Platform.dsc | Find "PcdPciExpressBaseAddress"
	$(ECHO) " +----------------------------------------------------------------------------+"
endif
endif
ifeq ("$(NEEDWARNING)", "1")
	$(ECHO) " $(EOL) +----------------------------------------------------------------------------+"
	$(ECHO) " |  DEBUGGER WARNINGS                                                         |"
	$(ECHO) " +----------------------------------------------------------------------------+"
ifeq ("$(SHOWDYNMMIOWARN)", "1")
	$(ECHO) " | INFO -- DYNAMIC_MMCFG_BASE_SUPPORT turned ON                               |"
	$(ECHO) " | Please ensure the following tokens are set in accordance to platform       |"
	$(ECHO) " |  DBG_CORE05_BUS_NUM                                                        |"
	$(ECHO) " |  DBG_CORE05_DEV_NUM                                                        |"
	$(ECHO) " |  DBG_CORE05_FUNC_NUM                                                       |"
	$(ECHO) " |  DBG_R_IIO_MMCFG_B0                                                        |"
endif
ifeq ("$(SHOWDEBUGMODEWARN)", "1")
	$(ECHO) " |  WARNING #0x100002 -- DEBUG_MODE turned off - Debugger operations          |$(EOL)\
 |  cannot be performed in this mode                                          |"
endif
ifeq ("$(SHOWOPTIMIZATIONWARN)", "1")
	$(ECHO) " |  WARNING #0x100001-- OPTIMIZATION OFF preferable, OPTIMIZATION ON          |$(EOL)\
 |  will provide limited functionality related to variables                   |$(EOL)\
 |  and source level debugging                                                |"
endif
ifeq ("$(SHOWDEBUGCODEWARN)", "1")
	$(ECHO) " |  WARNING #0x100003 -- DEBUG_CODE - Target Debug Messages will not be       |$(EOL)\
 |	 available when DEBUG_CODE turned OFF                                      |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
endif



