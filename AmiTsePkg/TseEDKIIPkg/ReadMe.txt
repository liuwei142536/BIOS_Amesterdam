*****************************************************************
*****************************************************************
**                                                             **
**        (C)Copyright 1985-2017, American Megatrends, Inc.    **
**                                                             **
**                     All Rights Reserved.                    **
**                                                             **
**             5555 Oakbrook Pkwy   , Norcross, GA 30071       **
**                                                             **
**                     Phone: (770)-246-8600                   **
**                                                             **
*****************************************************************
*****************************************************************

================================================================================
                                   DISCLAIMER
================================================================================
This release note as well as the software described in it is furnished under license
and may only be used or copied in accordance with the terms of the license. The
information in this manual is furnished for informational use only, is subject to
change without notice, and should not be construed as a commitment by AMI Inc.
AMI Inc assumes no responsibility or liability for any errors or inaccuracies
that may appear in this document or any software that may be provided in association
with this document.
Except as permitted by such license, no part of this document may be reproduced,
stored in a retrieval system, or transmitted in any form or by any means without
the express written consent of AMI Inc.

================================================================================
The puporse of this document is to explain how to generate an EDKII compatible 
AmiTsePkg and integrate it. The AmiTsePkg will override the SetupBrowser that comes 
by default with EDKII. The following sequence of steps are mandatory to achieve the same.
Make sure you follow the steps as-is. The steps given are verified in 
Microsoft Windows 7 Enterprise 64-bit* 
================================================================================


================================================================================
                                     INDEX
================================================================================
1.  PREREQUISITES

2.  HOW TO GENERATE AMITSEPKG

3.  HOW TO BUILD AMITSEPKG IN EDK PROJECT

4.  HOW TO ADD ADDITIONAL TSE FILES INTO THE PACKAGE

================================================================================




================================================================================
                           PREREQUISITES 
================================================================================
1.  Any AptioV compatible project. Either NT32 or x64 project will do.
    Used NT32 in this case.

2.  AptioV AmiTsePkg - with Source and Binary
	Used latest AMITSE modules in this case.
	
3.  AptioV Build Tools.
    Used BuildTools_31 in this case.

================================================================================




================================================================================
                  HOW TO GENERATE AMITSEPKG  (FOR WINDOWS)
================================================================================
Build System Test Configuration:
  Microsoft Windows 7 Ultimate 64-bit*

1.  Package Creation Environment
    1)  Download any AptioV project.

    2)  Download the latest AptioV AMITSE modules from SVN.
			a. Tse Binary - $AptioV\Binary\Modules\AMITSE;
			b. Tse Source - $AptioV\Source\Modules\AMITSE;
			c. AmiTsePkg  - $AptioV\Source\Interface\AmiTsePkg;

    3)  Download the Windows BuildTools from the SVN path $AptioV\BuildTools\Windows\X86\.

2.  Package Creation
    1)  Copy TseEDKIIPkg and paste it under $(AptioV project)\AmiTsePkg\.

    2)  Open TseEDKII.bat,

			a. Update the Project path in the variable PROJECT_PATH (without backslash at the end of the path). eg: J:\Testing\NT32_3

			b. Update the Build tools path in the variable BUILDTOOLS_PATH (without backslash at the end of the path). eg: J:\SoftWares\Build_Tools\Tools_31


    3)  Open TseEDKII.sdl and enable TSE_EDKII_PACKAGE_GEN_SUPPORT token.
    
    4)  In TseEDKII.sdl, update the EDKII_BUILD_ARCH_PATH token with the Edk project's Platform build path (without backslash at the end of the path). 

		- Update in the format of <Build/$(PLATFORM_NAME)$(ARCH)/$(TARGET)_$(TOOL_CHAIN_TAG)/$(ARCH)>.

		- eg: "Build/NT32IA32/DEBUG_VS2008x86/IA32"
			here, PLATFORM_NAME  =  NT32
			      ARCH           =  IA32
			      TARGET         =  DEBUG
                              TOOL_CAHIN_TAG =  VS2008X86

    4)  Run the TseEDKII.bat file. 

    5)  Once the bat file completes execution, make sure below mentioned files under build folder.
		- AMITSE.inf
		- AMITSEElinks.h
		- AMITSEOem.h
		- HotcliskElinks.h
		- HotKeyElinks.h
		- TimeStamp.h
		- Token.h   - make sure EDKII_BUILD_ARCH_PATH macro present in this file.


    6)  Copy the AmiTsePkg from the path <Project location>\Build\TseEDKII\AmiTsePkg

================================================================================



	

================================================================================
                  HOW TO BUILD AMITSEPKG IN EDK PROJECT
================================================================================
	
	
    1) 	Paste the generated AmiTsePkg under EDK project's root folder.
    
    2)  Open EDK project's .dsc file and Search for "SetupBrowserdxe.inf". Replace with "AmiTsePkg/AMITSE.inf".

    3)  Open EDK project's .fdf file and Search for "SetupBrowserdxe.inf". Replace with "AmiTsePkg/AMITSE.inf".

    4)  Build the project.

================================================================================





================================================================================
             HOW TO ADD ADDITIONAL TSE FILES INTO THE PACKAGE
================================================================================

    1) Copy the required source files and paste it under generated AmiTsePkg folder.
    
    2) Update the source file's path under [Sources] section of AMITSE.inf.
	
    3) Update the binary file's path under [Binaries] section of AMITSE.inf.

    4) Update the required package, libraries, protocol and guids under correcponding section in AMITSE.inf.

    5) Build the project.

================================================================================



================================================================================
            OVERRIDING GENERIC TSE TOKEN IN TSEEDKII.SDL
================================================================================

    1) TSE_FOR_EDKII_SUPPORT = 1
        - This token should be enabled to build TSE package for EDK project. This is to avoid creating AmiTsePkg with Aptio related operations because they will not be present on EDKII project.
    
    2) TSE_USE_EDK_LIBRARY = 1
        - This token should be enable to depend on the EDK Libraries instead of Aptio libraries.
    
    3) SUPPORT_ESRT = 0
        - In TSE, ESRT is used for logo and post screen related operation(Capsule update). Since TSE does not support post screen in EDKII project we need to disable this token.

    4) TSE_NVRAM_DEFAULTS_SUPPORT = 0
        - EDK will not have AMI NVRAM. It will have its own nvram support, so we need to disable this token to avoid build error.

    5) TSE_HASH_PASSWORD = 0
        - By default EDK project may not have CryptoPkg. User can enable this token in EdkCompatible.sdl before generating package, if they want
        
    6) RESERVED_ADD_DEL_BOOT_OPTION_NAME = "UEFI:"
        - To avoid build error in AmiTsePkg with EDK project. If this token is not available then undeclared identifier error will come for this token.
        
    7) TSE_USE_AMI_EFI_KEYCODE_PROTOCOL = 0
        - TSE_USE_AMI_EFI_KEYCODE_PROTOCOL is Aptio specific protocol. It will not be available in EDKII project, so we should disable this token.
        
    8) TSE_USE_FILE_BROWSER_PROTOCOL = 0
        - TSE_USE_FILE_BROWSER_PROTOCOL is Aptio specific protocol. It will not be available in EDKII project, so we should disable this token.
        
    9) SETUP_GROUP_DYNAMIC_PAGES = 0
        - This token will group all the dynamic page inside advanced page. This is specific to AptioV. So we need to disable this token.

================================================================================


[END OF README.TXT]
