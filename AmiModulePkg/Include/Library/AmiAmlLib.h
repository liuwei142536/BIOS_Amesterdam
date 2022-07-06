//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  AmiAmlLib library class definition. Collection of AML manipulation/generation routines.
*/

#ifndef __AMI_AML_LIB__H__
#define __AMI_AML_LIB__H__

#include <IndustryStandard/Acpi10.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/AmiItemListLib.h>

//Structure to store dependent function resource descriptor pointers
typedef struct _AMI_AML_DEP_FUNC {
    VOID         *DepFn;
    AMI_ITEM_LIST DepRes;
}AMI_AML_DEP_FUNC;

typedef enum {
    AmiAmlResourseBus,
    AmiAmlResourseIoMem,
    AmiAmlResourseAll,
    AmiAmlResourseMax
}AMI_AML_RESOURSE_TYPE;

//Type of object
typedef enum {
    AmiAmlObjTypeNone,
    AmiAmlObjTypeScope,
    AmiAmlObjTypeBuffer,
    AmiAmlObjTypeName,
    AmiAmlObjTypeProc,
    AmiAmlObjTypeTermal,
    AmiAmlObjTypeDevice,
    AmiAmlObjTypeMethod,
    AmiAmlObjTypePwrRes,
    AmiAmlObjTypeOpReg
} AMI_AML_OBJECT_TYPE;

//Structure to store AML Object Information
typedef struct _AMI_AML_OBJECT_INFO {
    VOID    *Object;
    VOID    *ObjName;
    VOID    *DataStart;
    UINT32  ObjType; // A value of AMI_AML_OBJECT_TYPE type
    UINTN   Length;
} AMI_AML_OBJECT_INFO;

//AML Package Definition
typedef struct _AMI_AML_PLEN_BYTE {
    UINT8       Length    : 6;
    UINT8       ByteCount : 2;
    //Package Length Following Bytes 0..3
} AMI_AML_PLEN_BYTE;

//--------------------------------------------------------------------------------------------
//Function Prototypes
//--------------------------------------------------------------------------------------------

/**
    Generates a short IRQ descriptor and returns its address.
    User is responsible for freeing the returned memory.

    @param NumberOfIrqs Number of IRQ numbers in the variable argument list.
    @param Active IRQ flag.  0 = active-high, 1 = active-low.
    @param Trigger IRQ flag.  0 = level-triggered, 1 = edge-triggered.
    @param Shareable IRQ flag.  0 = not interrupt shareable, 1 = interrupt shareable.
    @param ... List of IRQ numbers.  Valid values: 0-15.

    @return Address of the created IRQ descriptor.
            User is responsible for freeing the returned memory.
**/
EFI_ACPI_IRQ_DESCRIPTOR* EFIAPI AmiBuildIrqDesc(
    UINT8 NumberOfIrqs, UINT8 Active, UINT8 Trigger, UINT8 Shareable, ...
);

/**
    Generates a short IRQ descriptor without an optional IRQ information byte and returns its
    address.  User is responsible for freeing the returned memory.

    @param DscCnt Number of IRQ numbers in the list.
    @param ... List of IRQ numbers.  Valid values: 0-15.

    @retval  VOID* address of the created IRQ descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR* EFIAPI AmiBuildIrqDescNoFlags(UINT8 NumberOfIrqs, ... );

/**
    Generates a short DMA descriptor and returns its address.  User is responsible for freeing the returned memory.

    @param DscCnt Number of channel numbers in the list.
    @param Type DMA channel speed.  Valid values are: EFI_ACPI_DMA_SPEED_TYPE_COMPATIBILITY, EFI_ACPI_DMA_SPEED_TYPE_A,
            EFI_ACPI_DMA_SPEED_TYPE_B, EFI_ACPI_DMA_SPEED_TYPE_F
    @param Bm - Bus Master. Valid values are: EFI_ACPI_DMA_BUS_MASTER  if BusMaster or 0 if NotBusMaster.
    @param Size Transfer size.  Valid values are:  EFI_ACPI_DMA_TRANSFER_TYPE_8_BIT, EFI_ACPI_DMA_TRANSFER_TYPE_8_BIT_AND_16_BIT,
            EFI_ACPI_DMA_TRANSFER_TYPE_16_BIT
    @param ... List of channel numbers.  Valid values are 0-7.

    @retval  VOID* address of the created DMA descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_DMA_DESCRIPTOR* EFIAPI AmiBuildDmaDesc(UINT8 NumberOfChannels, UINT8 Type, UINT8 Bm, UINT8 Size, ... );

/**
     Generates a Start-Dependent Function descriptor with the optional priority byte and returns its address.  User is
     responsible for freeing the returned memory.

    @param DscCnt Number of items in the descriptor list.
    @param Comp Compatibility priority.  Valid values are 0-2.
    @param Perf Performance/robustness priority.  Valid values are 0-2.
    @param ... List of descriptors for this dependent function.

    @retval  VOID* address of the created Start-Dependent Function descriptor.  User is responsible for freeing the returned memory.

**/
AMI_AML_DEP_FUNC* EFIAPI AmiBuildStartDepFnDesc(UINT8 DscCnt,UINT8 Comp,UINT8 Perf, ...);

