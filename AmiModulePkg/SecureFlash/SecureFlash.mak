#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
## @file
#	Includes main build module for Secure sub-components
#   1. Create a help file and a batch with instructions to sign Fw Capsule using CryptoCon.exe
#   2. Create extended Rom layout map file RomLayoutEx.bin 
#   3. Update FwCapsuleHdr ffs file inside BIOS.ROM with RomLayoutEx map 
#   4. Create signed BIOS image (Aptio FW Capsule)
##

.PHONY : CLEAR_FWCAPSULE_FILES MAKE_FWCAPSULE_HELP_FILES MOD_FWCAPSULE_HDR_FFS

#---------------------------------------------------------------------------
# Only for PKCS1v2.1 Key files the FWpub(public key part) can be derived 
# from FWpriv(full RSA key) if FWpub file is not available
#---------------------------------------------------------------------------
ifeq ($(FWCAPSULE_CERT_FORMAT),0)
ifneq ($(wildcard $(FWpub)), $(FWpub))
ifeq ($(wildcard $(FWpriv)),$(FWpriv))
FWpub := $(FWpriv)
endif
endif
endif

FWCAPSULE_MAKE_README := make_signed_capsule_readme.txt
FWCAPSULE_MAKE_BAT := make_signed_capsule.bat
#---------------------------------------------------------------------------
#        Prepare Signed Capsule : FWCAPSULE_FILE
#---------------------------------------------------------------------------
clean  : CLEAR_FWCAPSULE_FILES
Prepare: MAKE_FWCAPSULE_HELP_FILES

CLEAR_FWCAPSULE_FILES:
ifneq ("$(wildcard  $(FWCAPSULE_FILE))", "")
	-$(RM) $(FWCAPSULE_FILE)
endif
ifneq ("$(wildcard  $(FWCAPSULE_MAKE_README))", "") 
	-$(RM) $(FWCAPSULE_MAKE_README)
endif
ifneq ("$(wildcard  $(FWCAPSULE_MAKE_BAT))", "") 
	-$(RM) $(FWCAPSULE_MAKE_BAT)
endif

#---------------------------------------------------------------------------
#   1. Create readme and batch files with instructions to build signed Aptio FW Capsule using CryptoCon
#---------------------------------------------------------------------------
MAKE_FWCAPSULE_HELP_FILES: CLEAR_FWCAPSULE_FILES
	@$(ECHO) "@echo ----Generate signed Aptio FW Capsule...$(EOL)\
@echo ----REQUIRED:$(EOL)\
@echo ----Aptio images : '$(notdir $(UNSIGNED_BIOS_ROM))' and '$(notdir $(ROM_LAYOUT_EX))'$(EOL)\
@echo ----Sign keys    : '$(notdir $(FWpriv))' and '$(notdir $(FWpub))'$(EOL)\
@echo ----Ami tools    : 'cryptocon' and 'fwbuild'$(EOL)\
@echo ----OPTIONAL step:$(EOL)\
@echo ---- Re-generate Rom map file if the BIOS binary was modified by Aptio tools$(EOL)\
@echo ---- $(FWBUILD) $(UNSIGNED_BIOS_ROM) $(FWBUILD_OPTION_CHAR)s $(FWBUILD_OPTION_CHAR)m $(ROM_LAYOUT_EX)$(EOL)\
@echo ----Use 'Cryptocon' to sign BIOS image using external rom map file '-r $(notdir $(ROM_LAYOUT_EX))'$(EOL)\
$(CRYPTCON) $(CRYPTOCON_CMDLINE_SIG) -f $(UNSIGNED_BIOS_ROM) -o $(FWCAPSULE_FILE)"\
>> $(FWCAPSULE_MAKE_BAT)
	@$(ECHO) "==================================================================$(EOL)\
===  QUICK GUIDE TO FW CAPSULE SIGNING USING CRYPTOCON$(EOL)\
==================================================================$(EOL)\
1. Use Aptio FW image '$(UNSIGNED_BIOS_ROM)'$(EOL)\
   created on $(TODAY) at $(NOW), $(EOL)\
   Make sure the following FFS files are embedded into a binary:$(EOL)\
    a)FWKey.ffs with a public key to verify signed BIOS updates$(EOL)\
    b)FwCapsuleHdr.ffs with the current Rom Layout map"\
> $(FWCAPSULE_MAKE_README)
ifeq ($(FWCAPSULE_CERT_FORMAT),1)
	@$(ECHO) "2. Provide RSA key in PKCS#12 PFX(private) and X.509 DER(public) formats."\
