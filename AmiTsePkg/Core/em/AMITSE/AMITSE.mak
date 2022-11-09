#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#**********************************************************************
# Name:	AMITSE.mak

Prepare : $(BUILD_DIR)/SignOn.h $(BUILD_DIR)/AMITSEOem.h $(BUILD_DIR)/AMITSEStrTokens.h

# Use DXE_ARCHITECTURE if defined, otherwise fallback!!!
ifdef DXE_ARCHITECTURE
    AMITSE_TARGET_ARCH = $(DXE_ARCHITECTURE)
else 
  ifdef INTEL_ARCHITECTURE_SUPPORT
    ifeq ($(X64_SUPPORT), 1)
        AMITSE_TARGET_ARCH = X64
    else
        AMITSE_TARGET_ARCH = IA32
    endif
  else  
    AMITSE_TARGET_ARCH = AARCH64
  endif
endif


TSE_GC_MODE1 = $(subst {,,$(MAX_POST_GC_MODE))
TSE_GC_MODE2 = $(subst },,$(TSE_GC_MODE1))

ifeq ($(TSE_BUILD_AS_APPLICATION), 0)
TSE_MODULE_TYPE = DXE_DRIVER
TSE_EntryPoint = UefiDriverEntryPoint
else
TSE_MODULE_TYPE = UEFI_APPLICATION
TSE_EntryPoint = UefiApplicationEntryPoint
endif

ifeq ($(AMITSE_TARGET_ARCH), IA32)
ifeq ($(TSE_SOURCES_SUPPORT), 0)
$(error AMITSE no longer supports IA32 binary build. Please add TSE Sources module to the project.)
endif
endif

ifeq ($(TSE_SOURCES_SUPPORT), 0)
ifeq ($(TSE_STYLE_EZPORT_BIN_SUPPORT), 0)
$(error AMITSE no longer supports Legacy\EzportPlus binary build. Please add TSE Sources module to the project.)
endif
endif

.PHONY : CheckSW
Prepare : CheckSW

CheckSW :

ifneq ("$(SW_CUSTOMIZED_SETUP_DATA)", "")
ifneq ("$(SW_SETUP_DATA)", "")
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
	$(ECHO) \
"@echo off$(EOL)\
fc $(SW_SETUP_DATA) $(SW_CUSTOMIZED_SETUP_DATA) > nul$(EOL)\
if %%errorlevel%% == 0 (del $(SW_CUSTOMIZED_SETUP_DATA) $(SW_ORIGINAL_SETUP_DATA))  else (echo SetupPackage.xml and CustSetupPackage.xml not same)$(EOL)\
">$(BUILD_DIR)/RemoveCustomXml.bat
	$(BUILD_DIR)\RemoveCustomXml.bat
	$(RM) $(BUILD_DIR)\RemoveCustomXml.bat
else
	$(ECHO)\
"#!/bin/sh$(EOL)\
cmp -s $(SW_SETUP_DATA) $(SW_CUSTOMIZED_SETUP_DATA) $(EOL)\
if [ \$$? -eq 0 ]; then $(EOL)\
    rm  -f $(SW_CUSTOMIZED_SETUP_DATA)  $(SW_ORIGINAL_SETUP_DATA) $(EOL)\
fi $(EOL)\
">$(BUILD_DIR)/RemoveCustomXml.sh
	chmod 777 $(BUILD_DIR)/RemoveCustomXml.sh
	$(BUILD_DIR)/RemoveCustomXml.sh
	rm $(BUILD_DIR)/RemoveCustomXml.sh
endif
endif
endif

################################################################################
###	Creating SignOn.h, AMISEStrTokens.h, AMITSEOem.h files
################################################################################
$(BUILD_DIR)/SignOn.h :  $(BUILD_DIR)/Token.mak $(TSEBIN_DIR)/AMITSE.mak
#if exist $(BUILD_DIR)/SignOn.h
	$(ECHO) \
"$(EOL)\
#define BIOS_SIGNON_MESSAGE2_STRING	$(BIOS_SIGNON_MESSAGE2)$(EOL)\
" > $(BUILD_DIR)/SignOn.h
#endif

.PHONY: $(BUILD_DIR)/AMITSEStrTokens.h
$(BUILD_DIR)/AMITSEStrTokens.h : 
	$(ECHO) \