/**
     Generates a Start-Dependent Function descriptor without the optional priority byte and returns its address.
     User is responsible for freeing the returned memory.

    @param DscCnt Number of items in the descriptor list.
    @param ... List of descriptors for this dependent function.

    @retval VOID* address of the created Start-Dependent Function descriptor.  User is responsible for freeing the returned memory.

**/
AMI_AML_DEP_FUNC* EFIAPI AmiBuildStartDepFnDescNoPri(UINT8 DscCnt, ...);

/**
    Generates a short I/O descriptor and returns its address. User is responsible for freeing the returned memory.

    @param Dec Decode value.  Valid values are: ASLV_Decode16, ASLV_Decode10.
    @param Min Address minimum.
    @param Max Address maximum.
    @param Aln Base alignment.
    @param Len Range length.

    @retval  VOID* address of the created I/O descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_IO_PORT_DESCRIPTOR* EFIAPI AmiBuildIoDesc(UINT8 Dec,UINT16 Min,UINT16 Max,UINT8 Aln,UINT8 Len);

/**
    Generates a short fixed I/O descriptor and returns its address.  User is responible for freeing the returned memory.
    @param  Bas Address base.
    @param Len Range length.

    @retval VOID* address of the created fixed I/O descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR* EFIAPI AmiBuildFixedIoDesc(UINT16 Bas, UINT8 Len);

/**
    Generates a long 24-bit memory descriptor and returns its address.  User is responsible for freeing the returned memory.
    @param Rw  Write protection flag.  Valid values: EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_READ_ONLY,
            EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_READ_WRITE.
    @param Min Minimum base memory address [23:8].
    @param Max Maximum base memory address [23:8].
    @param Aln Base alignment.
    @param Len Range length.

    @retval  VOID* address of the created long 24-bit memory descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR* EFIAPI AmiBuildMemory24Desc(UINT8 Rw,UINT8 Min, UINT8 Max, UINT8 Aln, UINT8 Len);

/**
    Validates the consistency of the provided ACPI QWORD resource descriptors block which contains some
    number of EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTORs in memory terminated by an AMI_ASLR_END_TAG.  Returns the total size of the resource
    descriptors plus the size of the AMI_ASLR_END_TAG.
    If descriptor block is not valid, zero is returned.

    @param DscStart Pointer to the first descriptor in the block to be validated.
    @param ResType The resource type of the descriptors.  Valid values are:  AmiAmlResourseAll, AmiAmlResourseIoMem, AmiAmlResourseBus
    @param Fixed If TRUE, checks the general flags for consistency.

    @retval  UINTN size of the resource descriptor block plus the size of the AMI_ASLR_END_TAG.
    @retval  0 the descriptor block is invalid
**/
UINTN EFIAPI AmiGetAcpiDescriptorBlockLength(EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR* DscStart, AMI_AML_RESOURSE_TYPE ResType, BOOLEAN Fixed);

