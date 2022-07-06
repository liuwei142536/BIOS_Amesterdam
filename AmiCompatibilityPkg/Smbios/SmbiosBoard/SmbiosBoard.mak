#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

#/** @file SmbiosBoard.mak
# * Make file to build SmbiosOemUpdateList.h
# */

Prepare : $(BUILD_DIR)/SmbiosOemUpdateList.h
ifeq ($(SecSMIFlash_SUPPORT), 1)
ifeq ($(SMBIOS_DMIEDIT_DATA_LOC), 0)
    $(error  SMBIOS_DMIEDIT_DATA_LOC cannot be set to 0 when SecSMIFlash_SUPPORT = 1. Please select a different setting for SMBIOS_DMIEDIT_DATA_LOC (See explaination in Smbios.chm))
endif
endif

#----------------------------------------------------------------------------
#           Create SmbiosOemUpdate Elinks header file
#----------------------------------------------------------------------------

$(BUILD_DIR)/SmbiosOemUpdateList.h : $(BUILD_DIR)/Token.h $(SMBIOS_BOARD_DIR)/SmbiosBoard.mak
	$(ECHO) \
"#define SMBIOS_OEM_UPDATE_LIST $(SmbiosOemUpdateList)$(EOL)\
" > $(BUILD_DIR)/SmbiosOemUpdateList.h