"#include <$(OUTPUT_DIRECTORY)$(PATH_SLASH)$(TARGET)_$(TOOL_CHAIN_TAG)$(PATH_SLASH)$(AMITSE_TARGET_ARCH)/Build/AMITSE/DEBUG/AMITSEStrDefs.h>$(EOL)\
">$(BUILD_DIR)/AMITSEStrTokens.h


$(BUILD_DIR)/AMITSEOem.h : $(BUILD_DIR)/Token.mak $(TSEBIN_DIR)/AMITSE.mak
	$(ECHO) \
"$(if $(AMITSE_OEM_HEADER_LIST), $(foreach S_DEF, $(AMITSE_OEM_HEADER_LIST),#include<$(S_DEF)>$(EOL)))"\
>$(BUILD_DIR)/AMITSEOem.h
######################################################################################

Prepare: $(BUILD_DIR)/AMITSE.inf
.PHONY : HpkTool
BeforeGenFds : HpkTool

ABS_OUTPUT_DIR:=$(WORKSPACE)/$(OUTPUT_DIRECTORY)

ifeq ($(INI_FILE_PATH),)
  ifeq ($(wildcard $(TSEBIN_DIR)/uefisetup.ini), $(TSEBIN_DIR)/uefisetup.ini)
    HPKTOOL_INI_FILE_PATH:=$(WORKSPACE)/$(TSEBIN_DIR)/Uefisetup.ini
  else
    HPKTOOL_INI_FILE_PATH:=$(TOOLS_DIR)/DefaultHpkToolSetup.ini
  endif
else
HPKTOOL_INI_FILE_PATH:=$(WORKSPACE)/$(INI_FILE_PATH)
endif

ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
HPK_TOOL:=$(WORKSPACE)/$(TSEBIN_DIR)/UefiHpkTool.exe
else
HPK_TOOL:=$(WORKSPACE)/$(TSEBIN_DIR)/HPKTool_Linux
endif

HPKTOOL_COMMAND_LINE+=$(HPK_TOOL) $(DOUBLEDOLLAR)(IFR_DIR_LIST) -f$(HPKTOOL_INI_FILE_PATH)
HPKTOOL_COMMAND_LINE2:=$(HPKTOOL_COMMAND_LINE) -do$(ABS_OUTPUT_DIR)/Nvram.bin
HPKTOOL_COMMAND_LINE+=-do$(ABS_OUTPUT_DIR)/Defaults.bin
ifeq ($(BIN_SUPPORT_IN_HPKTOOL),0)
HPKTOOL_COMMAND_LINE+=-o$(ABS_BUILD_DIR)/tmp.asm
HPKTOOL_COMMAND_LINE2+=-o$(ABS_BUILD_DIR)/setupdata_asm.asm
else
HPKTOOL_COMMAND_LINE+=-o$(ABS_BUILD_DIR)/tmp.asm
HPKTOOL_COMMAND_LINE2+=-bin$(ABS_BUILD_DIR)/setupdata.bin
endif
ifeq ($(MAPPING_CONFIG_SUPPORT), 1)
HPKTOOL_COMMAND_LINE+=-js$(WORKSPACE)/$(JSON_FILE_PATH)
HPKTOOL_COMMAND_LINE+=-mcs
HPKTOOL_COMMAND_LINE+=-mcd$(MAPPING_CONFIG_DXE_SUPPORT)
endif
HPKTOOL_COMMAND_LINE+=-tml$(TSE_MAPPING_LANGUAGE)
HPKTOOL_COMMAND_LINE2+=-tml$(TSE_MAPPING_LANGUAGE)
ifeq ($(MANUFACTURING_MODE_SUPPORT),1)
HPKTOOL_COMMAND_LINE+=-dms
HPKTOOL_COMMAND_LINE2+=-dms
endif
ifeq ($(REPORT_MAPPING_ID_CONFLITCS_IN_FILE),1)
HPKTOOL_COMMAND_LINE2+=-mpl$(REPORT_MAPPING_ID_CONFLITCS_IN_FILE)
HPKTOOL_COMMAND_LINE2+=-mpf$(ABS_BUILD_DIR)/$(REPORT_MAPPING_ID_FILE_NAME)
HPKTOOL_COMMAND_LINE2+=-mps$(TSE_MAPPING_LANGUAGE_SKIP_NONSTORAGE_CONTROLS_CHECK)
endif
##############################################################################################################
# UEFI specification flag (-vs) for HPKTool should not be less than 0x2001E
# to support EFI_IFR_VARSTORE_EFI_OP varstore and should not be enclosed within bracket e.g (0x2001F)
##############################################################################################################
HPKTOOL_COMMAND_LINE+=-r -dn$(NVRAM_RECORD_CHECKSUM_SUPPORT) -de$(FLASH_ERASE_POLARITY) -vs0x2001F -dl$(DEFAULT_LANGUAGE) -dp$(SETUP_ORPHAN_PAGES_AS_ROOT_PAGE)\
-dg4599D26F-1A11-49b8-B91F-858745CFF824\
-h$(AMITSE_HPK_FILE_ORDER)
HPKTOOL_COMMAND_LINE2+=-dl$(DEFAULT_LANGUAGE) -dp$(SETUP_ORPHAN_PAGES_AS_ROOT_PAGE)\
-ds$(RAW_NVRAM_SIZE) -dn$(NVRAM_RECORD_CHECKSUM_SUPPORT) -de$(FLASH_ERASE_POLARITY) -vs0x2001F -h$(AMITSE_HPK_FILE_ORDER)
############################################################################
## Adding RT attribute to DEFAULT_VAR_ATTRIBUTE in command-line
## based on token RT_ACCESS_SUPPORT_IN_HPKTOOL
## when token is enabled RT will add to form (RT+BS+NV) 0x07 for varstore variable
#############################################################################
ifeq ($(RT_ACCESS_SUPPORT_IN_HPKTOOL),1)
	HPKTOOL_COMMAND_LINE+=-RT
	HPKTOOL_COMMAND_LINE2+=-RT