/**
    Determines the length in bytes of an AML package.

    @param Pck  AML package to be measured.
    @param BytesCnt The ByteCount value of the package.

    @retval  UINTN size of the AML package in bytes.

**/
UINTN EFIAPI AmiGetAmlPackageLength(AMI_AML_PLEN_BYTE *Pck, UINT8 *BytesCnt);

/**
    Finds an ASL object by its name string and returns its location.

    @param Start Location to begin scanning for the ASL object.
    @param ObjName Name of the ASL object to be located.
    @param Len Maximum length of bytes to scan for the ASL object.

    @retval pointer to the start of the ASL name string in memory.

**/
VOID* EFIAPI AmiFindAmlObjectByName(UINT8 *Start, CHAR8 *ObjName, UINTN Len);

/**
    Destroys Method passed as MethodName parameter if it is present in ASL object
    described by AslObj parameter

    @param AslObj  Pointer to AMI_AML_OBJECT_INFO structure, which discribes ASL object to modify
    @param MethodName Name of the ASL Method to be destroyed.

**/
EFI_STATUS EFIAPI AmiHideAmlMethodFromOs (AMI_AML_OBJECT_INFO *AslObj, CHAR8 *MethodName);

/**
    Getting Asl Obj

    @param Start Starting location in memory to begin scanning for the desired ASL object.
    @param Length Maximum number of bytes in memory to be scanned starting from Start for the ASL object.
    @param Name Pointer to the name of the ASL object to be located.
    @param ObjType Type of the ASL object to be located.  Supports: otScope, otName, otProc, otTermal, otDevice, otMethod, otPwrRes, otOpReg
    @param ObjInfo Returns completed AMI_AML_OBJECT_INFO structure, if the ASL object is found.

    @retval EFI_NOT_FOUND,  if object not found or invalid type.
        Otherwise, EFI_SUCCESS.
**/
EFI_STATUS EFIAPI AmiGetAmlObject(UINT8 *Start, UINTN Length, CHAR8 *Name, AMI_AML_OBJECT_TYPE ObjType, AMI_AML_OBJECT_INFO *ObjInfo);

/**
    Locates an ASL object by its name, ObjName in the DSDT or SSDT and updates its data with the provided Value.

     @param Sdt Pointer to the DSDT or SSDT.
     @param ObjName Name of the ASL object to be updated.
     @param Value New value for the ASL object's data entry.

     @retval EFI_INVALID_PARAMETER,  if the DSDT pointer, ObjName or Value are invalid.
        Otherwise, EFI_SUCCESS.
**/
EFI_STATUS EFIAPI AmiUpdateAmlObjectValue(EFI_ACPI_SDT_HEADER *Sdt, CHAR8 *ObjName, UINT64 Value);

/**
 Retrieves a pointer to the DSDT ACPI table for the user requested Version of ACPI.


 @param  DsdtAddr On output address of the DSDT associated with the user provided Version.
 @param Version Which version of ACPI's DSDT to return.  Valid values include:



  @retval EFI_NOT_AVAILABLE_YET,  if ACPI support protocol not installed yet.
  Otherwise based on result.

**/
EFI_STATUS EFIAPI AmiGetDsdt(EFI_ACPI_SDT_HEADER **Dsdt, EFI_ACPI_TABLE_VERSION Version);

/**
     Locates an ASL object by its name, ObjName within the scope of device DeviceName and updates its data with the provided Value.

     @param Sdt Pointer to the DSDT or SSDT.
     @param DeviceName Name of the ASL Device object having in it's scope Name Object to be updated.
     @param ObjName Name of the ASL Name Object to be updated.
     @param Value New value for the ASL object's data entry.

     @retval EFI_INVALID_PARAMETER,  if the DSDT pointer, ObjName or Value are invalid.
        EFI_NOT_FOUND,  if object specified by Device Name Or ObjName not present.
        Otherwise, EFI_SUCCESS.
**/
EFI_STATUS EFIAPI AmiUpdateAmlObjectValueInDeviceScope(
    EFI_ACPI_SDT_HEADER *Sdt, CHAR8 *DeviceName, CHAR8 *ObjName, UINT64 Value
);

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
