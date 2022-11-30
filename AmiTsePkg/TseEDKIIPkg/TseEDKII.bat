@echo off

:: ## Set project and build tools path ##

SET PROJECT_PATH=J:\Project\NT32
SET BUILDTOOLS_PATH=J:\SoftWares\Org\Veb_n_BuildTools\Build_Tools\Tools_31

if [%PROJECT_PATH%]==[] (
echo Project path is empty 
goto end
)

if [%BUILDTOOLS_PATH%]==[] (
echo Build tools Path is empty
goto end
)

if not exist "%PROJECT_PATH%\AmiTsePkg\" (
echo "Project path does not contain AmiTsePkg folder"
goto end
)

if not exist "%BUILDTOOLS_PATH%\AMISDL.jar" (
echo "Build tools path does not contain AMISDL.jar file"
goto end
)

if exist "%PROJECT_PATH%\Build" (
rmdir "%PROJECT_PATH%\Build" /s /q
echo Deleted existing build folder
) else (
echo no existing build folder found )

mkdir "%PROJECT_PATH%\Build"


:: Copy existing files

:: AmiTsePkg

xcopy %PROJECT_PATH%\AmiTsePkg\AmiTsePkg.dec %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\AmiTsePrivatePkg.dec %PROJECT_PATH%\Build\TseEDKII\AmiTsepkg\ /i /y /r

:: AmiTsePkg\Include

xcopy %PROJECT_PATH%\AmiTsePkg\Include\AMIVfr.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Include\Protocol\AMIPostMgr.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\Protocol\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Include\Protocol\EsaTseInterfaces.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\Protocol\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Include\Protocol\AMIScreenMgmt.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\Protocol\ /i /y /r


:: AmiTsePkg\Core
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\boot.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\HiiLib.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\HookAnchor.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\LogoLib.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\mem.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\PwdLib.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\setupdata.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\TseCommon.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\TseElinks.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Inc\variable.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\Inc\ /i /y /r


xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\AmiTSEStr.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\bootflow.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\bootflow.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\CommonHelper.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\commonoem.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\commonoem.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Faketokens.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\HookList.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Keymon.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Keymon.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\TseDrvHealth.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\setupdbg.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\Core\em\AMITSE\Uefisetup.ini %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r


:: AmiTsePkg\Minisetup

:: AmiTsePkg\Minisetup\BootOnly

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\AMILogo\AMILogo.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\AMILogo\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\AMILogo\AMILogo.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\AMILogo\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\PasswordEncode.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\PasswordEncode.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\TsePasswordEncode.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\PasswordEncode\ /i /y /r


xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\amiver.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\bbs.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\bbs.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\boot.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\box.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\box.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\buffer.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\buffer.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\dobmpmgr.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\EDKhelper.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\EDKhelper.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\globals.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\hiistring.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\hiistring.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\hiistring21.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\HookAnchor.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\logo.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\mem.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\minisetup.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\minisetup.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\minisetupext.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\notify.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\password.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\password.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\postmgmt.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\postmgmtext.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\print.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\protocol.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\protocol.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\Resources.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\screen.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\screen.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\string.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\string.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\timer.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\Timer.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\BootOnly\variable.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\BootOnly\ /i /y /r


:: AmiTsePkg\Minisetup\Ezport

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\ezport.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\ezport.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\HotclickBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\HotclickBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\HotkeyBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\HotkeyBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\style.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\style.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\stylecommon.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\StyleStr.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Ezport\StyleTokens.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Ezport\ /i /y /r


:: AmiTsePkg\Minisetup\EzportPlus

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\ezportplus.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\ezportplus.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\HotclickBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\HotclickBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\HotkeyBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\HotkeyBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\style.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\style.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\stylecommon.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\StyleStr.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\EzportPlus\StyleTokens.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\EzportPlus\ /i /y /r


:: AmiTsePkg\Minisetup\Ezport

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\legacy.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\legacy.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\HotclickBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\HotclickBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\HotkeyBin.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\HotkeyBin.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\style.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\style.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\stylecommon.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\StyleStr.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\Legacy\StyleTokens.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\Legacy\ /i /y /r


