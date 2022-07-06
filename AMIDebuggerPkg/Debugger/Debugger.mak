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
# 5     1/11/16 7:19p Sudhirv
# [Category]   	Spec Update
# [Severity]		Minor
# [Description]	AMI Debug for UEFI 23 Release process
# [Files]			Debugger.mak
# 
# 4     1/05/16 7:32p Sudhirv
# [TAG]			EIP249624
# [Category]   	Bug Fix
# [Severity]		Normal
# [Symptom]  	Setting Breakpoint in SMM Code from DXE phase when SMM
# Entry\Exit is ON causes the Host-Target to lose sync
# [RootCause]	Setting Breakpoint in SMM Code from DXE phase when SMM
# Entry\Exit is ON causes the Host-Target to lose sync
# [Solution]		handle the memory access case
# [Files]			Dbgr.c
# DbgrHdr.c
# XhciDebugger.sdl
# xp_layer.c
# usb_xp.c
# XportHdr.c
# PortingLib.h
# DbgXhci.h
# XhciPei.h
# Debugger.sdl
# Debugger.mak
# PeCoffExtraActionLib.c
# PeCoffExtraActionLibSmm.c
# DbgGenUsbXport.c
# 
# 3     6/09/15 2:07a Jekekumarg
# [TAG]			EIP217777
# [Description]	AMI Debug for UEFI - Generate Binary, Test Report and
# Document Update for INT Label 22 
# [Files]			Dbgr.c
# DbgPEIPerformanceLib.c
# DbgDXEPerformanceLib.c
# DbgXhci.h
# XhciPei.h
# UsbHostController.h
# usb.h
# UsbIo.h
# UsbPeim.h
# DbgUsb3Internal.h
# AmiMapping.h
# IncludeLib.cif
# Debugger.sdl
# Debugger.mak
# USB3DriverPkg.zip
# Debugger.cif
# PeCoffExtraActionLib.c
# CommonGenUsbLib.c
# 
# 2     4/30/15 6:20a Jekekumarg
# [TAG]			EIP212213
# [Category]   	Improvement
# [Description]	[USB Debugger] Have a token to make debugger module
# connection works only when it's at DXE phase
# [Files]			Debugger.mak
# 
# 1     9/21/13 4:25a Sudhirv
# Added for AMI Debug for UEFI 3.01.0006 Label.
# 
# 2     11/21/12 10:38a Sudhirv
# Changed Elink Name to avoid clash with SIO modules.
# 
# 1     11/19/12 7:42a Sudhirv
# [TAG]     EIP106836
# [Category] Function Request
# [Severity] Normal
# [Symptom] Make changes to AMIDebugRx\Debugger for chipset porting
# through ELink
# [Solution] Issue is fixed, Now chipset porting is done by ELink.
# 
# 
#**********************************************************************


#<AMI_FHDR_START>
#-----------------------------------------------------------------------
#
# Name:			Debugger.mak
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
CURRENTDBGTYPE = "AMI_DEBUGGER_SUPPORT1"
GLOBALWARNING = " +----------------------------------------------------------------------------+$(EOL)\
| DEBUGGER INFORMATION                                                       |$(EOL)\
|                                                                            |$(EOL)\
+----------------------------------------------------------------------------+$(EOL)\
| TOKEN CONFIGURATION                                                        |$(EOL)\
+----------------------------------------------------------------------------+"

FILE_PLATFORM_DSC = $(shell $(CAT) $(BUILD_DIR)\Platform.dsc)

all: CommonTokenChecks AmiDebuggerTokenCheck AmiDebuggerCommonPortingCheck EndMessage CommonEndMessage

EdkII: AmiDebuggerTokenCheck AmiDebuggerCommonPortingCheck

