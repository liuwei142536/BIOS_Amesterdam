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

Prepare : $(BUILD_DIR)/OA3TableAddressElink.h $(BUILD_DIR)/OA3SmmTableAddressElink.h

$(BUILD_DIR)/OA3TableAddressElink.h : $(BUILD_DIR)$(PATH_SLASH)Token.mak
	$(ECHO) \
"#define OA3_TABLE_ADDRESS_ELINK $(OA3TableAddressElink)$(EOL)"\
> $@

$(BUILD_DIR)/OA3SmmTableAddressElink.h : $(BUILD_DIR)$(PATH_SLASH)Token.mak
	$(ECHO) \
"#define OA3_SMM_TABLE_ADDRESS_ELINK $(OA3SmmTableAddressElink)$(EOL)"\
> $@

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
