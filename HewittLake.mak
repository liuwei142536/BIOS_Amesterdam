#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
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
# $Header:  $
#
# $Revision:  $
#
# $Date:  $
#**********************************************************************
# Revision History
# ----------------
# $Log:  $
# 
# 
#**********************************************************************
#<AMI_FHDR_START>
#
# Name:	Grantley.mak
#
# Description:	
#
#<AMI_FHDR_END>
#**********************************************************************

#Do not change the order of Platform configurations.

ifeq ($(GRANGEVILLE_PLATFORM_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D GRANGEVILLE_FLAG
endif

ifeq ($(PWP_FEATURE_EN), 1)
EXTERNAL_CC_FLAGS += /D PWP_FEATURE
endif
 
ifeq ($(CRB_FLAG_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /D CRB_FLAG /D VFR_CRB_FLAG /D EXTERNAL_PACKAGE
endif

ifeq ($(SDP_PLATFORM), 1)
EXTERNAL_CC_FLAGS += /D SDP_FLAG /D SV_OR_SDP_FLAG
endif

ifeq ($(BDX_DE_CPU_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /D HSX_HOST  /D BDX_HOST /D DE_SKU
endif

ifeq ($(SV_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /D SV_HOOKS /D SV_HOOKS_CPU /D SV_HOOKS_PCH /D QPI_SV_HOOKS /D SV_OR_SDP_FLAG /D MEM_SV_HOOKS
endif

ifeq ($(SOFTSDV_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /D SOFTSDV /D SOFTSDV_FLAG
endif

ifeq ($(INTEL_TXT_SUPPORT), 1)
EXTERNAL_CC_FLAGS +=  /D LT_FLAG
endif

ifeq ($(SPLIT_RECOVERY_FV), 1)
EXTERNAL_CC_FLAGS +=  /D SPLIT_RECOVERY_FV
endif

ifeq ($(TRAD_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DTRAD_FLAG=1
endif

ifeq ($(ULT_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DULT_FLAG=1
endif

ifeq ($(SERIAL_IO_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DSERIAL_IO_FLAG=1
endif

ifeq ($(SUS_WELL_RESTORE_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DSUS_WELL_RESTORE=1
endif

ifeq ($(ADSP_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DADSP_FLAG=1
endif

ifeq ($(GRANGEVILLE_PO_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /D GRANGEVILLE_PO_FLAG
endif

ifeq ($(USB_PRECONDITION_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DUSB_PRECONDITION_ENABLE_FLAG=1
endif

ifeq ($(RSTE_SUPPORT), 1)
EXTERNAL_CC_FLAGS +=  /DRSTE_TEST=1
endif

ifeq ($(WDT_SUPPORT_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DWDT_SUPPORT_ENABLED=1
endif

ifeq ($(PCH_SERVER_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DPCH_SERVER_BIOS_FLAG=1
endif

ifeq ($(SERVER_BIOS_ENABLE), 1)
EXTERNAL_CC_FLAGS +=  /DSERVER_BIOS_FLAG=1
endif

ifeq ($(STORAGE_FEATURE_EN), 1)
EXTERNAL_CC_FLAGS +=  /D MEM_NVDIMM_EN /D NVMEM_FEATURE_EN
endif

ifeq ($(KAHUNA_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D KAHUNA_PPO_FLAG
endif

ifeq ($(MAYAN_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D MAYAN_PPO_FLAG
endif

ifeq ($(ARANDAS_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D ARANDAS_PPO_FLAG
endif

ifeq ($(AZTEC_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D AZTEC_PPO_FLAG
endif

ifeq ($(AZTECSTHI_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D AZTECSTHI_PPO_FLAG
endif

ifeq ($(HEDT_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D HEDT_PPO_FLAG
endif

ifeq ($(HEDTME_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D HEDT_PPO_FLAG
endif

ifeq ($(INCA_PPO_ENABLE), 1)
EXTERNAL_CC_FLAGS += /D INCA_PPO_FLAG
endif

ifeq ($(ME_ENABLE), 1)
EXTERNAL_CC_FLAGS += /DME_SUPPORT_FLAG=1
	ifeq ($(SPS_ENABLE), 1)
  	EXTERNAL_CC_FLAGS += /DSPS_SUPPORT=1
   	endif
    ifeq ($(AMT_ENABLE), 1) 
    EXTERNAL_CC_FLAGS += /DAMT_SUPPORT=1
		ifeq ($(AT_ENABLE), 1)
         EXTERNAL_CC_FLAGS += /DAT_SUPPORT=1
   		endif
        ifeq ($(ICC_ENABLE), 1)
         EXTERNAL_CC_FLAGS += /DICC_SUPPORT=1
        endif
	endif #ifeq ($(AMT_ENABLE), 1)
endif #ifeq ($(ME_ENABLE), 1)

#Following are EDK build configurations.
#These are not required but defining then, to be in safer side.
ifeq ($(PFAT_EC_ENABLE), 1)
EXTERNAL_CC_FLAGS += /DPFAT_FLAG=1 /DPFAT_EC_FLAG=1
else
EXTERNAL_CC_FLAGS += /DPFAT_FLAG=1
endif

#MRC debug message support is provided as EXTERNAL flags based on DEBUG_CODE token.
ifeq ($(DEBUG_CODE), 1)
EXTERNAL_CC_FLAGS += /D SERIAL_DBG_MSG
endif

ifeq ($(BTG_ENABLE), 1 )
EXTERNAL_CC_FLAGS += /D BTG_FLAG
endif

ifeq ($(PCI_IOV_SUPPORT_FLAG), 1)
EXTERNAL_CC_FLAGS += /D EFI_PCI_IOV_SUPPORT
endif

#Dynamic mmcfg base address change support is provided as EXTERNAL flags based on DYNAMIC_MMCFG_BASE_SUPPORT token.
ifeq ($(DYNAMIC_MMCFG_BASE_SUPPORT), 1)
EXTERNAL_CC_FLAGS += /D DYNAMIC_MMCFG_BASE_FLAG
endif

EXTERNAL_CC_FLAGS += /wd4819

Prepare: DeleteRomFile Prebuild

DeleteRomFile: 
ifeq ($(SecureMod_SUPPORT),1)
	if exist $(FWCAPSULE_FILE_NAME) $(RM) $(FWCAPSULE_FILE_NAME)
endif	
	if exist $(PROJECT_TAG)*.CAP $(RM) $(PROJECT_TAG)*.CAP
	if exist $(PROJECT_TAG)*.ROM $(RM) $(PROJECT_TAG)*.ROM
	if exist OutImage*.bin $(RM) OutImage*.bin
	
Prebuild:
	attrib -r GrangevillePkg\PlatformPkgConfig.dsc
	
End : CreateBinFile

OUTPUT_DIR = $(BUILD_DIR)\$(PLATFORM_NAME)\$(TARGET)_$(TOOL_CHAIN_TAG)
export OUTPUT_DIR

.PHONY : CreateBinFile
CreateBinFile:
ifeq ($(SecureMod_SUPPORT),1)
    ifeq ($(wildcard $(FWpriv)), $(FWpriv))
	@echo ----- Copy Signed BIOS Image file to AMIROM.fd for RomImage"
	@copy /Y $(FWCAPSULE_FILE_NAME) $(OUTPUT_DIR)\FV\AMIROM.fd
    else
	@echo ----- Copy Unsigned BIOS Image file from AMIROM.fd"
	@copy /Y $(OUTPUT_DIR)\FV\AMIROM.fd $(BIOS_TAG).ROM
    endif
else
	@copy /Y $(OUTPUT_DIR)\FV\AMIROM.fd $(BIOS_TAG).ROM
endif
	@$(ECHO) Generating OutImage.bin
	@$(ECHO) $(WORKSPACE)$(PATH_SLASH)$(FITC_EXE_DIR)/spsFITC.exe /s $(WORKSPACE)$(PATH_SLASH)$(FITC_EXE_DIR) /b /bios $(WORKSPACE)$(PATH_SLASH)$(OUTPUT_DIR)/FV/AMIROM.fd /gbe $(WORKSPACE)$(PATH_SLASH)$(FITC_BIN_DIR)$(PATH_SLASH)$(GBE_BIN) /pdr $(WORKSPACE)$(PATH_SLASH)$(FITC_BIN_DIR)$(PATH_SLASH)$(PDR_BIN) /der $(WORKSPACE)$(PATH_SLASH)$(FITC_ROM_DIR)$(PATH_SLASH)$(PTU_ROM) $(WORKSPACE)$(PATH_SLASH)$(FITC_XML_DIR)$(PATH_SLASH)$(Grangeville_XML_config) /o $(WORKSPACE)$(PATH_SLASH)$(OUTPUT_DIR)/ROM/OutImage.bin
	$(WORKSPACE)$(PATH_SLASH)$(FITC_EXE_DIR)/spsFITC.exe \
	/s $(WORKSPACE)$(PATH_SLASH)$(FTOOL_DIR) \
	/b /bios $(WORKSPACE)$(PATH_SLASH)$(OUTPUT_DIR)/FV/AMIROM.fd \
	/gbe $(WORKSPACE)$(PATH_SLASH)$(FITC_BIN_DIR)$(PATH_SLASH)$(GBE_BIN) \
	/pdr $(WORKSPACE)$(PATH_SLASH)$(FITC_BIN_DIR)$(PATH_SLASH)$(PDR_BIN) \
	/der $(WORKSPACE)$(PATH_SLASH)$(FITC_ROM_DIR)$(PATH_SLASH)$(PTU_ROM) \
	$(WORKSPACE)$(PATH_SLASH)$(FITC_XML_DIR)$(PATH_SLASH)$(Grangeville_XML_config) \
	/o $(WORKSPACE)$(PATH_SLASH)OutImage.bin
	
ifeq ($(SecureMod_SUPPORT),1)
    ifeq ($(wildcard $(FWpriv)), $(FWpriv))
	@$(ECHO) OutImage.bin and $(FWCAPSULE_FILE_NAME) binaries created.
    else
	@$(ECHO) OutImage.bin and $(BIOS_TAG).ROM binaries created.
    endif
else
	@$(ECHO) OutImage.bin and $(BIOS_TAG).ROM binaries created.
endif

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************