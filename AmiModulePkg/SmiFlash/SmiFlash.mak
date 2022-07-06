#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

ERRMSG1 = This Elink SMIFlashSaveVarsList is no longer supported
ERRMSG2 = This Elink SMIFlashSaveGuidsList is no longer supported
ERRMSG3 =  Please also check NVRAM.chm and use NvramPreservedVariables or NvramPersistentVariables instead of the retired Elinks.
Prepare : $(BUILD_DIR)/SMIFlashELinks.h
ifeq ($(call __ge, $(NVRAM_VERSION), 7),yes)
ifneq ($(SMIFlashSaveVarsList),)
    $(error  $(ERRMSG1), please remove the below child Elinks $(SMIFlashSaveVarsList).\$(ERRMSG3))
endif
ifneq ($(SMIFlashSaveGuidsList),)
    $(error  $(ERRMSG2), please remove the below child Elinks $(SMIFlashSaveGuidsList).\$(ERRMSG3))
endif
endif
$(BUILD_DIR)/SMIFlashELinks.h :  $(BUILD_DIR)/Token.mak
	$(ECHO) \
"#define SMIFLASH_IN_SMM_LIST $(SMIFlashInSmmList)$(EOL)\
#define SMIFLASH_NOT_IN_SMM_LIST $(SMIFlashNotInSmmList)$(EOL)\
#define SMIFLASH_END_UPDATE_LIST $(SMIFlashEndUpdateList)$(EOL)\
#define SMIFLASH_PRE_UPDATE_LIST $(SMIFlashPreUpdateList)$(EOL)\
#define SMIFLASH_PRE_HANDLER_LIST $(SMIFlashPreHandlerList)$(EOL)\
#define SMIFLASH_END_HANDLER_LIST $(SMIFlashEndHandlerList)$(EOL)\
#define SMIFLASH_SAVE_VARS_LIST $(SMIFlashSaveVarsList)$(EOL)\
#define SMIFLASH_SAVE_GUIDS_LIST $(SMIFlashSaveGuidsList)$(EOL)"\
> $(BUILD_DIR)/SMIFlashELinks.h