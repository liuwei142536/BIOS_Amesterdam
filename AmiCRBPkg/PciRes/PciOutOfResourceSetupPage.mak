#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
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
# $Header: /Alaska/BIN/Modules/Template/PciPlatformResources/PciOutOfResourceSetupPage.mak 1     3/30/11 9:51a Yakovlevs $Revision: 1 $
#
# $Date: 3/30/11 9:51a $
#**********************************************************************
# Revision History
# ----------------
# $Log: /Alaska/BIN/Modules/Template/PciPlatformResources/PciOutOfResourceSetupPage.mak $
# 
# 1     3/30/11 9:51a Yakovlevs
# Initial Checkin of the component
# 
# 1     12/23/10 11:52a Bhimanadhunik
# Support added for to display PCI resource error in the setup.
# 
# 
#**********************************************************************
#<AMI_FHDR_START>
#
# Name:	PciOutOfResourceSetupPage.mak
#
# Description:	
#
#<AMI_FHDR_END>
#**********************************************************************
Prepare : $(BUILD_DIR)\PciOutOfResourceSetupPageDefinitions.h

$(BUILD_DIR)\PciOutOfResourceSetupPageDefinitions.h : $(PciOutOfResourceSetupPage_DIR)\PciOutOfResourceSetupPage.mak
	@$(ECHO)\
"$(if $(PciOutOfResourceSetupPage_DEFINITIONS) ,\
$(foreach S_DEF, $(PciOutOfResourceSetupPage_DEFINITIONS),#include<$(S_DEF)>$(EOL)))\
$(EOL)\
#define PciOutOfResourceSetupPage_FUNCTION_LIST $(PciOutOfResourceSetupPageFunctions)$(EOL)\
" > $(BUILD_DIR)\PciOutOfResourceSetupPageDefinitions.h

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************