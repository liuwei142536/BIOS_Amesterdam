;------------------------------------------------------------------------------
;   Copyright (c) 2009 Nationz Technologies Inc. All rights reserved.
;   This program contains proprietary and confidential information. All
;   rights reserved except as may be permitted by prior written consent.
;
;   Product:    TCM DXE Driver v1.04 
;   Author:     Nosa
;
;         R E V I S I O N    H I S T O R Y
;
;------------------------------------------------------------------------------
;
; $Revision:    01.04.11.09 
; $Date:        10/29/2009
;
;******************************************************************************
 .686
 .model  flat
 .code
;------------------------------------------------------------------------------
; VOID
; EFIAPI
; IODelay (
;   VOID
;   );
;------------------------------------------------------------------------------
IODelay   PROC  PUBLIC
    out	  0edh, al
    
    ret
IODelay   ENDP

    END
