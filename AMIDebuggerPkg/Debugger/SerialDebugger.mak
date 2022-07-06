#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
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
# Name:			SerialDebugger.mak
#
# Description:	
#				
#
#-----------------------------------------------------------------------
#<AMI_FHDR_END>
#**********************************************************************

include $(UTILITIES_MAK)
include $(DEBUGGER_DIR)/CommonDbg.mak

NEEDWARNING = 0
SHOWOPTIMIZATIONWARN = 0
SHOWDEBUGCODEWARN = 0
SHOWDEBUGMODEWARN = 0
SHOWLIBMAPPINGERR = 0
SHOWDYNMMIOWARN = 0
DEBUGGER_MODE = 0
GLOBALWARNING = " +----------------------------------------------------------------------------+$(EOL)\
| DEBUGGER INFORMATION                                                       |$(EOL)\
|                                                                            |$(EOL)\
+----------------------------------------------------------------------------+$(EOL)\
| TOKEN CONFIGURATION                                                        |$(EOL)\
+----------------------------------------------------------------------------+"
CURRENTDBG = "SERIAL_DEBUGGER_SUPPORT"
FILE_PLATFORM_DSC = $(shell $(CAT) $(BUILD_DIR)\Platform.dsc)
HWFLOWCONTROL = 0

all: CommonTokenChecks AmiDebuggerTokenCheck AmiDebuggerPortingCheck AmiDebuggerCommonPortingCheck AmiDebuggerPortingCheck2 EndMessage EndMessageDebugPorting CommonEndMessage

EdkII: AmiDebuggerTokenCheck AmiDebuggerPortingCheck AmiDebuggerCommonPortingCheck AmiDebuggerPortingCheck2

