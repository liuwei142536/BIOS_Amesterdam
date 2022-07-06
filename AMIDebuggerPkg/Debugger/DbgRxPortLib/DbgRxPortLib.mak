#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2011, American Megatrends, Inc.         **
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
# $Header: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/DbgRxPortLib.mak 4     1/14/16 2:57p Sudhirv $
#
# $Revision: 4 $
#
# $Date: 1/14/16 2:57p $
#**********************************************************************
# Revision History
# ----------------
# $Log: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/DbgRxPortLib.mak $
# 
# 4     1/14/16 2:57p Sudhirv
# [TAG]			EIP252291
# [Category]   	Improvement
# [Severity]		Important
# [Description]	Fix casing and backslash issues in mak files
# [Files]			DbgRxPortLib.mak
# 
# 3     9/21/13 4:23a Sudhirv
# Updated for AMI Debug for UEFI 3.01.0006 Label.
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
# Name:			DbgRxPortLib.mak
#
# Description:	
#				
#
#-----------------------------------------------------------------------
#<AMI_FHDR_END>
#**********************************************************************
# Make sure that Debugger and USBRedirection modules are not enabled togather
ifeq ("$(AMI_DEBUGGER_SUPPORT)", "1")
  ifeq ("$(USB_REDIRECTION_SUPPORT)", "1")
    $(error  AMI_DEBUGGER_SUPPORT requires the USB_REDIRECTION_SUPPORT SDL token to be 0)
  endif
endif

Prepare : $(BUILD_DIR)/PEIDbgRxPortELink.h

$(BUILD_DIR)/PEIDbgRxPortELink.h :  $(BUILD_DIR)/Token.mak
	$(ECHO) \
"#define PEI_DBG_INIT_LIST  $(PeiDebugInitialize)$(EOL)"\
> $(BUILD_DIR)/PEIDbgRxPortELink.h

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2011, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************
