;------------------------------------------------------------------------------
;   Copyright (c) 2009 Nationz Technologies Inc. All rights reserved.
;   This program contains proprietary and confidential information. All
;   rights reserved except as may be permitted by prior written consent.
;
;   Product:    TCM PEI Driver v1.4 
;   Author:     Nosa
;
;         R E V I S I O N    H I S T O R Y
;
;------------------------------------------------------------------------------
;
; $Revision:    01.04.11.09 
; $Date:        11/12/2009
;
;******************************************************************************
 .model  small
 .data

DRV_VER_INFO 	db " v1.4 Copyright 2009, Nationz Limited "
;-align 16
DRV_DETAIL 	db "NATIONZTCMDRV-PEI-X64"
Vendor_Info	db " VID"
VID		dw 1B4Eh
DID		dw 0100h

	END  

