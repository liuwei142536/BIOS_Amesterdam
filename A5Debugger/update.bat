@echo OFF
echo (C)Copyright 2015, American Megatrends, Inc.  All Rights Reserved
IF [%1] == [/?] GOTO help
IF [%1] == [/i] GOTO install
IF [%1] == [/r] GOTO regsymbol
IF [%1] == [/m] GOTO msdia
IF [%1] == [/u] GOTO uregsym
IF [%1] == [/f] GOTO installWithoutVersionCompare
IF [%1] == [/d] GOTO installDependency
 

GOTO help

:install
IF [%2] == [] GOTO err1
IF NOT EXIST %2 GOTO err2

echo installing in %2
SET VEB_ROOT=%2
set iscopy=false
set extractzip=false

IF NOT EXIST "plugins\com.ami.debugger.ui.jar" GOTO err3




REM perform installation, if debugger plugins not present at VeB location.
IF NOT EXIST "%VEB_ROOT%\plugins\com.ami.debugger.ui.jar" set iscopy=true


REM if debugger plugins present at install location and VeB location both, then call the Extract.vbs to compare the Bundle-Version number of manifest file

wscript Extract.vbs "plugins\com.ami.debugger.ui.jar" "%VEB_ROOT%\plugins\com.ami.debugger.ui.jar"

REM if errorlevel is 2 or More than 2 then installation required.
if %errorlevel%==0  echo Debugger Versions are same, No Installation Required
if %errorlevel%==1  echo VEB already has Higher Version Debugger , No Need to install lower version Debugger 
if %errorlevel% GEQ 2  set iscopy=true
GOTO copyAll
REM install without bothering about version like install.bat
:copyAll
REM start copying necessary files, 

REM Copy the debug loader plugin first
echo installing DebugLoader Plugin in %VEB_ROOT%
IF NOT EXIST .\com.ami.debug_1.0.0.zip GOTO err4

REM Start Copying DebugLoader Plugin in VEB Path
:Extractloader 
IF EXIST "%VEB_ROOT%\plugins\com.ami.debug_1.0.0" rmdir /s /q "%VEB_ROOT%\plugins\com.ami.debug_1.0.0"
wscript loaderExtract.vbs "%cd%\com.ami.debug_1.0.0.zip" "%VEB_ROOT%\plugins\"
if not %errorlevel%==0 GOTO err5

REM Copy the debugger core-ui plugins and dlls
if "%iscopy%"=="true" xcopy . %VEB_ROOT% /S /Y /EXCLUDE:excludelist.txt
if not %errorlevel%==0 GOTO exit

IF EXIST plugins\org.eclipse.ui.intro.universal_3.2.500.v20110510.zip set extractzip=true
GOTO Extract

:Extract 
IF EXIST "%VEB_ROOT%\plugins\org.eclipse.ui.intro.universal_3.2.500.v20110510" rmdir /s /q "%VEB_ROOT%\plugins\org.eclipse.ui.intro.universal_3.2.500.v20110510"
if "%extractzip%"=="true" wscript loaderExtract.vbs "%cd%\plugins\org.eclipse.ui.intro.universal_3.2.500.v20110510.zip" "%VEB_ROOT%\plugins\"
if NOT %errorlevel%==0 GOTO exit


IF NOT EXIST .\Tracehub.zip echo tracehub.zip not present.Please install using NDA collateral
if Exist .\Tracehub.zip (

	IF EXIST "%VEB_ROOT%\Tracehub" rmdir /s /q "%VEB_ROOT%\Tracehub"
	wscript loaderExtract.vbs "%cd%\Tracehub.zip" "%VEB_ROOT%\"
	if not %errorlevel%==0 GOTO err5

	IF NOT EXIST .\targets.zip echo targets.zip not present.Please install using NDA collateral
	IF EXIST .\targets.zip (
	wscript loaderExtract.vbs "%cd%\targets.zip" "%VEB_ROOT%\Tracehub"
	if not %errorlevel%==0 GOTO err5
	
)
)




echo Extracting Scripts
IF NOT EXIST .\scripts.zip GOTO errscrips
IF EXIST "%VEB_ROOT%\scripts" rmdir /s /q "%VEB_ROOT%\scripts"
wscript loaderExtract.vbs "%cd%\scripts.zip" "%VEB_ROOT%\"
if not %errorlevel%==0 GOTO err5

IF not [%2]==[/usb3]  GOTO skipusb3Installation