endif
############################################################################
## Adding RT attribute to DEFAULT_EFI_VAR_ATTRIBUTE in command-line
## based on token RT_ACCESS_FOR_EFIVARSTORE
## when token is enabled RTE will add to allow to set RT attribute for efivarstore variable 
#############################################################################
ifeq ($(RT_ACCESS_FOR_EFIVARSTORE),1)
	HPKTOOL_COMMAND_LINE+=-RTE
	HPKTOOL_COMMAND_LINE2+=-RTE
endif
#############################################################################

AMI_TSE_PKG_USE_NEW_HPK_SCRIPT:=0
ifdef __ge
ifdef AMI_PKG_VERSION
ifeq ($(call __ge, $(AMI_PKG_VERSION), 11),yes)
AMI_TSE_PKG_USE_NEW_HPK_SCRIPT:=1
endif
endif
endif

ifeq ($(AMI_TSE_PKG_USE_NEW_HPK_SCRIPT),1)
AMI_TSE_PKG_BEFORE_GEN_FDS_FILE:=$(BUILD_DIR)/BeforeGenFds.mak
AMI_TSE_PKG_FIRST_REDIR:=>>

HpkTool : InitBeforeGenFds $(BUILD_DIR)/IfrListScript.txt

#create gawk file that is used to process the IFR_DIR_LIST
$(BUILD_DIR)/IfrListScript.txt : $(TSEBIN_DIR)/AMITSE.mak
	$(ECHO) \
"BEGIN { i = 0 }\
{ IfrList[$(DOUBLEDOLLAR)0] = 1 } \
$(EOL)END { for (item in IfrList) IfrList2[i++] = item; n = asort(IfrList2); for (i = 1; i <= n; i++) print \"IFR_DIR_LIST+= -i\"IfrList2[i] }" > $(BUILD_DIR)/IfrListScript.txt
else
APTIO_MAKE:=$(MAKE)
TAB := \t
AMI_TSE_PKG_BEFORE_GEN_FDS_FILE:=$(BUILD_DIR)/AmiTseBeforeGenFds.mak
AMI_TSE_PKG_FIRST_REDIR:=>
endif

# HpkTool creates temporary files in the current directory.
# We are passing -C option to a child instance of make to change directory 
# prior to running the HpkTool.
HpkTool :
	$(ECHO) "$(EOL).PHONY : HpkTool$(EOL)all: HpkTool$(EOL)HpkTool :" $(AMI_TSE_PKG_FIRST_REDIR) $(AMI_TSE_PKG_BEFORE_GEN_FDS_FILE)
