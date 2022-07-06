#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

#*************************************************************************
#<AMI_FHDR_START>
#
# Name: Variables.mak
#
# Description: includes default Secure Boot policy variables into the FV
# 
#<AMI_FHDR_END>
#*************************************************************************
SEC_BOOT_VAR_LIST := PK KEK db
ifeq ($(DBT_include),1)
SEC_BOOT_VAR_LIST += dbt
endif
ifeq ($(DBX_include),1)
SEC_BOOT_VAR_LIST += dbx
endif
ifeq ($(DBR_include),1)
SEC_BOOT_VAR_LIST += dbr
endif

# to maintain backward comp with "Certificates (Secure Boot, Create OEM Variables)" eModule before label 18
PkVarFile := $(PKVarFile)
KekVarFile:= $(KEKVarFile)

#---------------------------------------------------------------------------
#   Prepare Secure Variables in Build folder to be added as .FFS to FV
#---------------------------------------------------------------------------
define SecBootVar_template =
Prepare: $(BUILD_DIR)$(PATH_SLASH)$(1)
$(BUILD_DIR)$(PATH_SLASH)$(1): $$($(1)Var)
	$(CP) $$< $$@
endef
$(foreach var,$(SEC_BOOT_VAR_LIST),$(eval $(call SecBootVar_template,$(var))) )
