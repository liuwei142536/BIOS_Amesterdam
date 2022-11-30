
CONFIGURATION_DIR := AmiPkg\Configuration

include $(CONFIGURATION_DIR)\utilities.mak
include $(BUILD_DIR)\Token.mak

TODAY := $(shell $(DATE) +'%m/%d/%Y')
TODAY_TWO_DIGIT_LIST := $(subst /, ,$(TODAY))
FOUR_DIGIT_YEAR := $(word 3,$(TODAY_TWO_DIGIT_LIST))

AMITSE : AMITSE_INF AMITSE_OEM TIMESTAMP


AMITSE_OEM : $(BUILD_DIR)\Token.mak
	$(ECHO) \
"$(if $(AMITSE_OEM_HEADER_LIST), $(foreach S_DEF, $(AMITSE_OEM_HEADER_LIST),#include<../../../../$(S_DEF)>$(EOL)))"\
>$(BUILD_DIR)/AMITSEOem.h

TIMESTAMP : $(CONFIGURATION_DIR)\utilities.mak
	$(ECHO) \
"#define FOUR_DIGIT_YEAR \"$(FOUR_DIGIT_YEAR)\" $(EOL)\
">$(BUILD_DIR)/TimeStamp.h

AMITSE_INF : $(BUILD_DIR)\Token.mak
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
  MODULE_TYPE                    = DXE_DRIVER$(EOL)\
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
../$(TSEBIN_DIR)/Keymon.c$(EOL)\
../$(TSEBIN_DIR)/Keymon.h$(EOL)\
../$(TSEBIN_DIR)/Inc/TseElinks.h$(EOL)\
../$(TSEBIN_DIR)/Inc/TseCommon.h$(EOL)\
../$(TSEBIN_DIR)/Inc/setupdata.h$(EOL)\
../$(TSEBIN_DIR)/TseDrvHealth.h$(EOL)\
../$(TSEBIN_DIR)/setupdbg.h$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

##
## BootOnly
##
	@$(ECHO) \
"$(EOL)\
..\$(TSESRC_DIR)\AMILogo/AMILogo.c$(EOL)\
..\$(TSESRC_DIR)\AMILogo/AMILogo.h$(EOL)\
..\$(TSESRC_DIR)\PasswordEncode\PasswordEncode.c$(EOL)\
..\$(TSESRC_DIR)\PasswordEncode\PasswordEncode.h$(EOL)\
..\$(TSESRC_DIR)\PasswordEncode\TsePasswordEncode.c$(EOL)\
..\$(TSESRC_DIR)\\amiver.h$(EOL)\
..\$(TSESRC_DIR)\\bbs.c$(EOL)\
..\$(TSESRC_DIR)\\bbs.h$(EOL)\
..\$(TSESRC_DIR)\\boot.c$(EOL)\
..\$(TSESRC_DIR)\\box.c$(EOL)\
..\$(TSESRC_DIR)\\box.h$(EOL)\
..\$(TSESRC_DIR)\\buffer.c$(EOL)\
..\$(TSESRC_DIR)\\buffer.h$(EOL)\
..\$(TSESRC_DIR)\dobmpmgr.c$(EOL)\
..\$(TSESRC_DIR)\globals.c$(EOL)\
..\$(TSESRC_DIR)\hiistring.c$(EOL)\
..\$(TSESRC_DIR)\hiistring.h$(EOL)\
..\$(TSESRC_DIR)\logo.c$(EOL)\
..\$(TSESRC_DIR)\mem.c$(EOL)\
..\$(TSESRC_DIR)\minisetup.c$(EOL)\
..\$(TSESRC_DIR)\minisetup.h$(EOL)\
..\$(TSESRC_DIR)\minisetupext.c$(EOL)\
..\$(TSESRC_DIR)\\notify.c$(EOL)\
..\$(TSESRC_DIR)\postmgmt.c$(EOL)\
..\$(TSESRC_DIR)\print.c$(EOL)\
..\$(TSESRC_DIR)\protocol.c$(EOL)\
..\$(TSESRC_DIR)\protocol.h$(EOL)\
..\$(TSESRC_DIR)\screen.c$(EOL)\
..\$(TSESRC_DIR)\screen.h$(EOL)\
..\$(TSESRC_DIR)\string.c$(EOL)\
..\$(TSESRC_DIR)\string.h$(EOL)\
..\$(TSESRC_DIR)\\timer.c$(EOL)\
..\$(TSESRC_DIR)\Timer.h$(EOL)\
..\$(TSESRC_DIR)\\variable.c$(EOL)\
..\$(TSESRC_DIR)\Resources.c$(EOL)\
..\$(TSESRC_DIR)\HookAnchor.c$(EOL)\
..\$(TSESRC_DIR)\EDKhelper.c$(EOL)\
..\$(TSESRC_DIR)\EDKhelper.h$(EOL)\
..\$(TSESRC_DIR)\password.h$(EOL)\
..\$(TSESRC_DIR)\password.c$(EOL)\
..\$(TSESRC_DIR)\postmgmtext.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