:: AmiTsePkg\Minisetup\TseAdvanced

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\dogif.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\dogifmgr.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ijpeg.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ipcx.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\jpeg.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\jpeg6.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\Jpeg6.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\Logo.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\MyDefs.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\pcx.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\pcxc.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\png.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\png.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\pnguncmp.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AdvancedImages\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AddDelBootDrvOpts.sd %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\AddDelBootDrvOpts.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\Bbs.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\bootflowLib.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\Mouse.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\special.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\special.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseAdvanced\TseAdvanced.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseAdvanced\ /i /y /r


:: AmiTsePkg\Minisetup\TseLite

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\StyleHook\StyleHook.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\StyleHook\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\minisetupext.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\application.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Action.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\action.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\application.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\object.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\object.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\page.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\page.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\control.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\control.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\callback.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\callback.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\hotkey.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\frame.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\frame.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\hotkey.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\SubMenu.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\SubMenu.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Text.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Text.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Menu.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Menu.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\MessageBox.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\MessageBox.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Memo.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Memo.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\popup.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\popup.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Label.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Label.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\numeric.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\numeric.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\time.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Time.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Date.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\Date.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\edit.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\edit.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ListBox.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ListBox.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupPassword.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupPassword.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupSel.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupSel.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupString.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupString.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupEdit.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\PopupEdit.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ordlistbox.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ordlistbox.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\minisetupext.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\variable.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\AddBootOption.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\tselitecommon.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\UefiAction.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\UefiAction.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ResetButton.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\ResetButton.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\hotclick.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\TseLite\hotclick.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\TseLite\ /i /y /r


:: AmiTsePkg\Minisetup\uefi2.1

xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\CtrlCond.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\CtrlCond.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\Expression.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\FormBrowser2.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\FormBrowser2.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\hii.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\HiiCallback.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\HiiNotificationHandler.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\Parse.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\TseUefiHii.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\uefi21wapper.c %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\EDK\MiniSetup\uefi2.1\UefiTianoHii.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\EDK\MiniSetup\uefi2.1\ /i /y /r

:: Clear existing TseEDKII.ldl file since it may contains project sdl list
if exist "%PROJECT_PATH%\TseEDKII.ldl" (del %PROJECT_PATH%\TseEDKII.ldl)

:: Copy project.ldl file to project folder which contains only AMITSE sdls list to be processed by AMISDL.jar
xcopy %PROJECT_PATH%\AmiTsePkg\TseEDKIIPkg\TseEDKII.ldl %PROJECT_PATH%\

:: Create Build time generated files
java -jar %BUILDTOOLS_PATH%\AMISDL.jar %PROJECT_PATH%\TseEDKII.ldl

:: Copy build time generated files

xcopy %PROJECT_PATH%\Build\AMITSEElinks.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r
xcopy %PROJECT_PATH%\Build\Token.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\tokens.h /i /y /r
xcopy %PROJECT_PATH%\Build\HotClickElinks.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r
xcopy %PROJECT_PATH%\Build\HotKeyElinks.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r



:: Copy (new)Edk dependent files

xcopy %PROJECT_PATH%\AmiTsePkg\TseEDKIIPkg\VFR.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r
xcopy %PROJECT_PATH%\AmiTsePkg\TseEDKIIPkg\Languages.uni %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Core\em\AMITSE\ /i /y /r

xcopy %PROJECT_PATH%\AmiTsePkg\TseEDKIIPkg\Tiano.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r

xcopy %PROJECT_PATH%\AmiTsePkg\TseEDKIIPkg\AMITSEForEdk2.mak  %PROJECT_PATH%\Build\ /i /y /r


:: genarte AMITSE.inf

set CUR_BAT_PATH=%cd%
set TOOLS_DIR=%BUILDTOOLS_PATH%

:: Back to project path, otherwise make.exe will fail due to missing of .veb file in current path
cd %PROJECT_PATH%
%BUILDTOOLS_PATH%\make.exe -f %PROJECT_PATH%\Build\AMITSEForEdk2.mak
cd %CUR_BAT_PATH%

xcopy %PROJECT_PATH%\Build\AMITSE.inf  %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\ /i /y /r
xcopy %PROJECT_PATH%\Build\AMITSEOem.h  %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include /i /y /r
xcopy %PROJECT_PATH%\Build\TimeStamp.h %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\ /i /y /r
if exist %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\AMITSE.inf echo Copied AMITSE.inf to the location "%PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\"
if exist %PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include\AMITSEOem.h echo Copied AMITSEOem.h to the location "%PROJECT_PATH%\Build\TseEDKII\AmiTsePkg\Include"


echo "Operation completed ..."

:end