>> $(FWCAPSULE_MAKE_README)
else
	@$(ECHO) "2. Provide RSA2048 Key in PKCS#1v2.1 DER or PEM format.$(EOL)\
    '$(FWrootKey)' - Root Certificate key (private RSA key)"\
>> $(FWCAPSULE_MAKE_README)
endif
	@$(ECHO) "    '$(FWpriv)' - Signer Certificate key (private RSA key)$(EOL)\
    '$(FWpub)' - Signer Certificate key (public key part)$(EOL)\
3. (Optional)$(EOL)\
    Re-build the Rom map if the original binary '$(UNSIGNED_BIOS_ROM)' was modified$(EOL)\
    $(FWBUILD) $(UNSIGNED_BIOS_ROM) $(FWBUILD_OPTION_CHAR)s $(FWBUILD_OPTION_CHAR)m $(ROM_LAYOUT_EX)$(EOL)\
4. Create signed FwCapsule using sign key from '$(FWpriv)' file$(EOL)\
$(EOL)\
$(CRYPTCON) $(CRYPTOCON_CMDLINE_SIG) -f $(UNSIGNED_BIOS_ROM) -o $(FWCAPSULE_FILE)$(EOL)\
$(EOL)\
=========================================================$(EOL)\
=== Cryptocon commands used to build FwCapsule$(EOL)\
=========================================================$(EOL)\
-c'FWrootPriv' -k'FWsignPriv' Make PKCS#1v1.5 signed FwCapsule(Note1)$(EOL)\
-c2 -x 'FWpriv'[,'pswd']      Make PKCS#7 signed FwCapsule(Note2,Note3)$(EOL)\
-f'file' input, un-signed BIOS image$(EOL)\
-o'file' output, signed FwCapsule image$(EOL)\
-y use embedded FwCapsule header to insert the signature$(EOL)\
-l'value' max size of a FwCapsule Header (or Hdr alignment)$(EOL)\
-p RSASSA-PSS signature padding scheme (replaces default PKCS#1v1.5)$(EOL)\
-n -k'key' insert Signer's public key inside of the signed image$(EOL)\
-r'rom.map' use a rom map from the external file$(EOL)\
-r1 ignore the embedded rom map - calculate hash for entire image$(EOL)\
-$$[Offs] offset within the file to adjust the RomMap ranges by$(EOL)\
-h2(3,5) Select hash algorithm to be used for signed digest$(EOL)\
         Supported hash types: sha256(-h2), sha384(-h3) and sha512(-h5)$(EOL)\
=============================================================$(EOL)\
Note1. -c'key1'-k'key2'         :use PKCS#1v2.1 DER(PEM) encoded RSA2048 keys$(EOL)\
Note2. -c2 -x'key1,[pswd]'-k'key2':key1-PKCS#12(PFX) with optional PFX password;$(EOL)\
                                 key2-X.509(DER) public key of 'key1'$(EOL)\
Note3. -c2 -x command will invoke MS signtool.exe or openSSL(-*) as external process$(EOL)\
=============================================================$(EOL)\
=== 4 step FwCapsule signing using external(remote) sign process$(EOL)\
=============================================================$(EOL)\
Example of generating Fw Capsule signed with PKCS#7 certificate using 3rd party sign tools$(EOL)\
1. Embed signer verification key $(FWpub) into the BIOS image$(EOL)\
   $(notdir $(CRYPTCON)) -c2 -y -n -k $(FWpub) -f $(notdir $(UNSIGNED_BIOS_ROM)) -o $(notdir $(UNSIGNED_BIOS_ROM))$(EOL)\
2. Serialize all BIOS regions to be signed$(EOL)\
   $(notdir $(CRYPTCON)) -s -c2 -y -r $(notdir $(ROM_LAYOUT_EX)) -f $(notdir $(UNSIGNED_BIOS_ROM)) -o serial.bin$(EOL)\
3. Sign serial image and store a signature in DER encoded PKCS#7 signed certificate file format (serial.bin.p7)$(EOL)\
   The choice of supported hash algorithms: sha256, sha384, sha512$(EOL)\
   Examples of using Microsoft Signtool.exe or openSSL(MS or Linux) with local access to sign keys:$(EOL)\
    SignTool.exe sign /fd sha256 /p7 . /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /p FW_PFX_Password /f FWpriv serial.bin$(EOL)\
    openssl smime -sign -md sha256 -binary -noattr -in serial.bin -out serial.bin.p7 -outform DER -inkey FWpriv -signer FWpub -passin pass:FW_PFX_Password$(EOL)\
   Examples of signing with externally managed sign keys:$(EOL)\
   A computer store is specified for the certification authority (CA) store. Certificate is identified by a Subject Name "My High Value Certificate"$(EOL)\
    SignTool sign /fd sha256 /p7 .\ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /sm /n "My High Value Certificate" serial.bin$(EOL)\
   A private key is protected by a hardware cryptography module (e.g. HSM)$(EOL)\
    SignTool sign /fd sha256 /p7 .\ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /csp 'Hardware Cryptography Module' /f SigCert.cer /k KeyContainerName serial.bin$(EOL)\
   The HSM managed sign key is referenced in the certificate store by its SHA1 hash.$(EOL)\
    Signtool.exe sign /fd sha256 /p7 .\ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /a /sha1 "db314da0d0ef87d42b42f74b9c38a1f9173ef7a2" /sm serial.bin$(EOL)\
4. Insert a signature from "serial.bin.p7" into a target BIOS image$(EOL)\
   $(notdir $(CRYPTCON)) -s -x serial.bin.p7 -c2 -y -r $(notdir $(ROM_LAYOUT_EX)) -f $(notdir $(UNSIGNED_BIOS_ROM)) -o $(notdir $(FWCAPSULE_FILE))"\
