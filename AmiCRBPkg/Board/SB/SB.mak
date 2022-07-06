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
# $Header: $
#
# $Revision: $
#
# $Date: $
#**********************************************************************
# Revision History
# ----------------
# $Log: $
# 
# 
#**********************************************************************
#<AMI_FHDR_START>
#
# Name:		SB.mak
#
# Description:	
#
#<AMI_FHDR_END>
#**********************************************************************
Prepare : $(BUILD_DIR)/SbInc.h $(BUILD_DIR)\GPIO.h

AMI_CSP_LIB_INCLUDE_FILES := $(BUILD_DIR)/SbInc.h \
$(AMI_CSP_LIB_INCLUDE_FILES)

$(BUILD_DIR)/SbInc.h : $(SB_BOARD_DIR)/SB.h $(SB_CHIPSET_DIR)/SBCspLib.h $(SB_BOARD_DIR)/SB.mak
	$(ECHO) \
"// Don't delete this line$(EOL)\
#include <$(SB_BOARD_DIR)/SB.h>$(EOL)\
#include <$(SB_CHIPSET_DIR)/SBCspLib.h>$(EOL)\
// Don't delete this line$(EOL)"\
> $@

$(BUILD_DIR)\GPIO.h :
	$(ECHO) \
"@echo off$(EOL)\
for /L %%%%i in (0, 1, 127) do (@echo #ifdef GPIO%%%%i_CONFIG >> $(BUILD_DIR)\GPIO.h$(EOL)\
@echo { GPIO%%%%i_CONFIG }, >> $(BUILD_DIR)\GPIO.h$(EOL)\
@echo #endif >> $(BUILD_DIR)\GPIO.h)$(EOL)"\
> $(BUILD_DIR)/GPIO.bat
	@call $(BUILD_DIR)/GPIO.bat


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
