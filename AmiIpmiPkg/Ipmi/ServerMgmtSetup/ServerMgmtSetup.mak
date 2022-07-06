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

#
# @file  ServerMgmtSetup.mak
#
#  Make file for ServerMgmtSetup Modular part
#

Prepare :  $(BUILD_DIR)/ServerAutoId.h

$(BUILD_DIR)/ServerAutoId.h : $(ServerMgmtSetup_DIR)/ServerMgmtSetup.vfr $(SERVER_MGMT_SETUP_DEFINITIONS) $(BUILD_DIR)/Token.mak
	$(VFRID) /s$(SERVER_MGMT_SETUP_LOWEST_AUTOID_NUMBER) /o$(BUILD_DIR)/ServerAutoId.h $(ServerMgmtSetup_DIR)/ServerMgmtSetup.vfr $(SERVER_MGMT_SETUP_DEFINITIONS)
	@$(ECHO) \
"#ifndef AUTO_ID$(EOL)\
#define AUTO_ID(x) x$(EOL)\
#endif$(EOL)" >> $(BUILD_DIR)/ServerAutoId.h

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************