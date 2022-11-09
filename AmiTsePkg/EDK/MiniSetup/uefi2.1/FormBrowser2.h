//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//
//*****************************************************************//
/** @file FormBrowser2.h

**/
//*************************************************************************

//----------------------------------------------------------------------------
#ifndef _FORM_BROWSER2_H
#define _FORM_BROWSER2_H
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#include "minisetup.h"
#include <Protocol/FormBrowser2.h>
//----------------------------------------------------------------------------
VOID SetCallBackControlInfo(EFI_HANDLE VarHandle, UINT32 VarIndex);
EFI_STATUS  InstallFormBrowserProtocol(EFI_HANDLE Handle);
VOID UnInstallFormBrowserProtocol(EFI_HANDLE Handle);

EFI_STATUS
EFIAPI
SendForm (
  IN  CONST EFI_FORM_BROWSER2_PROTOCOL *This,
  IN  EFI_HII_HANDLE                   *Handles,
  IN  UINTN                            HandleCount,
  IN  EFI_GUID                         *FormSetGuid, OPTIONAL
  IN  UINT16                           FormId, OPTIONAL
  IN  CONST EFI_SCREEN_DESCRIPTOR      *ScreenDimensions, OPTIONAL
  OUT EFI_BROWSER_ACTION_REQUEST       *ActionRequest  OPTIONAL
  );

EFI_STATUS
EFIAPI
BrowserCallback (
  IN CONST EFI_FORM_BROWSER2_PROTOCOL  *This,
  IN OUT UINTN                         *ResultsDataSize,
  IN OUT EFI_STRING                    ResultsData,
  IN BOOLEAN                           RetrieveData,
  IN CONST EFI_GUID                    *VariableGuid, OPTIONAL
  IN CONST CHAR16                      *VariableName  OPTIONAL
  );

EFI_STATUS FormBrowserLocateSetupHandles(VOID*	**handleBuffer,UINT16 *count);
BOOLEAN FormBrowserHandleValid(VOID);

extern UINT32 AllocatedFirstPageSize;
extern UINT32 FirstPageOffset;
extern UINT32 ControlListSize;
extern UINT32 ControlListOffset;
extern UINT32 PageIdListSize;
extern UINT32 PageIdListOffset;
extern UINT32 PageIdInfoSize;
extern UINT32 PageIdInfoOffset;
extern UINT32 PageListSize;
extern UINT32 PageListOffset;
extern UINT32 PageInfoSize;
extern UINT32 PageInfoOffset;
extern UINT32 VariableListSize;
extern UINT32 VariableListOffset;
extern UINT32 VariableInfoSize;
extern UINT32 VariableInfoOffset;
extern EFI_GUID *gGuidDump;
extern UINTN  gGuidDumpCount;
extern UINTN gDynamicPageCount;
extern UINTN gDynamicPageGroupCount;
extern UINT16 gCurrDynamicPageGroup;
extern UINT16 gCurrDynamicPageGroupClass;
extern UINT32 gRefreshIdCount;
extern UINTN TotalRootPages;
extern CHAR8 *GlyphWidthCache;
extern EFI_IFR_REF *gRefData;
extern BOOLEAN gEnableProcessPack;
extern UINTN gFakeVariableCount;


extern EFI_STATUS RTIfrRegFormNotificationWrapper (VOID);
extern VOID RTIfrUnRegFormNotificationWrapper (VOID);
extern UINT16 _HiiGetLinkIndex(EFI_GUID *guid, UINT16 class, UINT16 subclass, UINT16 formid);
extern EFI_STATUS CreateEventforIFR (CONTROL_INFO *control_Info );
extern VOID CheckRefreshIDForSFPage(PAGE_INFO *PageInfo,UINT16 Link);
extern EFI_STATUS	InitApplicationData(EFI_HANDLE ImageHandle);
extern VOID InitGlobalPointers( VOID );
extern UINT8 TseGetResetType();
extern EFI_STATUS SaveTseLiteGlobalData(VOID *temp);
extern VOID RestoreTseLiteGlobalData(VOID *temp);
extern EFI_STATUS SaveTseAdvancedGlobalData(VOID *temp);
extern VOID RestoreTseAdvancedGlobalData(VOID *temp);
extern BOOLEAN IsMouseSupported(VOID);
extern BOOLEAN IsSoftKbdSupported(VOID);
extern EFI_STATUS ProcessPackNotificationHook (void);

#endif /* #define _FORM_BROWSER2_H */