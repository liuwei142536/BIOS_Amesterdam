/** @file
  This file defines the EFI SPI Protocol which implements the
  Intel(R) PCH SPI Host Controller Compatibility Interface.

@copyright
  Copyright (c) 2006 - 2013 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _EFI_SPI_H_
#define _EFI_SPI_H_


///
/// Extern the GUID for protocol users.
///
extern EFI_GUID                   gEfiSpiProtocolGuid;
extern EFI_GUID                   gEfiSmmSpiProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_SPI_PROTOCOL  EFI_SPI_PROTOCOL;

//
// SPI protocol data structures and definitions
//
///
/// Number of Prefix Opcodes allowed on the SPI interface
///
#define SPI_NUM_PREFIX_OPCODE 2

///
/// Number of Opcodes in the Opcode Menu
///
#define SPI_NUM_OPCODE  8


//
// SPI default opcode slots
//
#define SPI_OPCODE_JEDEC_ID_INDEX        0

///
/// Opcode Type
///    EnumSpiOpcodeCommand: Command without address
///    EnumSpiOpcodeRead: Read with address
///    EnumSpiOpcodeWrite: Write with address
///
typedef enum {
  EnumSpiOpcodeReadNoAddr,
  EnumSpiOpcodeWriteNoAddr,
  EnumSpiOpcodeRead,
  EnumSpiOpcodeWrite,
  EnumSpiOpcodeMax
} SPI_OPCODE_TYPE;

typedef enum {
  EnumSpiCycle20MHz,
  EnumSpiCycle33MHz,
  EnumSpiCycle66MHz,  ///< not supported by PCH
  EnumSpiCycle50MHz,
  EnumSpiCycleMax
} SPI_CYCLE_FREQUENCY;

typedef enum {
  EnumSpiRegionAll,
  EnumSpiRegionBios,
  EnumSpiRegionMe,
  EnumSpiRegionGbE,
  EnumSpiRegionDescriptor,
  EnumSpiRegionPlatformData,
  EnumSpiRegionDer,
  EnumSpiRegionMax
} SPI_REGION_TYPE;

///
/// Hardware Sequencing required operations (as listed in PCH EDS "Hardware
/// Sequencing Commands and Opcode Requirements"
///
typedef enum {
  EnumSpiOperationWriteStatus,
  EnumSpiOperationProgramData_1_Byte,
  EnumSpiOperationProgramData_64_Byte,
  EnumSpiOperationReadData,
  EnumSpiOperationWriteDisable,
  EnumSpiOperationReadStatus,
  EnumSpiOperationWriteEnable,
  EnumSpiOperationFastRead,
  EnumSpiOperationEnableWriteStatus,
  EnumSpiOperationErase_256_Byte,
  EnumSpiOperationErase_4K_Byte,
  EnumSpiOperationErase_8K_Byte,
  EnumSpiOperationErase_64K_Byte,
  EnumSpiOperationFullChipErase,
  EnumSpiOperationJedecId,
  EnumSpiOperationDualOutputFastRead,
  EnumSpiOperationDiscoveryParameters,
  EnumSpiOperationOther,
  EnumSpiOperationWriteStatusReg2,
  EnumSpiOperationReadStatusReg2,
  EnumSpiOperationMax
} SPI_OPERATION;

///
/// SPI Command Configuration
///
typedef struct _SPI_COMMAND_CONFIG {
  ///
  /// The expected frequency to be used (value to be programmed to the SSFC Register)
  ///
  SPI_CYCLE_FREQUENCY Frequency;
  ///
  /// Which Hardware Sequencing required operation this opcode respoinds to.
  /// The required operations are listed in EDS Table 5-55: "Hardware
  /// Sequencing Commands and Opcode Requirements"
  /// If the opcode does not corresponds to any operation listed, use
  /// EnumSpiOperationOther, and provides TYPE and Code for it in
  /// SpecialOpcodeEntry.
  ///
  SPI_OPERATION Operation;
} SPI_COMMAND_CONFIG;

///
/// Special Opcode entries
///
typedef struct _SPI_SPECIAL_OPCODE_ENTRY {
  ///
  /// Opcode Menu Index whose Opcode Type/Menu Configuration Register need to be
  /// overrided or programmed per "Type" and "Code". Filled this field with 0xFF
  /// as the end tag of SpecialOpcodeEntry.
  ///
  UINT8 OpcodeIndex;
  ///
  /// Operation Type (value to be programmed to the OPTYPE register)
  ///
  SPI_OPCODE_TYPE Type;
  ///
  /// The opcode (value to be programmed to the OPMENU register)
  ///
  UINT8 Code;
} SPI_SPECIAL_OPCODE_ENTRY;

///
/// Initialization data table loaded to the SPI host controller
///
/// Note:  Most of time, the SPI flash parts with the same vendor would have the same
///        Prefix Opcode, Opcode menu, so you can provide one table for the SPI flash parts with
///        the same vendor.
///
typedef struct _SPI_INIT_DATA {
  ///
  /// Prefix opcodes which are loaded into the SPI host controller
  ///
  UINT8 PrefixOpcode[SPI_NUM_PREFIX_OPCODE];
  ///
  /// Determines Opcode Type, Menu and Frequency of the SPI commands
  ///
  SPI_COMMAND_CONFIG SpiCmdConfig[SPI_NUM_OPCODE];
  ///
  /// Special Opcode entry for the special operations.
  ///
  SPI_SPECIAL_OPCODE_ENTRY  *SpecialOpcodeEntry;
  ///
  /// The offset of the start of the BIOS image relative to the flash device.
  /// Please note this is a Flash Linear Address, NOT a memory space address.
  /// This value is platform specific and depends on the system flash map.
  /// This value is only used on non Descriptor mode.
  ///
  UINTN BiosStartOffset;
  ///
  /// The the BIOS Image size in flash. This value is platform specific
  /// and depends on the system flash map. Please note BIOS Image size may
  /// be smaller than BIOS Region size (in Descriptor Mode) or the flash size
  /// (in Non Descriptor Mode), and in this case, BIOS Image is supposed to be
  /// placed at the top end of the BIOS Region (in Descriptor Mode) or the flash
  /// (in Non Descriptor Mode)
  ///
  UINTN BiosSize;
} SPI_INIT_DATA;


///
/// Initialization data table loaded to the SPI host controller
///    PrefixOpcode    Prefix opcodes which are loaded into the SPI host controller
///    SpiCmdConfig    Determines Opcode Type, Menu and Frequency of the SPI commands
///    BiosStartOffset The offset of the start of the BIOS image relative to the flash device.
///                    Please note this is a Flash Linear Address, NOT a memory space address.
///                    This value is platform specific and depends on the system flash map.
///                    This value is only used on non Descriptor mode.
///    BiosSize        The the BIOS Image size in flash. This value is platform specific
///                    and depends on the system flash map. Please note BIOS Image size may
///                    be smaller than BIOS Region size (in Descriptor Mode) or the flash size
///                    (in Non Descriptor Mode), and in this case, BIOS Image is supposed to be
///                    placed at the top end of the BIOS Region (in Descriptor Mode) or the flash
///                    (in Non Descriptor Mode)
///
typedef struct _SPI_INIT_TABLE {
  UINT8                 PrefixOpcode[SPI_NUM_PREFIX_OPCODE];
  SPI_COMMAND_CONFIG  SpiCmdConfig[SPI_NUM_OPCODE];
  UINTN                 BiosStartOffset;
  UINTN                 BiosSize;
} SPI_INIT_TABLE;

typedef struct _SPI_ID_TABLE {
  UINT8       VendorId;
  UINT8       DeviceId0;
  UINT8       DeviceId1;
} SPI_ID_TABLE;

typedef struct _SPI_DESCRIPTOR_TABLE {
  UINT8       NumberComponents    :2;
  UINT8       Comp1Density        :4;
  UINT8       Rsvd                :2;
} SPI_DESCRIPTOR_TABLE;



//
// Protocol member functions
//

/**
  JEDEC Read IDs from SPI flash part, this function will return 1-byte Vendor ID and 2-byte Device ID

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] Address              This value is to determine the command is sent to SPI Component 1 or 2
  @param[in, out] Buffer          Pointer to caller-allocated buffer containing the data received or sent during the SPI cycle.

  @retval EFI_SUCCESS             Read Jedec Id completed.
  @retval EFI_DEVICE_ERROR        Device error, operation failed.
  @exception EFI_UNSUPPORTED      This function is unsupported after SpiProtocolInit is called
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPI_READ_ID) (
  IN EFI_SPI_PROTOCOL     * This,
  IN     UINTN            Address,
  IN OUT UINT8            * Buffer
  );

/**
  Initializes the host controller to execute SPI commands.

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] InitData             Pointer to caller-allocated buffer containing the SPI
                                  interface initialization table.

  @retval EFI_SUCCESS             Opcode initialization on the SPI host controller completed.
  @retval EFI_ACCESS_DENIED       The SPI configuration interface is locked.
  @retval EFI_OUT_OF_RESOURCES    Not enough resource available to initialize the device.
  @retval EFI_DEVICE_ERROR        Device error, operation failed.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPI_INIT) (
  IN EFI_SPI_PROTOCOL     * This,
  IN SPI_INIT_DATA        * InitData
  );

/**
  Execute SPI commands from the host controller.

  @param[in] This                 Pointer to the EFI_SPI_PROTOCOL instance.
  @param[in] OpcodeIndex          Index of the command in the OpCode Menu.
  @param[in] PrefixOpcodeIndex    Index of the first command to run when in an atomic cycle sequence.
  @param[in] DataCycle            TRUE if the SPI cycle contains data
  @param[in] Atomic               TRUE if the SPI cycle is atomic and interleave cycles are not allowed.
  @param[in] ShiftOut             If DataByteCount is not zero, TRUE to shift data out and FALSE to shift data in.
  @param[in] Address              In Descriptor Mode, for Descriptor Region, GbE Region, ME Region and Platform
                                  Region, this value specifies the offset from the Region Base; for BIOS Region,
                                  this value specifies the offset from the start of the BIOS Image. In Non
                                  Descriptor Mode, this value specifies the offset from the start of the BIOS Image.
                                  Please note BIOS Image size may be smaller than BIOS Region size (in Descriptor
                                  Mode) or the flash size (in Non Descriptor Mode), and in this case, BIOS Image is
                                  supposed to be placed at the top end of the BIOS Region (in Descriptor Mode) or
                                  the flash (in Non Descriptor Mode)
  @param[in] DataByteCount        Number of bytes in the data portion of the SPI cycle.
  @param[in, out] Buffer          Pointer to caller-allocated buffer containing the dada received or sent during the SPI cycle.
  @param[in] SpiRegionType        SPI Region type. Values EnumSpiRegionBios, EnumSpiRegionGbE, EnumSpiRegionMe,
                                  EnumSpiRegionDescriptor, and EnumSpiRegionPlatformData are only applicable in
                                  Descriptor mode. Value EnumSpiRegionAll is applicable to both Descriptor Mode
                                  and Non Descriptor Mode, which indicates "SpiRegionOffset" is actually relative
                                  to base of the 1st flash device (i.e., it is a Flash Linear Address).

  @retval EFI_SUCCESS             Command succeed.
  @retval EFI_INVALID_PARAMETER   The parameters specified are not valid.
  @exception EFI_UNSUPPORTED      Command not supported.
  @retval EFI_DEVICE_ERROR        Device error, command aborts abnormally.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPI_EXECUTE) (
  IN     EFI_SPI_PROTOCOL   * This,
  IN     UINT8              OpcodeIndex,
  IN     UINT8              PrefixOpcodeIndex,
  IN     BOOLEAN            DataCycle,
  IN     BOOLEAN            Atomic,
  IN     BOOLEAN            ShiftOut,
  IN     UINTN              Address,
  IN     UINT32             DataByteCount,
  IN OUT UINT8              *Buffer,
  IN     SPI_REGION_TYPE    SpiRegionType
  );

///
/// EFI SPI Protocol definition
///
/// These protocols/PPI allows a platform module to perform SPI operations through the
/// Intel PCH SPI Host Controller Interface.
///
struct _EFI_SPI_PROTOCOL {
  EFI_SPI_READ_ID ReadId;   ///< JEDEC Read IDs from SPI flash part, this function will return 1-byte Vendor ID and 2-byte Device ID.
  EFI_SPI_INIT    Init;     ///< Initialize the host controller to execute SPI commands.
  EFI_SPI_EXECUTE Execute;  ///< Execute SPI commands from the host controller.
};

#endif