AmiDebuggerTokenCheck: CommonTokenChecks
	$(ECHO) "-------------------------------------------------$(EOL)\
 DEBUGGER Enabled - Checking USB Debugger Configuration$(EOL)\
------------------------------------------------"
# Check the conflicting tokens for the USB2-USB3 debugger configurations
ifeq ("$(AMI_DEBUGGER_SUPPORT)", "1")
DEBUGGER_MODE = 1
ifneq ("$(AMIDEBUGGERPKG_SUPPORT)", "1")
$(error  DEBUGGER WARNING #0x300001 AMI_DEBUGGER_SUPPORT requires the AMIDEBUGGERPKG_SUPPORT SDL token to be 1)
endif
ifeq ("$(USB_REDIRECTION_SUPPORT)", "1")
$(error DEBUGGER WARNING #0x300002 AMI_DEBUGGER_SUPPORT requires the USB_REDIRECTION_SUPPORT SDL token to be 0)
endif
ifeq ("$(USB_3_DEBUG_SUPPORT)", "1")
ifeq ("$(USB_DEBUG_TRANSPORT)", "1")
$(error  DEBUGGER WARNING #0x300003 AMI_DEBUGGER_SUPPORT - USB_3_DEBUG_SUPPORT requires the USB_DEBUG_TRANSPORT SDL token to be 0)
endif
ifneq ("$(PeiDebugger_SUPPORT)", "1")
$(error  DEBUGGER WARNING #0x300004 AMI_DEBUGGER_SUPPORT - USB_3_DEBUG_SUPPORT requires the PeiDebugger_SUPPORT SDL token to be 1)
endif
DEBUGGER_MODE = 2
endif
ifneq ("$(USB_3_DEBUG_SUPPORT)", "1")
ifeq ("$(USB_DEBUG_TRANSPORT)", "0")
ifneq ("$(GENERIC_USB_CABLE_SUPPORT)", "1")
ifneq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
$(error  DEBUGGER WARNING #0x300005 AMI_DEBUGGER_SUPPORT - USB 2 DEBUGGER requires the USB_DEBUG_TRANSPORT SDL token to be 1)
endif
endif
endif
endif
ifeq ("$(SERIAL_DEBUGGER_SUPPORT)", "1")
$(warning  DEBUGGER WARNING #0x300006 AMI_DEBUGGER_SUPPORT and SERIAL_DEBUGGER_SUPPORT cannot be turned ON together)
endif
endif
# Check the conflicting token configuration for GENERIC_USB_CABLE_SUPPORT
ifeq ("$(GENERIC_USB_CABLE_SUPPORT)", "1")
ifeq ("$(PeiDebugger_SUPPORT)", "1")
$(error  DEBUGGER WARNING #0x300007 GENERIC_USB_CABLE_SUPPORT requires the PeiDebugger_SUPPORT SDL token to be 0)
endif
# Make sure that Generic USB Debugger and USB_DEBUG_TRANSPORT SDL tokens are not enabled togather
ifeq ("$(USB_DEBUG_TRANSPORT)", "1")
$(error  DEBUGGER WARNING #0x300008 GENERIC_USB_CABLE_SUPPORT requires the USB_DEBUG_TRANSPORT SDL token to be 0)
endif
# Make sure that Generic USB Debugger and USB_3_DEBUG_SUPPORT SDL tokens are not enabled togather
ifeq ("$(USB_3_DEBUG_SUPPORT)", "1")
$(error  DEBUGGER WARNING #0x300009 GENERIC_USB_CABLE_SUPPORT requires the USB_3_DEBUG_SUPPORT SDL token to be 0)
endif
endif
#$(ECHO) " ----------------------------------------------"

EndMessage: End
ifeq ("$(AMI_DEBUGGER_SUPPORT)", "1")
	$(ECHO) "Debugger has been enabled during the build.." 
	$(ECHO) $(GLOBALWARNING)
	$(ECHO) " |  AMIDEBUGGERPKG_SUPPORT   - $(AMIDEBUGGERPKG_SUPPORT)                                              |"
	$(ECHO) " |  AMI_DEBUGGER_SUPPORT  - $(AMI_DEBUGGER_SUPPORT)                                                 |"
	$(ECHO) " |  USB_3_DEBUG_SUPPORT   - $(USB_3_DEBUG_SUPPORT)                                                 |"
	$(ECHO) " |  USB_DEBUG_TRANSPORT   - $(USB_DEBUG_TRANSPORT)                                                 |"
	$(ECHO) " |  PeiDebugger_SUPPORT   - $(PeiDebugger_SUPPORT)                                                 |"
	$(ECHO) " |  DEBUG_MODE            - $(DEBUG_MODE)                                                 |"
ifeq ("$(DEBUG_MODE)", "1")
	$(ECHO) " |  OPTIMIZATION          - $(OPTIMIZATION)                                                 |"
	$(ECHO) " |  DEBUG_CODE            - $(DEBUG_CODE)                                                 |"
endif
	$(ECHO) " +----------------------------------------------------------------------------+"
	$(ECHO) " |  DEBUGGER MODE                                                             |"
	$(ECHO) " +----------------------------------------------------------------------------+"
ifeq ("$(DEBUGGER_MODE)", "1")
ifeq ("$(SHELL_DEBUGGER_SUPPORT)", "1")
	$(ECHO) " |   DEBUGGER Configuration -  SHELL DEBUGGER                                 |"
	$(ECHO) " +----------------------------------------------------------------------------+"	
else
	$(ECHO) " |   DEBUGGER Configuration -  USB 2 DEBUGGER MODE                            |"
	$(ECHO) " |   DebugPortingLib mapping used in project--                                |
	$(ECHO) -n " |"
	@type $(BUILD_DIR)\Platform.dsc | Find "DebugPortingLib"
	$(ECHO) " +----------------------------------------------------------------------------+"
endif

endif
ifeq ("$(DEBUGGER_MODE)", "2")
	$(ECHO) " |   DEBUGGER configuration -  USB 3 DEBUGGER MODE                            |"
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
