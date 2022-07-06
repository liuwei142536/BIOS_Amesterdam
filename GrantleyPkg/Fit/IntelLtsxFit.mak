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
# $Header: /Alaska/Tools/template.mak 6     1/13/10 2:13p Felixp $
#
# $Revision: 6 $
#
# $Date: 1/13/10 2:13p $
#**********************************************************************
#<AMI_FHDR_START>
#
# Name:	<ComponentName>.mak
#
# Description:	
#
#<AMI_FHDR_END>
#**********************************************************************

End : RunFitUtility

OUTPUT_DIR = $(BUILD_DIR)\$(PLATFORM_NAME)\$(TARGET)_$(TOOL_CHAIN_TAG)
export OUTPUT_DIR

.PHONY : RunFitUtility
RunFitUtility:
	@$(ECHO) Run FIT Utility
	@copy /Y $(OUTPUT_DIR)\FV\AMIROM.fd $(BIOS_TAG).ROM
	@copy $(FIT_UTILITY) $(BUILD_DIR)\FitUtility.exe

ifeq ($(INTEL_TXT_SUPPORT),1)
	@$(ECHO) Updating the TXT patch on $(BIOS_TAG).ROM
	$(BUILD_DIR)\FitUtility.exe -Rom=$(BIOS_TAG).ROM -Fit=$(FIT_FILE) -Acm=$(BIOSACM_FILE) -MicrocodeFV=$(FIT_MICROCODE_BASE) -Alignment=16 -FitBase=$(FV_FIT_LOCATION) -AcmBase=$(FV_ACM_BASE)
else
	$(BUILD_DIR)\FitUtility.exe -Rom=$(BIOS_TAG).ROM -Fit=$(FIT_FILE) -MicrocodeFV=$(FIT_MICROCODE_BASE) -Alignment=16 -FitBase=$(FV_FIT_LOCATION)
endif

	@copy /Y $(BIOS_TAG).ROM $(OUTPUT_DIR)\FV\AMIROM.fd
ifeq ($(SecureMod_SUPPORT),1)	
	if exist $(BIOS_TAG).ROM $(RM) $(BIOS_TAG).ROM
endif	

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