ifeq ($(AMI_TSE_PKG_USE_NEW_HPK_SCRIPT),1)	
	$(ECHO) "$(TAB)gawk -f $(WORKSPACE)/$(BUILD_DIR)/IfrListScript.txt $(ABS_OUTPUT_DIR)/$(TARGET)_$(TOOL_CHAIN_TAG)/IfrDirList.txt\
> $(ABS_OUTPUT_DIR)/$(TARGET)_$(TOOL_CHAIN_TAG)/IfrList.mak" >> $(AMI_TSE_PKG_BEFORE_GEN_FDS_FILE)
else
# We are dealing with the older version of AmiPkg that does not launch BeforeGenFds.mak
	$(ECHO) "$(APTIO_MAKE) -C $(WORKSPACE) -f $(WORKSPACE)/$(AMI_TSE_PKG_BEFORE_GEN_FDS_FILE)" >> $(BUILD_DIR)/BeforeGenFds.bat
	$(ECHO) "$(TAB)$(ECHO) \"IFR_DIR_LIST:=-i$(subst $(SPACE),$(SPACE)-i,$(IFR_DIR_LIST))\"\
> $(ABS_OUTPUT_DIR)/$(TARGET)_$(TOOL_CHAIN_TAG)/IfrList.mak" >> $(AMI_TSE_PKG_BEFORE_GEN_FDS_FILE)
endif
	$(ECHO) "$(TAB)$(APTIO_MAKE) -C $(ABS_OUTPUT_DIR)/$(TARGET)_$(TOOL_CHAIN_TAG) -f $(WORKSPACE)/$(BUILD_DIR)/AmiTseGenFds.mak" >> $(AMI_TSE_PKG_BEFORE_GEN_FDS_FILE)
	$(ECHO_NO_ESC) "export Path := $(subst \;,;,$(PATH))" > $(BUILD_DIR)/AmiTseGenFds.mak
	$(ECHO) \
"include $(ABS_OUTPUT_DIR)/$(TARGET)_$(TOOL_CHAIN_TAG)/IfrList.mak\
$(EOL).PHONY : all\
$(EOL)all : \
" >> $(BUILD_DIR)/AmiTseGenFds.mak
ifneq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
	$(ECHO) "$(TAB)chmod +x $(HPK_TOOL)" >> $(BUILD_DIR)/AmiTseGenFds.mak
endif
	$(ECHO) "$(TAB)$(HPKTOOL_COMMAND_LINE) \
$(EOL)$(TAB)$(HPKTOOL_COMMAND_LINE2) \
" >> $(BUILD_DIR)/AmiTseGenFds.mak
ifeq ($(BIN_SUPPORT_IN_HPKTOOL),0)
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
	$(ECHO) "$(TAB)$(subst $(PATH_SLASH),/,$(TSE_MASM_PATH))ml /c /nologo /Fo$(ABS_BUILD_DIR)/ $(ABS_BUILD_DIR)/setupdata_asm.asm \
$(EOL)$(TAB)\"$(subst $(PATH_SLASH),/,$(CCX86DIR))/link\" /NOENTRY /FIXED /DLL $(ABS_BUILD_DIR)/setupdata_asm.obj /OUT:$(ABS_BUILD_DIR)/setupdata.dll \
$(EOL)$(TAB)genfw --exe2bin $(ABS_BUILD_DIR)/setupdata.dll -o $(ABS_BUILD_DIR)/setupdata.bin" >> $(BUILD_DIR)/AmiTseGenFds.mak
else
	$(ECHO) "$(TAB)gcc -c -x assembler $(ABS_BUILD_DIR)/setupdata_asm.asm   -o$(ABS_BUILD_DIR)/setupdata_asm.o\
$(EOL)$(TAB)objcopy -O binary $(ABS_BUILD_DIR)/setupdata_asm.o $(ABS_BUILD_DIR)/setupdata.bin" >> $(BUILD_DIR)/AmiTseGenFds.mak
endif
endif



################################################################################
###	Creating AMITSE.inf file
################################################################################
$(BUILD_DIR)/AMITSE.inf : $(BUILD_DIR)/Token.mak $(TSEBIN_DIR)/AMITSE.mak
	@$(ECHO) \
