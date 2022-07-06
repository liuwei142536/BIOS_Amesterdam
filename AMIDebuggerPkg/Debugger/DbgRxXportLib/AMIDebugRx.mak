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
# $Log: /AptioV/SRC/AMIDebugRx/AMIDebugRx.mak $
# 
# 
# 
#**********************************************************************


#<AMI_FHDR_START>
#-----------------------------------------------------------------------
#
# Name:			AMIDebugRx.mak
#
# Description:	
#				
#
#-----------------------------------------------------------------------
#<AMI_FHDR_END>
#**********************************************************************

include $(UTILITIES_MAK)

NEEDWARNING = 0
SHOWOPTIMIZATIONWARN = 0
SHOWDEBUGCODEWARN = 0
SHOWDEBUGMODEWARN = 0
SHOWLIBMAPPINGERR = 0
DEBUGGER_MODE = 0
GLOBALWARNING = " +----------------------------------------------------------------------------+$(EOL)\
 | DEBUGGER INFORMATION                                                       |$(EOL)\
 |                                                                            |$(EOL)\
 +----------------------------------------------------------------------------+$(EOL)\
 | TOKEN CONFIGURATION                                                        |$(EOL)\
 +----------------------------------------------------------------------------+\
  "
TOKENCONFIGSTR = ""
FILE_PLATFORM_DSC = $(shell $(CAT) $(BUILD_DIR)\Platform.dsc) 

all: RxAmiDebuggerTokenCheck RxAmiDebuggerPortingCheck RxEndMessage

EdkII: RxAmiDebuggerTokenCheck RxAmiDebuggerPortingCheck

RxAmiDebuggerTokenCheck:
ifeq ("$(AMIDEBUG_RX_SUPPORT)", "1")
DEBUGGER_MODE = 4
ifeq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
$(error AMIDEBUG_RX_SUPPORT ERROR#0x400001 - AMIDEBUG_RX_SUPPORT and SERIAL_DEBUGGER_SUPPORT cannot be turned ON together)
endif
ifneq ("$(AMI_DEBUGGER_SUPPORT)", "1")
ifeq ("$(USB_DEBUG_TRANSPORT)", "0")
$(error AMIDEBUG_RX_SUPPORT ERROR#0x400002 - AMIDEBUG_RX_SUPPORT - AMIDEBUG_RX_SUPPORT - REDIRECTION ONLY requires the USB_DEBUG_TRANSPORT SDL token to be 1)
endif
endif
ifeq ("$(Usb3Statuscode_Support)", "1")
$(error AMIDEBUG_RX_SUPPORT ERROR#0x400003 - AMIDEBUG_RX_SUPPORT - requires the Usb3Statuscode_Support SDL token to be 0)
endif
ifneq ("$(DEBUG_MODE)", "1")
$(warning  AMIDEBUG_RX_SUPPORT WARNING#0x400004 - DEBUG_MODE ON is necessary to enable the source level debugging)
NEEDWARNING = 1
SHOWDEBUGMODEWARN = 1
endif
ifneq ("$(DEBUG_CODE)", "1")
$(warning  AMIDEBUG_RX_SUPPORT WARNING#0x400005 - AMIDEBUG_RX_SUPPORT - DEBUG_CODE ON is necessary to enable the target message redirection)
NEEDWARNING = 1
SHOWDEBUGCODEWARN = 1
endif
#DEBUGGER_MODE = 1
endif

RxAmiDebuggerPortingCheck: RxAmiDebuggerTokenCheck
ifeq ("$(AMIDEBUG_RX_SUPPORT)", "1")
	$(ECHO) " AMIDEBUG_RX_SUPPORT Enabled : Checking Build dependencies"
ifeq ($(findstring DebugPortingLib, $(FILE_PLATFORM_DSC)), DebugPortingLib)
	$(ECHO) "Library mapping identified : DebugPortingLib"
else
	$(ECHO) "$(EOL)|  AMIDEBUG_RX WARNINGS$(EOL)| --------------------------$(EOL)"
	$(ECHO) "Library Mapping Not Found : DebugPortingLib"
	$(ECHO) "Refer Debugger Porting Guide Document for more information on porting"
	$(error "AMIDEBUG_RX ERROR #0x100004 - Library Mapping Not Found : DebugPortingLib. Refer Debugger Porting Guide Document for more information on porting. Refer https://cp.ami.com/Shared%20Documents/Aptio%20V/Porting%20Guides/AMI_Debug_for_UEFI_AptioV_Porting_Guide_NDA.pdf") 
endif
else
	$(ECHO) "AMIDEBUG_RX_SUPPORT : AMIDEBUG_RX_SUPPORT is Disabled"
endif


RxEndMessage: End
ifneq ("$(AMI_DEBUGGER_SUPPORT)", "1")
	$(ECHO) "Debugger has been enabled during the build.. " 
	$(ECHO) $(GLOBALWARNING)
	$(ECHO) " |  AMIDEBUG_RX_SUPPORT   - $(AMIDEBUG_RX_SUPPORT)                                                 |"
	$(ECHO) " |  USB_DEBUG_TRANSPORT   - $(USB_DEBUG_TRANSPORT)                                                 |"
	$(ECHO) " |  DEBUG_MODE            - $(DEBUG_MODE)                                                 |"
ifeq ("$(DEBUG_MODE)", "1")
	$(ECHO) " |  OPTIMIZATION          - $(OPTIMIZATION)                                                 |"
	$(ECHO) " |  DEBUG_CODE            - $(DEBUG_CODE)                                                 |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
	$(ECHO) " |  DEBUGGER MODE                                                             |"
	$(ECHO) " +----------------------------------------------------------------------------+"
ifeq ("$(DEBUGGER_MODE)", "4")
	$(ECHO) " |   DEBUGGER Configuration Mode:  AMIDEBUG_RX REDIRECTION ONLY MODE          |"
	$(ECHO) " |   DebugPortingLib mapping used in project::                                |
	$(ECHO) -n " |"
	@type $(BUILD_DIR)\Platform.dsc | Find "DebugPortingLib"
	$(ECHO) " +----------------------------------------------------------------------------+"
endif
ifeq ("$(NEEDWARNING)", "1")
	$(ECHO) " +----------------------------------------------------------------------------+"
	$(ECHO) " |  DEBUGGER WARNINGS                                                         |"
	$(ECHO) " +----------------------------------------------------------------------------+"
ifeq ("$(SHOWDEBUGCODEWARN)", "1")
	$(ECHO) " |  Warning #0x100003 - DEBUG_CODE - Target Debug Messages will not be        |$(EOL)\
 |  available when DEBUG_CODE turned OFF                                      |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
endif
endif
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
