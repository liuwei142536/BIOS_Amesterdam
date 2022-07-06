//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
#include <Library/AmiBufferValidationLib.h>
#include <Library/BaseMemoryLib.h>

/**
  This function check if the buffer is valid per processor architecture and not overlap with SMRAM.

  @param Buffer  The buffer start address to be checked.
  @param Length  The buffer length to be checked.

  @retval TRUE  This buffer is valid per processor architecture and not overlap with SMRAM.
  @retval FALSE This buffer is not valid per processor architecture or overlap with SMRAM.
**/
BOOLEAN EFIAPI SmmIsBufferOutsideSmmValid (IN EFI_PHYSICAL_ADDRESS Buffer, IN UINT64 Length){
    if ((UINTN)Buffer != Buffer) return FALSE; // Make sure buffer address can be represented by a pointer
    if ((UINTN)Length != Length) return FALSE; // Make sure buffer length can be represented by UINTN
    return !EFI_ERROR(AmiValidateMemoryBuffer((VOID*)(UINTN)Buffer, (UINTN)Length));
}

/**
  Copies a source buffer (non-SMRAM) to a destination buffer (SMRAM).

  This function copies a source buffer (non-SMRAM) to a destination buffer (SMRAM).
  It checks if source buffer is valid per processor architecture and not overlap with SMRAM.
  If the check passes, it copies memory and returns EFI_SUCCESS.
  If the check fails, it return EFI_SECURITY_VIOLATION.
  The implementation must be reentrant.

  @param  DestinationBuffer   The pointer to the destination buffer of the memory copy.
  @param  SourceBuffer        The pointer to the source buffer of the memory copy.
  @param  Length              The number of bytes to copy from SourceBuffer to DestinationBuffer.

  @retval EFI_SECURITY_VIOLATION The SourceBuffer is invalid per processor architecture or overlap with SMRAM.
  @retval EFI_SUCCESS            Memory is copied.

**/
EFI_STATUS EFIAPI SmmCopyMemToSmram(OUT VOID *DestinationBuffer, IN CONST VOID *SourceBuffer, IN UINTN Length){
    EFI_STATUS Status;

    Status = AmiValidateMemoryBuffer(SourceBuffer,Length);
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
    CopyMem (DestinationBuffer, SourceBuffer, Length);

    return EFI_SUCCESS;
}

/**
  Copies a source buffer (SMRAM) to a destination buffer (NON-SMRAM).

  This function copies a source buffer (non-SMRAM) to a destination buffer (SMRAM).
  It checks if destination buffer is valid per processor architecture and not overlap with SMRAM.
  If the check passes, it copies memory and returns EFI_SUCCESS.
  If the check fails, it returns EFI_SECURITY_VIOLATION.
  The implementation must be reentrant.

  @param  DestinationBuffer   The pointer to the destination buffer of the memory copy.
  @param  SourceBuffer        The pointer to the source buffer of the memory copy.
  @param  Length              The number of bytes to copy from SourceBuffer to DestinationBuffer.

  @retval EFI_SECURITY_VIOLATION The DesinationBuffer is invalid per processor architecture or overlap with SMRAM.
  @retval EFI_SUCCESS            Memory is copied.

**/
EFI_STATUS EFIAPI SmmCopyMemFromSmram(OUT VOID *DestinationBuffer, IN CONST VOID *SourceBuffer, IN UINTN Length){
    EFI_STATUS Status;

    Status = AmiValidateMemoryBuffer(DestinationBuffer,Length);
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
    CopyMem (DestinationBuffer, SourceBuffer, Length);

    return EFI_SUCCESS;
}

/**
  Copies a source buffer (NON-SMRAM) to a destination buffer (NON-SMRAM).

  This function copies a source buffer (non-SMRAM) to a destination buffer (SMRAM).
  It checks if source buffer and destination buffer are valid per processor architecture and not overlap with SMRAM.
  If the check passes, it copies memory and returns EFI_SUCCESS.
  If the check fails, it returns EFI_SECURITY_VIOLATION.
  The implementation must be reentrant, and it must handle the case where source buffer overlaps destination buffer.

  @param  DestinationBuffer   The pointer to the destination buffer of the memory copy.
  @param  SourceBuffer        The pointer to the source buffer of the memory copy.
  @param  Length              The number of bytes to copy from SourceBuffer to DestinationBuffer.

  @retval EFI_SECURITY_VIOLATION The DesinationBuffer is invalid per processor architecture or overlap with SMRAM.
  @retval EFI_SECURITY_VIOLATION The SourceBuffer is invalid per processor architecture or overlap with SMRAM.
  @retval EFI_SUCCESS            Memory is copied.

**/
EFI_STATUS EFIAPI SmmCopyMem(OUT VOID *DestinationBuffer, IN CONST VOID *SourceBuffer, IN UINTN Length){
    EFI_STATUS Status;

    Status = AmiValidateMemoryBuffer(SourceBuffer,Length);
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
    Status = AmiValidateMemoryBuffer(DestinationBuffer,Length);
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
    CopyMem (DestinationBuffer, SourceBuffer, Length);

    return EFI_SUCCESS;
}

/**
  Fills a target buffer (NON-SMRAM) with a byte value.

  This function fills a target buffer (non-SMRAM) with a byte value.
  It checks if target buffer is valid per processor architecture and not overlap with SMRAM.
  If the check passes, it fills memory and returns EFI_SUCCESS.
  If the check fails, it returns EFI_SECURITY_VIOLATION.

  @param  Buffer    The memory to set.
  @param  Length    The number of bytes to set.
  @param  Value     The value with which to fill Length bytes of Buffer.

  @retval EFI_SECURITY_VIOLATION The Buffer is invalid per processor architecture or overlap with SMRAM.
  @retval EFI_SUCCESS            Memory is set.

**/
EFI_STATUS EFIAPI SmmSetMem(OUT VOID *Buffer, IN UINTN Length, IN UINT8 Value){
    EFI_STATUS Status;

    Status = AmiValidateMemoryBuffer(Buffer,Length);
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
    SetMem (Buffer, Length, Value);

    return EFI_SUCCESS;
}

/**
  This function check if the MMIO resource is valid per processor architecture and
  valid per platform design.

  @param BaseAddress  The MMIO start address to be checked.
  @param Length       The MMIO length to be checked.
  @param Owner        A GUID representing the owner of the resource.
                      This GUID may be used by producer to correlate the device ownership of the resource.
                      NULL means no specific owner.

  @retval TRUE  This MMIO resource is valid per processor architecture and valid per platform design.
  @retval FALSE This MMIO resource is not valid per processor architecture or valid per platform design.
**/
BOOLEAN EFIAPI SmmIsMmioValid (
    IN EFI_PHYSICAL_ADDRESS  BaseAddress, IN UINT64 Length, IN EFI_GUID *Owner  OPTIONAL
){
    if ((UINTN)BaseAddress != BaseAddress) return FALSE; // Make sure base address can be represented by a pointer
    if ((UINTN)Length != Length) return FALSE; // Make sure buffer length can be represented by UINTN
    return !EFI_ERROR(AmiValidateMmioBuffer((VOID*)(UINTN)BaseAddress, (UINTN)Length));
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
