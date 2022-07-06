SMM module is no longer supported.
The content of the SMM module is part of Core starting from Core 5.12
If your project is based on Core 5.12 or newer, delete this component from your project using 
the procedure outlined below.
If your project is based on Core 5.11 or older, downgrade to SMM_08 or older.

SMM Module Deletion Procedure:
1. If your project is based on IntelArchitecture_04 or older, upgrade to IntelArchitecture_05 or newer.
2. If your project is an AARCH64 project that requires SMM support, create the following SDL token in
one of the project SDL files (if the token already exists outside of the SMM module, skip this step):
TOKEN
    Name  = "SMM_SUPPORT"
    Value  = "1"
    Help  = "Main switch to enable SMM support in Project"
    TokenType = Boolean
    TargetEQU = Yes
    TargetMAK = Yes
    TargetH = Yes
End
3. Delete SMM Module.

Details:
SMM MdePkg is integrated into MdePkg starting from MdePkg_10
SMM MdeModulePkg is integrated into MdeModulePkg starting from MdeModulePkg_14
AMI SMM Library Instances module part(including 'AmiSmmCorePlatformHookLib') is integrated into AmiModulePkg starting from AmiModulePkg_31
  NOTE: AmiSmmCorePlatformHookLib has been renamed to SmmCorePlatformHookLib.
SmmCore is integrated into Foundation starting from Foundation_08.1
SmmLockBox is added as a Core module part.

NOTE: AmiSmmCorePlatformHookLib and SmmCore source files have been copied to a different SVN location
corresponding to a location of the their new home component.
