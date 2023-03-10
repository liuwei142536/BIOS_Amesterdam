#***********************************************************************
#*                                                                     *
#*                      Copyright (c) 1985-2021, AMI.                  *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

Prepare : $(BUILD_DIR)/EfiOsNamesFilePathMaps.h

EfiOsBootOptionNamesFilePathItem := $(subst \,\\,$(EfiOsBootOptionNamesFilePathItem)) #Replace \ to \\
EfiOsBootOptionNamesFilePathItem := $(subst <,L\",$(EfiOsBootOptionNamesFilePathItem)) #Replace < to L"
EfiOsBootOptionNamesFilePathItem := $(subst >,\",$(EfiOsBootOptionNamesFilePathItem)) #Replace > to "

EfiOsBootOptionNamesFilePathPriorityItem := $(subst \,\\,$(EfiOsBootOptionNamesFilePathPriorityItem)) #Replace \ to \\
EfiOsBootOptionNamesFilePathPriorityItem := $(subst <,L\",$(EfiOsBootOptionNamesFilePathPriorityItem)) #Replace < to L"
EfiOsBootOptionNamesFilePathPriorityItem := $(subst >,\",$(EfiOsBootOptionNamesFilePathPriorityItem)) #Replace > to "

ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
	EFI_OS_ECHO := $(ECHO) -E
else # Windows
	EFI_OS_ECHO := $(ECHO)
endif

$(BUILD_DIR)/EfiOsNamesFilePathMaps.h : $(EfiOsBootOptionNames_DIR)/EfiOsBootOptionNames.sdl $(EfiOsBootOptionNamesMakDependencies)
	$(EFI_OS_ECHO) \
"#define EfiOsFilePathMaps $(EfiOsBootOptionNamesFilePathItem)"\
>$(BUILD_DIR)/EfiOsNamesFilePathMaps.h
	$(EFI_OS_ECHO) \
"#define EfiOsFilePathPriorityMaps $(EfiOsBootOptionNamesFilePathPriorityItem)"\
>>$(BUILD_DIR)/EfiOsNamesFilePathMaps.h
	$(ECHO) \
"#define CREATE_EFI_OS_BOOT_OPTION_FUNCTIONS $(CREATE_EFI_OS_BOOT_OPTION_FUNCTIONS)$(EOL)\
#define ADJUST_EFI_OS_BOOT_ORDER_FUNCTIONS $(ADJUST_EFI_OS_BOOT_ORDER_FUNCTIONS)$(EOL)\
#define CHECK_UEFI_OS_BOOT_OPTION_FUNCTIONS $(CHECK_UEFI_OS_BOOT_OPTION_FUNCTIONS)$(EOL)"\
>>$(BUILD_DIR)/EfiOsNamesFilePathMaps.h
