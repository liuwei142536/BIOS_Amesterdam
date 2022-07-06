#*************************************************************************
#*************************************************************************
#**                                                                     **
#**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
#**                                                                     **
#**                       All Rights Reserved.                          **
#**                                                                     **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
#**                                                                     **
#**                       Phone: (770)-246-8600                         **
#**                                                                     **
#*************************************************************************
#*************************************************************************

#*************************************************************************
#<AMI_FHDR_START>
#---------------------------------------------------------------------------
# Name:     ProcessorStartup.mak
#
# Description:  
#   Make file for SEC. "Prepare" target will make INF files generated
#   to support e-links features. 
#
#---------------------------------------------------------------------------
#<AMI_FHDR_END>

NumBistFunc := $(words $(GetBistFunc))

Prepare : DETECT_ERRORS $(BUILD_DIR)/ProcessorStartupAutoGen.h $(BUILD_DIR)/ProcessorStartup.inf

DETECT_ERRORS:
ifneq ($(NumBistFunc), 1)
	$(error There should only be one BIST function. ELINK PS_GetBist is incorrect. See sample in ProcessorStartup.sdl.)
endif

$(BUILD_DIR)/ProcessorStartupAutoGen.h : $(PROCESSOR_STARTUP_PATH)/ProcessorStartup.mak $(BUILD_DIR)/token.mak
	$(ECHO)  \
"#include <PiPei.h>$(EOL)\
$(EOL)\
typedef void (BIST_LIST)(void *);$(EOL)\
extern BIST_LIST $(GetBistFunc) EndOfOneParamList;$(EOL)\
BIST_LIST* BistList[] = {$(GetBistFunc) NULL};$(EOL)\
$(EOL)\
typedef void (SEC_ELINK_LIST)();$(EOL)\
extern SEC_ELINK_LIST $(SecElinkFuncs) EndOfNoParamList;$(EOL)\
SEC_ELINK_LIST* SecElinkList[] = {$(SecElinkFuncs) NULL};$(EOL)\
$(EOL)\
typedef EFI_PEI_PPI_DESCRIPTOR* (RET_PPI_DESC_FUNC)();$(EOL)\
extern RET_PPI_DESC_FUNC $(SecAdditionalPpis) EndOfAddPpiList;$(EOL)\
RET_PPI_DESC_FUNC *RetPpiList[] = {$(SecAdditionalPpis) NULL}; $(EOL)"\
>$(BUILD_DIR)/ProcessorStartupAutoGen.h

$(BUILD_DIR)/ProcessorStartup.inf : $(PROCESSOR_STARTUP_PATH)/ProcessorStartup.mak $(BUILD_DIR)/token.mak
	$(ECHO)  \
"[Defines]$(EOL)\
  INF_VERSION                    = 0x00010005$(EOL)\
  BASE_NAME                      = ProcessorStartup$(EOL)\
  FILE_GUID                      = 0833A84D-8A39-431f-BD1C-30965AAE71DD$(EOL)\
  MODULE_TYPE                    = SEC$(EOL)\
  VERSION_STRING                 = 1.0$(EOL)\
  LIBRARY_CLASS                  = PlatformSecLib$(EOL)\
$(EOL)\
[Sources]$(EOL)\
  SecCore.asm$(EOL)\
  $(patsubst %,../%,$(subst $(SPACE),$(EOL)$(SPACE),$(SEC_STARTUP_SRC)))$(EOL)\
  $(EOL)\
[Packages]$(EOL)\
  MdePkg/MdePkg.dec$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(SEC_STARTUP_PKG))$(EOL)\
  $(EOL)\
[LibraryClasses]$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(SEC_STARTUP_LIB_CLASSES))$(EOL)\
  $(EOL)\
[Pcd]$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(SEC_STARTUP_PCDS))$(EOL)\
  $(EOL)\
  "\
> $(BUILD_DIR)/ProcessorStartup.inf