AmiDebuggerTokenCheck:
	$(ECHO) "-------------------------------------------------$(EOL)\
 DEBUGGER Enabled - Checking Serial Debugger Token Configuration$(EOL)\
------------------------------------------------"
ifeq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
ifeq ("$(AMIDEBUG_RX_SUPPORT)", "1")
	$(error  DEBUGGER ERROR #0x200001 SERIAL_DEBUGGER_SUPPORT and AMIDEBUG_RX_SUPPORT cannot be turned ON together)
endif
ifeq ("$(USB_DEBUG_TRANSPORT)", "1")
	$(error  DEBUGGER ERROR #0x200002 SERIAL_DEBUGGER_SUPPORT and USB_DEBUG_TRANSPORT cannot be turned ON together)
endif
ifeq ("$(USB_3_DEBUG_SUPPORT)", "1")
	$(error  DEBUGGER ERROR #0x200002 SERIAL_DEBUGGER_SUPPORT and USB_3_DEBUG_SUPPORT cannot be turned ON together)
endif
ifeq ("$(GENERIC_USB_CABLE_SUPPORT)", "1")
	$(error  DEBUGGER ERROR #0x200002 SERIAL_DEBUGGER_SUPPORT and GENERIC_USB_CABLE_SUPPORT cannot be turned ON together)
endif
DEBUGGER_MODE = 3
#$(ECHO) " ----------------------------------------------"
endif


AmiDebuggerPortingCheck: AmiDebuggerTokenCheck
	$(ECHO) "-------------------------------------------------$(EOL)\
 DEBUGGER Enabled - Checking Build dependencies$(EOL)\
------------------------------------------------"
ifeq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
ifeq ($(findstring PcdSerialUseHardwareFlowControl|FALSE, $(FILE_PLATFORM_DSC)), PcdSerialUseHardwareFlowControl|FALSE)
	$(ECHO) "SERIAL DEBUGGER - HW Flow control disabled"
else
	$(ECHO) "SERIAL DEBUGGER - HW Flow control Enabled"
HWFLOWCONTROL = 1
endif
#$(ECHO) " ----------------------------------------------"
else
	$(ECHO) "DEBUGGER Disabled : SERIAL_DEBUGGER_SUPPORT is Disabled"
endif

AmiDebuggerPortingCheck2: 
	$(ECHO)  "Checking the DebugLib mappings"
	@if exist "$(BUILD_DIR)\SerDbgResults.txt" $(RM) $(BUILD_DIR)\SerDbgResults.txt
	@findstr "DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf" $(BUILD_DIR)\Platform.dsc > $(BUILD_DIR)\SerDbgResults.txt
	@if exist "$(BUILD_DIR)\SerDbgResults.txt" @type $(BUILD_DIR)\SerDbgResults.txt
	@if $(words $(shell $(CAT) $(BUILD_DIR)\SerDbgResults.txt)) GTR 2 (\
		$(ECHO) "|   DebugLib Porting  :  Additonal Overrides found      |$(EOL)\
		|   DEBUGGER WARNING #0x200003 : The additional overrides in DebugLib would conflict with Serial debugger. | $(EOL)| SerialDebugger may not be compatible with the current platform |$(EOL)| User may keep DEBUG_CODE OFF to continue using as DebugLib override would not take effect when DEBUG_CODE OFF | $(EOL)"\
	) else (\
		$(ECHO) "|   DebugLib Porting  :  Matches Default          |"\
	)

EndMessage: End
ifeq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
	$(ECHO) "Debugger has been enabled during the build.. " 
	$(ECHO) $(GLOBALWARNING)
	$(ECHO) " |  SERIAL_DEBUGGER_SUPPORT : $(SERIAL_DEBUGGER_SUPPORT)                                               |"
	$(ECHO) " |  PeiDebugger_SUPPORT     : $(PeiDebugger_SUPPORT)                                               |"
	$(ECHO) " |  DEBUG_MODE              : $(DEBUG_MODE)                                               |"
ifeq ("$(DEBUG_MODE)", "1")
	$(ECHO) " |  OPTIMIZATION            : $(OPTIMIZATION)                                               |"
	$(ECHO) " |  DEBUG_CODE              : $(DEBUG_CODE)                                               |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
	$(ECHO) " |  DEBUGGER MODE                                                             |"
	$(ECHO) " +----------------------------------------------------------------------------+"
ifeq ("$(DEBUGGER_MODE)", "1")
	$(ECHO) " |   DEBUGGER Configuration Mode:  USB 2 DEBUGGER MODE                        |"
	$(ECHO) " |   DebugPortingLib mapping used in project::                                |
	$(ECHO) -n " |"
	@type $(BUILD_DIR)\Platform.dsc | Find "DebugPortingLib"
endif
ifeq ("$(DEBUGGER_MODE)", "2")
	$(ECHO) " |   DEBUGGER configuration :  USB 3 DEBUGGER MODE                            |"
endif
ifeq ("$(DEBUGGER_MODE)", "3")
	$(ECHO) " |   DEBUGGER configuration :  SERIAL DEBUGGER MODE                           |"
	$(ECHO) " |   DebugPortingLib mapping used in project::                                |
	$(ECHO) -n " |"
	@type $(BUILD_DIR)\Platform.dsc | Find "DebugPortingLib"
ifeq ("$(HWFLOWCONTROL)", "0")
	$(ECHO) " |   HW Flow Control :  DISABLED                                              |"
else
	$(ECHO) " |   HW Flow Control :  ENABLED                                               |"
	$(ECHO) " |   Please Enable HW flow control at Host End                                |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
endif
endif

EndMessageDebugPorting: End
	$(ECHO) " | Checking the DebugLib mappings                                             |"
	@if exist "$(BUILD_DIR)\SerDbgResults.txt" $(RM) $(BUILD_DIR)\SerDbgResults.txt
	@findstr "DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf" $(BUILD_DIR)\Platform.dsc > $(BUILD_DIR)\SerDbgResults.txt
	@if $(words $(shell $(CAT) $(BUILD_DIR)\SerDbgResults.txt)) GTR 2 (\
		$(ECHO) " |   DebugLib Porting  :  Additonal Overrides found                           |$(EOL)\
		| DEBUGGER WARNING #0x200003 : The additional overrides in DebugLib would    |$(EOL)\
		| conflict with Serial debugger.                                             |$(EOL)\
		| SerialDebugger may not be compatible with the current platform             |$(EOL)\
		| User may keep DEBUG_CODE OFF to continue using as DebugLib override would  |$(EOL)\
		| not take effect when DEBUG_CODE OFF                                        | "\
	) else (\
		$(ECHO) " |   DebugLib Porting  :  Matches Default                                     |"\
	)
    
#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************