"$(EOL)\
## @file$(EOL)\
#   The [Sources] section for this file is auto-generated from ELINKs:$(EOL)\
#   SETUP_DEFINITIONS, SETUP_FORMSETS, SetupStringFiles, and SetupCallbackFiles$(EOL)\
##$(EOL)\
$(EOL)\
[Defines]$(EOL)\
  INF_VERSION                    = 0x00010005$(EOL)\
  BASE_NAME                      = AMITSE$(EOL)\
  FILE_GUID                      = B1DA0ADF-4F77-4070-A88E-BFFE1C60529A$(EOL)\
  MODULE_TYPE                    = $(TSE_MODULE_TYPE)$(EOL)\
  VERSION_STRING                 = 1.0$(EOL)\
  ENTRY_POINT                    = MiniSetupApplication$(EOL)\
#$(EOL)\
#  This flag specifies whether HII resource section is generated into PE image.$(EOL)\
#$(EOL)\
  UEFI_HII_RESOURCE_SECTION      = TRUE$(EOL)\
  $(EOL)\
#$(EOL)\
# The following information is for reference only and not required by the build tools.$(EOL)\
#$(EOL)\
#  VALID_ARCHITECTURES           = IA32 X64 IPF EBC$(EOL)\
"> $(BUILD_DIR)/AMITSE.inf
##
## Sources Sections
##
	@$(ECHO) \
"$(EOL)\
[Sources]$(EOL)\
../$(TSEBIN_DIR)/bootflow.c$(EOL)\
../$(TSEBIN_DIR)/bootflow.h$(EOL)\
#../$(TSEBIN_DIR)/commonoem.c$(EOL)\
../$(TSEBIN_DIR)/commonoem.h$(EOL)\
../$(TSEBIN_DIR)/FakeTokens.c$(EOL)\
../$(TSEBIN_DIR)/Inc/boot.h$(EOL)\
../$(TSEBIN_DIR)/Inc/HiiLib.h$(EOL)\
../$(TSEBIN_DIR)/Inc/LogoLib.h$(EOL)\
../$(TSEBIN_DIR)/Inc/mem.h$(EOL)\
../$(TSEBIN_DIR)/Inc/PwdLib.h$(EOL)\
../$(TSEBIN_DIR)/Inc/variable.h$(EOL)\
../$(TSEBIN_DIR)/Inc/HookAnchor.h$(EOL)\
../$(TSEBIN_DIR)/HookList.c$(EOL)\
#../$(TSEBIN_DIR)/CommonHelper.c$(EOL)\
../$(TSEBIN_DIR)/Keymon.c$(EOL)\
../$(TSEBIN_DIR)/Keymon.h$(EOL)\
../$(TSEBIN_DIR)/Inc/TseElinks.h$(EOL)\
../$(TSEBIN_DIR)/Inc/TseCommon.h$(EOL)\
../$(TSEBIN_DIR)/Inc/setupdata.h$(EOL)\
../$(TSEBIN_DIR)/TseDrvHealth.h$(EOL)\
../$(TSEBIN_DIR)/setupdbg.h$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

#################################################################################################
##
##	Adding TSE_SOURCE_FILE, TSE_CONSUMER_LIST, TSE_STRING_FILES file entries in Sources Section
##
#################################################################################################
ifneq ($(TSE_SOURCE_FILES),"")
	@$(ECHO) \
"  $(patsubst %,../%,$(subst $(SPACE),$(EOL)$(SPACE),$(TSE_SOURCE_FILES)))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

ifneq ($(TSE_STRING_FILES),"")
	@$(ECHO) \
"  $(patsubst %,../%,$(subst $(SPACE),$(EOL)$(SPACE),$(TSE_STRING_FILES)))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

ifneq ($(TSE_STRING_CONSUMERS_LIST),"")
	@$(ECHO) \
"  $(patsubst %,../%,$(subst $(SPACE),$(EOL)$(SPACE),$(TSE_STRING_CONSUMERS_LIST)))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif
##
## Packages Section
##
	@$(ECHO) \