##
## TseAdvanced 
##
	@$(ECHO) \
"$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\MyDefs.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\iJpeg.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\jpeg6.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\Jpeg6.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\jpeg.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\Logo.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\ipcx.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\pcx.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\pcxc.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\dogifmgr.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\dogif.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\png.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\png.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\AdvancedImages\pnguncmp.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\Bbs.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\TseAdvanced.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\special.c$(EOL)\
..\$(TSE_ADVANCED_DIR)\special.h$(EOL)\
..\$(TSE_ADVANCED_DIR)\\bootflowLib.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

##
## TseLite
##
	@$(ECHO) \
"$(EOL)\
..\$(TSELITESRC_DIR)\minisetupext.h$(EOL)\
..\$(TSELITESRC_DIR)\\application.h$(EOL)\
..\$(TSELITESRC_DIR)\Action.c$(EOL)\
..\$(TSELITESRC_DIR)\\action.h$(EOL)\
..\$(TSELITESRC_DIR)\\application.c$(EOL)\
..\$(TSELITESRC_DIR)\object.c$(EOL)\
..\$(TSELITESRC_DIR)\object.h$(EOL)\
..\$(TSELITESRC_DIR)\page.c$(EOL)\
..\$(TSELITESRC_DIR)\page.h$(EOL)\
..\$(TSELITESRC_DIR)\\control.c$(EOL)\
..\$(TSELITESRC_DIR)\\control.h$(EOL)\
..\$(TSELITESRC_DIR)\\callback.c$(EOL)\
..\$(TSELITESRC_DIR)\\callback.h$(EOL)\
..\$(TSELITESRC_DIR)\hotkey.h$(EOL)\
..\$(TSELITESRC_DIR)\\frame.c$(EOL)\
..\$(TSELITESRC_DIR)\\frame.h$(EOL)\
..\$(TSELITESRC_DIR)\hotkey.c$(EOL)\
..\$(TSELITESRC_DIR)\SubMenu.h$(EOL)\
..\$(TSELITESRC_DIR)\SubMenu.c$(EOL)\
..\$(TSELITESRC_DIR)\Text.c$(EOL)\
..\$(TSELITESRC_DIR)\Text.h$(EOL)\
..\$(TSELITESRC_DIR)\Menu.c$(EOL)\
..\$(TSELITESRC_DIR)\Menu.h$(EOL)\
..\$(TSELITESRC_DIR)\MessageBox.h$(EOL)\
..\$(TSELITESRC_DIR)\MessageBox.c$(EOL)\
..\$(TSELITESRC_DIR)\memo.c$(EOL)\
..\$(TSELITESRC_DIR)\Memo.h$(EOL)\
..\$(TSELITESRC_DIR)\popup.c$(EOL)\
..\$(TSELITESRC_DIR)\popup.h$(EOL)\
..\$(TSELITESRC_DIR)\Label.h$(EOL)\
..\$(TSELITESRC_DIR)\Label.c$(EOL)\
..\$(TSELITESRC_DIR)\\numeric.c$(EOL)\
..\$(TSELITESRC_DIR)\\numeric.h$(EOL)\
..\$(TSELITESRC_DIR)\\time.c$(EOL)\
..\$(TSELITESRC_DIR)\Time.h$(EOL)\
..\$(TSELITESRC_DIR)\Date.c$(EOL)\
..\$(TSELITESRC_DIR)\Date.h$(EOL)\
..\$(TSELITESRC_DIR)\edit.c$(EOL)\
..\$(TSELITESRC_DIR)\edit.h$(EOL)\
..\$(TSELITESRC_DIR)\ListBox.c$(EOL)\
..\$(TSELITESRC_DIR)\ListBox.h$(EOL)\
..\$(TSELITESRC_DIR)\PopupPassword.c$(EOL)\
..\$(TSELITESRC_DIR)\PopupPassword.h$(EOL)\
..\$(TSELITESRC_DIR)\PopupSel.c$(EOL)\
..\$(TSELITESRC_DIR)\PopupSel.h$(EOL)\
..\$(TSELITESRC_DIR)\PopupString.c$(EOL)\
..\$(TSELITESRC_DIR)\PopupString.h$(EOL)\
..\$(TSELITESRC_DIR)\PopupEdit.h$(EOL)\
..\$(TSELITESRC_DIR)\PopupEdit.c$(EOL)\
..\$(TSELITESRC_DIR)\ordlistbox.h$(EOL)\
..\$(TSELITESRC_DIR)\ordlistbox.c$(EOL)\
..\$(TSELITESRC_DIR)\minisetupext.c$(EOL)\
..\$(TSELITESRC_DIR)\\variable.c$(EOL)\
..\$(TSELITESRC_DIR)\AddBootOption.c$(EOL)\
..\$(TSELITESRC_DIR)\\tselitecommon.c$(EOL)\
..\$(TSELITESRC_DIR)\UefiAction.c$(EOL)\
..\$(TSELITESRC_DIR)\UefiAction.h$(EOL)\
..\$(TSELITESRC_DIR)\ResetButton.h$(EOL)\
..\$(TSELITESRC_DIR)\ResetButton.c$(EOL)\
..\$(TSELITESRC_DIR)\hotclick.h$(EOL)\
..\$(TSELITESRC_DIR)\hotclick.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf


##
## Uefi2.1
##
	@$(ECHO) \
"$(EOL)\
..\$(UEFISRC_DIR)\CtrlCond.c$(EOL)\
..\$(UEFISRC_DIR)\CtrlCond.h$(EOL)\
..\$(UEFISRC_DIR)\FormBrowser2.c$(EOL)\
..\$(UEFISRC_DIR)\FormBrowser2.h$(EOL)\
..\$(UEFISRC_DIR)\Expression.c$(EOL)\
..\$(UEFISRC_DIR)\hii.c$(EOL)\
..\$(UEFISRC_DIR)\HiiCallback.c$(EOL)\
..\$(UEFISRC_DIR)\HiiNotificationHandler.c$(EOL)\
..\$(UEFISRC_DIR)\Parse.c$(EOL)\
..\$(UEFISRC_DIR)\TseUefiHii.h$(EOL)\
..\$(UEFISRC_DIR)\uefi21wapper.c$(EOL)\
..\$(UEFISRC_DIR)\UefiTianoHii.h$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

##
## Ezport
##
ifeq ($(TSE_STYLE_EZPORT_SUPPORT), 1)
	@$(ECHO) \
"$(EOL)\
..\$(TSE_STYLE_DIR)\ezport.c$(EOL)\
..\$(TSE_STYLE_DIR)\ezport.h$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

##
## EzportPlus
##
ifeq ($(TSE_STYLE_EZPORTPLUS_SUPPORT), 1)
	@$(ECHO) \
"$(EOL)\
..\$(TSE_STYLE_DIR)\ezportplus.h$(EOL)\
..\$(TSE_STYLE_DIR)\ezportplus.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