>> $(FWCAPSULE_MAKE_README)

#--------------------------------------------------------------------------
# 2. Creating Signing descriptor table (RomLayout map) file
#--------------------------------------------------------------------------
$(ROM_LAYOUT_EX): $(UNSIGNED_BIOS_ROM)
	@echo ----FvCapsule: Build extended rom map '$(ROM_LAYOUT_EX)'
	$(FWBUILD) $< $(FWBUILD_OPTION_CHAR)s $(FWBUILD_OPTION_CHAR)m $@

#---------------------------------------------------------------------------
# 3. Embed Signing descriptor table "$(ROM_LAYOUT_EX)" inside "$(UNSIGNED_BIOS_ROM)"
#--------------------------------------------------------------------------
MOD_FWCAPSULE_HDR_FFS: $(ROM_LAYOUT_EX) $(UNSIGNED_BIOS_ROM)
	@echo ----Update '$(UNSIGNED_BIOS_ROM)' with extended rom map '$(ROM_LAYOUT_EX)'
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
	chmod +x $(CRYPTCON:-@=)
endif
	$(CRYPTCON) $(CRYPTOCON_CMDLINE_MAP) -f $(UNSIGNED_BIOS_ROM) -o $(UNSIGNED_BIOS_ROM)

#---------------------------------------------------------------------------
# 4. Invoke cryptocon.exe to create Signed FwCapsule if CREATE_FWCAPSULE == 1
#---------------------------------------------------------------------------
# Should be the last step after creating of the ROM image. All fixups to the .ROM must be made prior to this step.
# check END target in the MAIN.MAK and all .MAK files to make sure this step is not overriden
#---------------------------------------------------------------------------
$(FWCAPSULE_BUILD_TARGET): $(FWCAPSULE_FILE)
$(FWCAPSULE_FILE): MOD_FWCAPSULE_HDR_FFS $(UNSIGNED_BIOS_ROM) $(ROM_LAYOUT_EX)
ifeq ($(CREATE_FWCAPSULE),1)
	@echo ----Create signed BIOS image '$(FWCAPSULE_FILE)'
	-$(CRYPTCON) $(CRYPTOCON_CMDLINE_SIG) -f $(UNSIGNED_BIOS_ROM) -o $(FWCAPSULE_FILE)
ifneq ($(BUILD_OS), $(BUILD_OS_LINUX))
	@if not exist $@ @echo WARNING !!! Signed BIOS image '$@' not found.
else
	@if [ ! -f "$@" ]; then echo "WARNING !!! Signed BIOS image '$@' not found."; fi
endif
ifneq ($(wildcard $(FWpub)),$(FWpub))
	@echo WARNING !!!'$(FWpub)' file with RSA public key not found.
	@echo Final BIOS image includes the dummy key as a Platform Key placeholder.
	@echo WARNING !!! The flash updates will fail, unless the valid key is set in FWKey FFS.
	@echo Provide a key at a time of build OR insert a key into a BIOS binary using cryptocon switch '-n -k key'.
endif
ifeq ($(FWCAPSULE_CERT_FORMAT),1)
ifeq ($(CONFIG_PEI_PKCS7),0)
$(warning *** PKCS#7 certificate parser in PEI is disabled through SDL in AmiCryptoPkg:CONFIG_PEI_PKCS7.)
$(error The 'Capsule' and 'Recovery' flash updates will fail on PKCS#7 Signed images)
endif
endif
endif #ifeq  ($(CREATE_FWCAPSULE),1)