"$(EOL)\
[Packages]$(EOL)\
  AmiTsePkg/AmiTsePkg.dec$(EOL)\
  AmiTsePkg/AmiTsePrivatePkg.dec$(EOL)\
  AmiCompatibilityPkg/AmiCompatibilityPkg.dec$(EOL)\
  MdePkg/MdePkg.dec$(EOL)\
#  AmiModulePkg/AmiModulePkg.dec$(EOL)\
  MdeModulePkg/MdeModulePkg.dec$(EOL)\
  IntelFrameworkPkg/IntelFrameworkPkg.dec$(EOL)\
  IntelFrameworkModulePkg/IntelFrameworkModulePkg.dec$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_Packages))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## LibraryClasses Section
##
	@$(ECHO) \
"$(EOL)\
[LibraryClasses]$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_LibraryClasses))$(EOL)\
  AmiDxeLib$(EOL)\
  MemoryAllocationLib$(EOL)\
  BaseLib$(EOL)\
  BaseMemoryLib$(EOL)\
  PerformanceLib$(EOL)\
  DevicePathLib$(EOL)\
  DebugLib$(EOL)\
  UefiLib$(EOL)\
  PrintLib$(EOL)\
  $(TSE_EntryPoint)$(EOL)\
  ReportStatusCodeLib$(EOL)\
  PcdLib$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## Protocols Section
##
	@$(ECHO) \
"$(EOL)\
[Protocols]$(EOL)\
  gEfiConsoleControlProtocolGuid$(EOL)\
  gEfiFirmwareVolume2ProtocolGuid$(EOL)\
  gEfiUnicodeCollation2ProtocolGuid$(EOL)\
  gEfiSimpleFileSystemProtocolGuid$(EOL)\
  gEfiBlockIoProtocolGuid$(EOL)\
  gEfiLegacyBiosProtocolGuid$(EOL)\
  gEfiSimpleTextInProtocolGuid$(EOL)\
  gEfiLoadedImageProtocolGuid$(EOL)\
  gAmiPostManagerProtocolGuid$(EOL)\
  gTSEInvalidateBgrtStatusProtocolGuid$(EOL)\
  gEsaInterfacesForTseProtocolGuid$(EOL)\
  gEsaFileBrowserProtocolGuid$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_Protocols))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## GUIDs Section
##
	@$(ECHO) \
"$(EOL)\
[Guids]$(EOL)\
   gAmiTseSetupEnterGuid$(EOL)\
   gAmiTseAfterFirstBootOptionGuid$(EOL)\
   gAmiTseEventBeforeBootGuid$(EOL)\
   gAmiTseEventAfterBootGuid$(EOL)\
   gAmiTseBootOrderChangeGuid$(EOL)\
   gAmiTseNVRAMUpdateGuid$(EOL)\
   gAmiTsePasswordPromptEnterGuid$(EOL)\
   gAmiTsePasswordPromptExitGuid$(EOL)\
   gAmiTseUserPasswordValidGuid$(EOL)\
   gAmiTseAdminPasswordValidGuid$(EOL)\
   gAmiTseInvalidPasswordGuid$(EOL)\
   gAmiTseBeforeTimeOutGuid$(EOL)\
   gAmiTseAfterTimeOutGuid$(EOL)\
   gAmiTseOemTSEVarGuid$(EOL)\
   gAmiTseOemPortingVar1Guid$(EOL)\
   gAmiTseOemPortingVar2Guid$(EOL)\
   gAmiTseOemPortingVar3Guid$(EOL)\
   gAmiTseOemPortingVar4Guid$(EOL)\
   gAmiTseOemPortingVar5Guid$(EOL)\
   gAmiTseOemPortingGuid1$(EOL)\
   gAmiTseOemPortingGuid2$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_Guids))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## PCD Section
##
	@$(ECHO) \
"$(EOL)\
[Pcd]$(EOL)\
$(EOL)\
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_PCDs))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## DEPEX Section
##
	@$(ECHO) \
"$(EOL)\
[Depex]$(EOL)\
  gEfiVariableArchProtocolGuid AND gEfiVariableWriteArchProtocolGuid$(EOL)\
  $(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## BuildOptions Section
##
	@$(ECHO) \
"$(EOL)\
[BuildOptions]$(EOL)\
  MSFT:*_*_*_CC_FLAGS = /DTSE_FOR_APTIO_4_50$(EOL)\
  GCC:*_*_*_CC_FLAGS = -nodefaultlibs -DTSE_FOR_APTIO_4_50$(EOL)\
  *_*_*_BUILD_FLAGS=-s$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