:installUSB3Driver
IF NOT EXIST ..\AMIDebuggerPkg\Debugger\USB3DriverPkg.zip echo unable to locate usb3package, skip usb3 driver installation
IF NOT EXIST ..\AMIDebuggerPkg\Debugger\USB3DriverPkg.zip GOTO skipusb3Installation
IF EXIST "%cd%\USB3DriverPkg.zip" del %cd%\USB3DriverPkg.zip
IF EXIST "%cd%\USB3DriverPkg" rmdir /s /q "%cd%\USB3DriverPkg"
xcopy ..\AMIDebuggerPkg\Debugger\USB3DriverPkg.zip %cd%
wscript loaderExtract.vbs "%cd%\USB3DriverPkg.zip" "%cd%"
IF NOT EXIST ./USB3DriverPkg/AMI_USB3.0_Debug_Driver.bat GOTO errdriverbatch
CD %cd%\USB3DriverPkg
call AMI_USB3.0_Debug_Driver.bat /i
echo USB3 driver registration complete.
CD ..
IF EXIST "%cd%\USB3DriverPkg.zip" del %cd%\USB3DriverPkg.zip
IF EXIST "%cd%\USB3DriverPkg" rmdir /s /q "%cd%\USB3DriverPkg"
if NOT %errorlevel%==0 GOTO exit

:skipusb3Installation

REM continue register
cd %VEB_ROOT%

:regsymbol

IF NOT EXIST SymbolManager.dll GOTO errornodll
regsvr32 /u /s SymbolManager.dll
regsvr32 /s SymbolManager.dll
if not %errorlevel%==0 goto regfail
echo Registering SymbolManager [1/2] ... OK
IF NOT EXIST msdia80.dll GOTO errornodll
regsvr32 /u /s msdia80.dll
regsvr32 /s msdia80.dll
if not %errorlevel%==0 goto regfail
echo Registering SymbolManager[2/2] ... component registration complete.
GOTO exit

:installDependency
IF EXIST vcredist_x86_2010.exe start /w vcredist_x86_2010.exe /passive /promptrestart /log vcredist_x86_2010.log
IF EXIST vcredist_x86_2012.exe start /w vcredist_x86_2012.exe /install /passive /norestart /log vcredist_x86_2012.log
IF EXIST vcredist_x86_2013.exe start /w vcredist_x86_2013.exe /install /passive /norestart /log vcredist_x86_2013.log

REM for forceful installation without version Comparison  
:installWithoutVersionCompare
IF [%2] == [] GOTO err1

IF [%2]==[/usb3] SET VEB_ROOT=%3
IF not [%2]==[/usb3] SET VEB_ROOT=%2

IF NOT EXIST %VEB_ROOT% GOTO err2


echo installing in %VEB_ROOT%
set iscopy=true
GOTO copyAll

:uregsym
regsvr32 /u /s SymbolManager.dll
if not %errorlevel%==0 goto unregfail
echo AptioVDebugger host Components successfully unregistered.
GOTO exit

:msdia
IF NOT EXIST msdia.dll GOTO errdia
regsvr32 /u /s msdia.dll
regsvr32 /s msdia.dll
GOTO exit

REM Error branch starts
:err1
echo Usage 1:
echo update.bat /f [vebroot]
GOTO exit

echo Usage 2:
echo update.bat /f /usb3 [vebroot]
GOTO exit

:err2
echo Incorrect VeB path
GOTO exit

:err3
echo AptioV Debugger plugins\com.ami.debugger.ui.jar file not present for installation.
GOTO exit

:err4
echo AptioV Debugger plugins\com.ami.debug.zip file not present for installation.
GOTO exit

:err5
echo AptioV Debugger Loader plugins could not be copied successfully.
GOTO exit

:err6
echo AptioV Debugger script folder not copied successfully
GOTO exit

:errscrips
echo AptioV Debugger Script.zip not present
GOTO exit



:errornodll
echo Re-install AptioV Debugger and run the command from VeB root
GOTO exit

:errdia
echo msdia.dll should be available in the root of VeB to execute this option
GOTO exit


:regfail
echo Registering components failed.
EXIT /B 1

:unregfail
echo Unable to find any previously registered components
GOTO exit

:errdriver
echo USB3Driver does not exist
GOTO exit

:errdriverbatch
echo unable to find driver batch file
GOTO exit


:help
echo Usage 1:
echo update.bat /f [vebroot]
echo [vebroot] is the path where veb is installed. The command installs AptioVDebugger for VeB and registers necessary components
echo.

echo Usage 2:
echo update.bat /f /usb3 [vebroot]
echo [vebroot] is the path where veb is installed. The command installs AptioVDebugger for VeB and registers necessary components along with USB3Driver Installation
echo.

echo Usage 3:
echo update.bat /d [vebroot]
echo [vebroot] is the path where veb is installed. The command installs AptioVDebugger for VeB and registers necessary components, including the visual c++ redistributable
echo.

echo Usage 4:
echo update.bat /d /usb3 [vebroot]
echo [vebroot] is the path where veb is installed. The command installs AptioVDebugger for VeB and registers necessary components along with USB3Driver Installation and visual c++ redistributable
echo.

echo Usage 5:
echo update.bat /r 
echo register the necessary components required for AptioVDebugger host
echo.

echo Usage 6:
echo update.bat /u 
echo unregisters AptioVDebugger host components
echo.


:exit
@echo ON