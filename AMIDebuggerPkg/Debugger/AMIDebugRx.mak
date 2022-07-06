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
# $Header: /AptioV/SRC/AMIDebugRx/AMIDebugRx.mak 5     1/05/16 7:19p Sudhirv $
#
# $Revision: 5 $
#
# $Date: 1/05/16 7:19p $
#**********************************************************************
# Revision History
# ----------------
# $Log: /AptioV/SRC/AMIDebugRx/AMIDebugRx.mak $
# 
# 5     1/05/16 7:19p Sudhirv
# [Category]   	Improvement
# [Severity]		Minor
# [Description]	Code Clean Up
# [Files]			AMIDebugRx.mak
# 
# 4     4/30/15 6:19a Jekekumarg
# [TAG]			EIP212213
# [Category]   	Improvement
# [Description]	[USB Debugger] Have a token to make debugger module
# connection works only when it's at DXE phase
# [Files]			PeiAMIDebugAgent\PeiDbgMain.c
# DxeAMIDebugAgent\DxeDbgMain.c
# SmmAMIDebugAgentLib.inf
# SmmAMIDebugAgent\SmmDbgMain.c
# CommonDebugAgent.c
# AMIDebugRx.mak
# UsbDebugPort.c
# PeiDbgStatusCode.c
# CommonDebugStatusCodeLib.inf
# 
# 3     12/21/14 10:53p Sudhirv
# Removed the SDL token dependency
# 
# 2     6/24/14 6:00p Sudhirv
# Updated for AMI Debug for UEFI 3.01.0010 Label
# 
# 1     9/21/13 4:21a Sudhirv
# EIP 107788 - Debugger 3.0: SD File to indicate AMIDebugRX is added to
# project
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
