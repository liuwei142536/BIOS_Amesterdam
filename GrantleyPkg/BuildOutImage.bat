@REM
@REM This file contains an 'Intel Peripheral Driver' and is      
@REM licensed for Intel CPUs and chipsets under the terms of your
@REM license agreement with Intel or your vendor.  This file may 
@REM be modified by the user, subject to additional terms of the 
@REM license agreement                                           
@REM
@REM #/*++
@REM #  
@REM #  Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>
@REM #  This software and associated documentation (if any) is furnished
@REM #  under a license and may only be used or copied in accordance
@REM #  with the terms of the license. Except as permitted by such
@REM #  license, no part of this software or documentation may be
@REM #  reproduced, stored in a retrieval system, or transmitted in any
@REM #  form or by any means without the express written consent of
@REM #  Intel Corporation.
@REM #
@REM #  Module Name:
@REM #  
@REM #    BuildOutImage.bat
@REM #  
@REM #  Abstract:
@REM #    
@REM #    Platform flash image build script. 
@REM #
@REM #--*/

if not exist %OUTPUT_DIR%\ROM mkdir %OUTPUT_DIR%\ROM

@REM ---run FTool and create ROM images---
  cd %WORKSPACE%\%OUTPUT_DIR%\ROM
@REM OEM RELEASE NOTE START: INTEGRATE RESTRICTED ITEM
%WORKSPACE%\%1\Tools\FlashImageTool\spsFITC.exe /s %WORKSPACE%\%1 /b /bios %WORKSPACE%\%OUTPUT_DIR%\FV\AMIROM.fd %WORKSPACE%\%2\%3.xml /o %WORKSPACE%\%OUTPUT_DIR%\ROM\OutImage.bin
if NOT "%~4" == "" %WORKSPACE%\%1\Tools\FlashImageTool\spsFITC.exe /s %WORKSPACE%\%1 /b /bios %WORKSPACE%\%OUTPUT_DIR%\FV\AMIROM.fd /der %WORKSPACE%\%1\PTU\SpsNMPTU.rom %WORKSPACE%\%2\%4.xml /o %WORKSPACE%\%OUTPUT_DIR%\ROM\OutImage_NM.bin

@REM OEM RELEASE NOTE END
cd %WORKSPACE%
cd %1\Tools\FlashImageTool
  @del spsfitc.ini /f /q
@echo off