##
## Legacy
##
ifeq ($(TSE_STYLE_LEGACY_SUPPORT), 1)
	@$(ECHO) \
"$(EOL)\
..\$(TSE_STYLE_DIR)\legacy.h$(EOL)\
..\$(TSE_STYLE_DIR)\legacy.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

##
## Style common files
##
	@$(ECHO) \
"..\$(TSE_STYLE_DIR)\stylecommon.c$(EOL)\
..\$(TSE_STYLE_DIR)\style.h$(EOL)\
..\$(TSE_STYLE_DIR)\style.c$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

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
  MdePkg/MdePkg.dec$(EOL)\
  MdeModulePkg/MdeModulePkg.dec$(EOL)\
  IntelFrameworkModulePkg/IntelFrameworkModulePkg.dec$(EOL)\
  IntelFrameworkPkg/IntelFrameworkPkg.dec$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_Packages))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## LibraryClasses Section
##
	@$(ECHO) \
"$(EOL)\
[LibraryClasses]$(EOL)\
  MemoryAllocationLib$(EOL)\
  BaseLib$(EOL)\
  BaseMemoryLib$(EOL)\
  PerformanceLib$(EOL)\
  DevicePathLib$(EOL)\
  DebugLib$(EOL)\
  UefiLib$(EOL)\
  PrintLib$(EOL)\
  UefiDriverEntryPoint$(EOL)\
  ReportStatusCodeLib$(EOL)\
  PcdLib$(EOL)\
  UefiBootServicesTableLib$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf

ifeq ($(TSE_HASH_PASSWORD),1)
	@$(ECHO) \
"BaseCryptLib$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
endif

##
## Protocols Section
##
	@$(ECHO) \
"$(EOL)\
[Protocols]$(EOL)\
  gEfiFirmwareVolume2ProtocolGuid$(EOL)\
  gEfiUnicodeCollation2ProtocolGuid$(EOL)\
  gEfiUnicodeCollationProtocolGuid$(EOL)\
  gEfiOEMBadgingProtocolGuid$(EOL)\
  gEfiSimpleFileSystemProtocolGuid$(EOL)\
  gEfiUnicodeCollation2ProtocolGuid$(EOL)\
  gEfiUnicodeCollationProtocolGuid$(EOL)\
  gEfiBlockIoProtocolGuid$(EOL)\
  gEfiLegacyBiosProtocolGuid$(EOL)\
  gEfiFirmwareVolumeProtocolGuid$(EOL)\
  gEfiSimpleTextInProtocolGuid$(EOL)\
  gEfiLoadedImageProtocolGuid$(EOL)\
  gEfiFirmwareVolume2ProtocolGuid$(EOL)\
  gAmiPostManagerProtocolGuid$(EOL)\
  gTSEInvalidateBgrtStatusProtocolGuid$(EOL)\
  gEsaInterfacesForTseProtocolGuid$(EOL)\
  gEfiLoadFileProtocolGuid$(EOL)\
  gEfiUsbIoProtocolGuid$(EOL)\
  gEfiDiskIoProtocolGuid$(EOL)\
  gEfiDevicePathFromTextProtocolGuid$(EOL)\
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
  gEfiMdeModulePkgTokenSpaceGuid.PcdProgressCodeOsLoaderLoad$(EOL)\
  gEfiMdeModulePkgTokenSpaceGuid.PcdProgressCodeOsLoaderStart$(EOL)\
  $(subst $(SPACE),$(EOL)$(SPACE),$(ADDITIONAL_AMITSE_PCDs))$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## DEPEX Section
##
	@$(ECHO) \
"$(EOL)\
[Depex]$(EOL)\
  TRUE$(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
##
## BuildOptions Section
##
	@$(ECHO) \
"$(EOL)\
[BuildOptions]$(EOL)\
  MSFT:*_*_*_CC_FLAGS = /Od /Oy- $(EOL)\
  GCC:*_*_*_CC_FLAGS = -nodefaultlibs $(EOL)\
">> $(BUILD_DIR)/AMITSE.inf
