
/*
#include "Efi.h"
#include "Pei.h"
#include "SpiAccPpi.h"
#include "SpsPei.h"
#include "token.h"
*/

EFI_STATUS
MEDisable (
  EFI_PEI_SERVICES      **PeiServices,
  EFI_PEI_SPI_ACC_PPI   *SpiAccPpi,
  UINT8                 DisableME
  )
{

  UINT32          CurrentStrapValue;
  BOOLEAN         ResetRequired;
  PEI_CPU_IO_PPI  *CpuIo;

  ResetRequired = FALSE;
  CpuIo         = (**PeiServices).CpuIo;

  SpiAccPpi->SoftStrapRead (
              PeiServices,
              SpiAccPpi,
              0,
              PCHSTRP10,
              &CurrentStrapValue
              );

  if (DisableME) {
    //
    // Hide all ME PCI Devices: HECI1, HECI2, IDER and KT
    //
    (*(volatile UINT32 *) (UINTN) (SB_RCBA + R_PCH_RCRB_FUNC_DIS2) |= (0x1e));

    if ((CurrentStrapValue & 0x1) == 0) {
      //
      // ME To be Disabled...
      //
      CurrentStrapValue |= 1;
      ResetRequired = TRUE;
      SpiAccPpi->SoftStrapWrite (PeiServices, SpiAccPpi, 0, PCHSTRP10, &CurrentStrapValue);
    }
  } else {
    if ((CurrentStrapValue & 0x1) == 1) {
      CurrentStrapValue &= ~((UINT32) 0x1);
      ResetRequired = TRUE;
      SpiAccPpi->SoftStrapWrite (PeiServices, SpiAccPpi, 0, PCHSTRP10, &CurrentStrapValue);
    }
  }

  if (ResetRequired == TRUE) {
    UINT8 Data8;
    Data8 = 0;
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] Performing global reset for Me To be DISABLE \n"));
    Data8 = COLD_RESET | FULL_RESET;
    CpuIo->IoWrite8 (PeiServices, CpuIo, RESET_PORT, Data8);
    Data8 |= RST_CPU;
    CpuIo->IoWrite8 (PeiServices, CpuIo, RESET_PORT, Data8);
  }

  return EFI_SUCCESS;

}
//
// =============================================================================
// 	Access Soft Straps.
// =============================================================================
// ============================================================================
// Procedure:	SoftStrapWrite
//
// Description:	This function is called to do multiple writes to the I/O space
// 		destinated to target SPI flash memory in descriptor region that saves
// 		Soft Strap information
//
// Input:
// 		**PeiServices - pointer to the Pei Services function and data structure
// 		*This - Pointer to the instance of the SPI I/O PPI structure
// 		Width - always set to 0
// 		Address - A 64 bit address that points to the PCH STRAP to be write
//
// Output:	always returns EFI_SUCCESS
// 		*Buffer - Data to be Write from the I/O space
//
// Referrals:
//
// ============================================================================
//
EFI_STATUS
SoftStrapWrite (
  IN    EFI_PEI_SERVICES                **PeiServices,
  IN    struct _EFI_PEI_SPI_ACC_PPI     *This,
  IN    EFI_PEI_SPI_CFG_PPI_WIDTH       Width,
  IN    UINT64                          Address,
  IN OUT  VOID                          *Buffer
  )
{
  UINT32      BlockSize;
  UINT8       *BackUpBlock;
  UINT8       *BackUpBlockStart;
  UINT32      Index;
  UINT32      Flmap1;
  UINT32      FlashAddr;
  UINT32      DescSize;
  UINT32      Data32;
  UINT16      Data16;
  UINT16      PchStrapBaseAddr;
  UINT8       CmdCycle;
  UINT8       PchStrapLength;
  EFI_STATUS  Status;

  //
  // ====================================================================
  // 	Check SPI Controller avialability
  // ====================================================================
  //
  // If RCBA has not been properly set SPI controll registers will return
  // 0xF's function will return with unsuported request
  //
  // Check if SPI controller is available
  //
  PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION]========== CHECKING SOFT STRAP ========== \n"));

  Status = SpiAccessCheck ();

  if (Status != EFI_SUCCESS) {
    //
    // SPI controller was not available signal error
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: SPI Controller not available \n"));
    return Status;
  } else {
    //
    // SPI controller was is available
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] SPI Controller is available \n"));
  }
  //
  // ====================================================================
  // 	Check if we are working in Descriptor Mode
  // ====================================================================
  //
  // SPI can work in descriptor or non descriptor mode this is establish
  // by reading a valid descriptor signature 0x0FF0A55A
  //
  Status = DescriptorCheck ();

  if (Status != EFI_SUCCESS) {
    //
    // Valid signature not found we are not in descriptor mode
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: VALID SIGNATURE NOT FOUND.\n"));
    return Status;
  } else {
    //
    // Valid signature found
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] VALID SIGNATURE FOUND.\n"));
  }
  //
  // ====================================================================
  // 	Start Write Process
  // ====================================================================
  //
  Data16 = *(volatile UINT16 *) (UINT32) (SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS);

  //
  // Get Block Size from HSFSTS
  //    HSFSTS offset 0x4: Hardware Sequencing Flash Status
  //    bits [4:3] Block/Sector Erase Size
  //    valid bit settings:
  //          00: 256 Byte
  //          01: 4K  Byte
  //          10: 8K  Byte
  //          11: 64K Byte
  //
  BlockSize = (Data16 & 0x1F) >> 3;
  //
  // get bits 4:3 from HSFSTS
  //
  switch (BlockSize) {
  case 0:
    BlockSize = BLOCK_SIZE_256B;
    //
    // ~256 Bytes
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] BLOCK SIZE 256B \n"));
    break;

  case 1:
    BlockSize = BLOCK_SIZE_4KB;
    //
    // ~4KB
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] BLOCK SIZE  4KB \n"));
    break;

  case 2:
    BlockSize = BLOCK_SIZE_8KB;
    //
    // ~8KB
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] BLOCK SIZE  8KB \n"));
    break;

  case 3:
    BlockSize = BLOCK_SIZE_64KB;
    //
    // ~64KB
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] BLOCK SIZE 64KB \n"));
    break;

  default:
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR] Invalid BLOCK SIZE \n"));
    return EFI_UNSUPPORTED;
  }
  //
  // end switch
  //
  // SPI Write operation requires a block erase since even
  // for a single bit the erase operation will clear an entire block
  // so before we attempt a write we must find a way to back up the
  // block that it will be erased.
  //
  DescSize = DESCRIPT_SIZE;
  //
  // This function will create a buffer of block size to back up data
  //
  Status = (*PeiServices)->AllocatePool (
                            PeiServices,
                            DescSize,
                            &BackUpBlock
                            );
  if (Status != EFI_SUCCESS) {
    //
    // unable to create buffer of size BlockSize
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: UNABLE to allocate buffer of size=%x \n", BlockSize));
    return Status;
  } else {
    CmdCycle  = B_SB_SPI_READ_CYCLE;
    FlashAddr = DESCRIP_FLMAP1;

    SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &Flmap1);

    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] FLASH MAP1 Off. 08h =%x.\n", Flmap1));

    //
    // Flash Map 1 offset 0x18 in Descriptor
    // ICH Strap lenght bits [31:24]
    //     identifies the 1's based number of Dwords of ICH Straps
    //     to be read up to 255 DWs 1KB max. if 0 means no ICH straps
    //
    //get bits 31:24 from FLMAP1
    //
    PchStrapLength = (UINT8) (Flmap1 >> 24);

    //
    // Flash Map 1 offset 0x18 in Descriptor
    // ICH Strap Base Addr bits [23:16]
    //     this identifes the address bits [11:4] for the ICH strap
    //     portion of the Flash Dexcriptor. bits [24:12] and bits
    //     [3:0] are 0
    //
    PchStrapBaseAddr = (UINT8) ((Flmap1 & 0x00FF0000) >> 16);
    //
    // get bits 24:12 from FLMAP1
    //
    PchStrapBaseAddr = PchStrapBaseAddr << 4;
    //
    // shift address in to position 11:0
    //
    // Check validity of Base Addres and length
    //
    if ((PchStrapLength == 0) || (PchStrapBaseAddr == 0)) {
      PEI_TRACE_EX (
        (
        TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: INVALID ADDRESS=%x OR SIZE =%x.\n", PchStrapBaseAddr,
        PchStrapLength
        )
        );
      return EFI_UNSUPPORTED;
    } else {

      BackUpBlockStart = BackUpBlock;

      for (Index = 0; Index < DescSize; Index += 4) {
        CmdCycle  = B_SB_SPI_READ_CYCLE;
        FlashAddr = Index;

        SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &Data32);
        *(UINT32 *) BackUpBlock = Data32;
        //
        //            PEI_TRACE_EX((TRACE_ALWAYS,PeiServices,"%x: at 0x%08x = 0x%08x\n",Index, BackUpBlock, *(UINT32*)BackUpBlock));
        //
        BackUpBlock += 4;
      }

      BackUpBlock = BackUpBlockStart;

      //
      // Start reading Soft Straps
      //      Calculate softstrap address in SPI flash
      //
      FlashAddr = PchStrapBaseAddr + (UINT32) Address;

      PEI_TRACE_EX (
        (
        TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] Original strap info 0x%08x\n", *(UINT32 *)
        (BackUpBlock + FlashAddr)
        )
        );

      *(UINT32 *) (BackUpBlock + FlashAddr) = *(UINT32 *) Buffer;

      PEI_TRACE_EX (
        (
        TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] New strap info 0x%08x at 0x%08x \n", *(UINT32 *)
        (BackUpBlock + FlashAddr), (BackUpBlock + FlashAddr)
        )
        );

      SpiBlockErase (0x0);

      for (Index = 0; Index < DescSize; Index += 4) {
        CmdCycle  = B_SB_SPI_WRITE_CYCLE;
        FlashAddr = Index;

        Data32    = *(UINT32 *) BackUpBlock;
        //
        //             PEI_TRACE_EX((TRACE_ALWAYS,PeiServices,"[SPS_INTERACTION] %x: at 0x%08x = 0x%08x\n",Index, BackUpBlock, *(UINT32*)BackUpBlock));
        //
        SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &Data32);
        BackUpBlock += 4;
      }

      PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] SPI SOFT STRAP COMPLETE \n"));

    }
    //
    // end valid base and size
    //
  }
  //
  // end else allocate buffer
  //
  return Status;
}
//
// ============================================================================
// Procedure:	SoftStrapRead
//
// Description:	This function is called to do multiple reads from the I/O space
// 		destinated to target SPI flash memory in descriptor region that saves
// 		Soft Strap information
//
// Input:
// 		**PeiServices - pointer to the Pei Services function and data structure
// 		*This - Pointer to the instance of the SPI I/O PPI structure
// 		Width - always set to 0
// 		Address - A 64 bit address that points to the PCH STRAP to be read from
//
// Output:	always returns EFI_SUCCESS
// 		*Buffer - Data to be read from the I/O space
//
// Referrals:
//
// ============================================================================
//
EFI_STATUS
SoftStrapRead (
  IN    EFI_PEI_SERVICES                **PeiServices,
  IN    struct _EFI_PEI_SPI_ACC_PPI     *This,
  IN    EFI_PEI_SPI_CFG_PPI_WIDTH       Width,
  IN    UINT64                          Address,
  IN OUT  VOID                          *Buffer
  )
{
  UINT32      Flmap1;
  UINT32      PchStrp;
  UINT32      FlashAddr;
  UINT16      PchStrapBaseAddr;
  UINT8       CmdCycle;
  UINT8       PchStrapLength;
  EFI_STATUS  Status;

  //
  // ====================================================================
  // 	Check SPI Controller avialability
  // ====================================================================
  //
  // If RCBA has not been properly set SPI controll registers will return
  // 0xF's function will return with unsuported request
  //
  // Check if SPI controller is available
  //
  PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] ========== CHECKING SOFT STRAP ========== \n"));

  Status = SpiAccessCheck ();

  if (Status != EFI_SUCCESS) {
    //
    // SPI controller was not available signal error
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: SPI Controller not available \n"));
    return Status;
  } else {
    //
    // SPI controller was is available
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] SPI Controller is available \n"));
  }
  //
  // ====================================================================
  // 	Check if we are working in Descriptor Mode
  // ====================================================================
  //
  // SPI can work in descriptor or non descriptor mode this is establish
  // by reading a valid descriptor signature 0x0FF0A55A
  //
  Status = DescriptorCheck ();

  if (Status != EFI_SUCCESS) {
    //
    // Valid signature not found we are not in descriptor mode
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: VALID SIGNATURE NOT FOUND.\n"));
    return Status;
  } else {
    //
    // Valid signature found
    //
    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] VALID SIGNATURE FOUND. \n"));
  }
  //
  // ====================================================================
  // 	Start Read Process
  // ====================================================================
  //
  if (Status == EFI_SUCCESS) {
    CmdCycle  = B_SB_SPI_READ_CYCLE;
    FlashAddr = DESCRIP_FLMAP1;

    SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &Flmap1);

    PEI_TRACE_EX ((TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION] FLASH MAP1 Off. 08h =%x.\n", Flmap1));

    //
    // Flash Map 1 offset 0x18 in Descriptor
    // ICH Strap lenght bits [31:24]
    //     identifies the 1's based number of Dwords of ICH Straps
    //     to be read up to 255 DWs 1KB max. if 0 means no ICH straps
    //
    // get bits 31:24 from FLMAP1
    //
    PchStrapLength = (UINT8) (Flmap1 >> 24);

    // Flash Map 1 offset 0x18 in Descriptor
    // ICH Strap Base Addr bits [23:16]
    //     this identifes the address bits [11:4] for the ICH strap
    //     portion of the Flash Dexcriptor. bits [24:12] and bits
    //     [3:0] are 0
    //
    PchStrapBaseAddr = (UINT8) ((Flmap1 & 0x00FF0000) >> 16);
    //
    // get bits 24:12 from FLMAP1
    //
    PchStrapBaseAddr = PchStrapBaseAddr << 4;
    //
    // shift address in to position 11:0
    //
    // Check validity of Base Addres and length
    //
    if ((PchStrapLength == 0) || (PchStrapBaseAddr == 0)) {
      PEI_TRACE_EX (
        (
        TRACE_ALWAYS, PeiServices, "[SPS_INTERACTION_ERROR]: INVALID ADDRESS=%x OR SIZE =%x.\n", PchStrapBaseAddr,
        PchStrapLength
        )
        );
      return EFI_UNSUPPORTED;
    } else {
      //
      // Start reading Soft Straps
      //      Calculate softstrap address in SPI flash
      //
      FlashAddr = PchStrapBaseAddr + (UINT32) Address;

      //
      //      Issue SPI read cycle
      //
      Status = SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &PchStrp);

      //
      //      Return Read Data in Buffer
      //
      *(UINT32 *) Buffer = PchStrp;

      //
      //      Return EFI_SUCCESS status
      //
      return Status;
    }
    //
    // end valid base and size
    //
  } else {
    return Status;
  }

}

EFI_STATUS
SetupVSCC (
  VOID
  )
{
  UINT16  VSCC;

  VSCC  = SPI_VSCC_MINUM_COMMON_DENOMINATOR;

  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_LVSCC) = VSCC;
  //
  // Set Erase block upcode
  //
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_UVSCC) = VSCC;
  //
  // Set Erase block upcode
  //
  if (*(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_UVSCC) == VSCC) {
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

EFI_STATUS
SpiAccessCheck (
  void
  )
{

  UINT32  Data32;
  UINT16  Data16;

  //
  // Check if SPI controller is available
  //
  Data32 = *(volatile UINT32 *) (UINT32) (SPI_BASE_ADDRESS);

  if (Data32 != 0xFFFFFFFF) {

    SetupVSCC ();
    //
    // Setup VSCC values.
    //
    // HSFSTS offset 0x4: Hardware Sequencing Flash Status
    // Flash Configuration Lock-Down bit [15]
    //     when set to 1 Flash program registers controlled by FLOCKDN
    //     cannot be written.
    //
    Data16 = *(volatile UINT16 *) (UINT32) (SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS);

    if (Data16 & 0x8000) {
      //
      // if Lock down is set there is nothing to do
      //
      return EFI_UNSUPPORTED;
    } else {
      //
      // SPI Controller is available
      //
      return EFI_SUCCESS;
    }
    //
    // end lock bit set
    //
  } else {
    //
    // SPI controller was not available signal error
    //
    return EFI_UNSUPPORTED;
  }
  //
  // end SPI controller available
  //
}

EFI_STATUS
DescriptorCheck (
  void
  )
{

  UINT32  Data32;
  UINT32  FlashAddr;
  UINT8   CmdCycle;

  CmdCycle  = B_SB_SPI_READ_CYCLE;
  FlashAddr = DESCRIP_ADDR;

  //
  // FLVALSIG offset 010h: Flash Valid signature
  //       bits [31:00]: 0x0FF0A55Ah
  //       This field identifies the flash descriptor sector as vaild
  //       if content is not expected value, then flash descriptor region
  //       is assumed un-programmed or corrupted
  //
  SpiDescriptorAccess (0x3, CmdCycle, FlashAddr, &Data32);

  if (Data32 == 0x0FF0A55A) {
    //
    // Valid signature found
    //
    return EFI_SUCCESS;
  } else {
    //
    // Valid signature not found we are not in descriptor mode
    //
    return EFI_UNSUPPORTED;
  }
  //
  // end valid signature
  //
}

EFI_STATUS
SpiDescriptorAccess (
  UINT8  BYTE_COUNT,
  UINT8  FLASH_CYCLE,
  UINT32 FLASH_ADDR,
  UINT32 *DATA_BUFF
  )
{
  UINT16  Mask;
  UINT8   Index;
  UINT8   ReadCmd;
  UINT8   WriteCmd;

  ReadCmd   = B_SB_SPI_READ_CYCLE;
  WriteCmd  = B_SB_SPI_WRITE_CYCLE;

  Mask      = (1 << 15) | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3);

  //
  // set Flash Address to get Descriptor
  //
  *(volatile UINT32 *) (SPI_BASE_ADDRESS + R_SB_SPI_FADDR) = FLASH_ADDR;

  //
  // initiate Read cycle to Flash and get Descriptor Region 0
  //
  // HSFCTL offset 0x6: Hardware Sequencing Flash Control
  // need to clear following bits:
  //     bits [13:8] Flash Data Byte Count(FDBC), specifies the number of bytes
  //                 to shift in or out during data portion of SPI cycle
  //     bits [07:1] Flash Cycle, this field specifies the cycle type
  //                 00 Read     (1 to 64 defined by FDBC)
  //                 01 Reserved
  //                 10 Write    (1 to 64 defined by FDBC)
  //                 11 Block Erase
  //     bit  [0]    Flash Cycle Go, A write to this register with 1 will initiate
  //                 an spi cycle
  //
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) &= Mask;
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) |= ((UINT16) BYTE_COUNT) << 8;

  //
  // READ command to SPI
  //
  if (FLASH_CYCLE == ReadCmd) {
    //
    // send Read Command
    //
    *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) |= FLASH_CYCLE;

    //
    // wait for Data to be ready
    // HSFSTS offset 0x4: Hardware Sequencing Flash Status
    // SPI Cycle in progress bit [5]
    //     Hardware sets this bit when software sets the flash cycle go
    //     this bit remains set until the cycle completes on SPI interface
    //
    while ((*(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1 << 5))
      ;

    //
    // Data is ready copy to Buffer
    //
    for (Index = 0; Index <= (BYTE_COUNT / 4); Index++) {
      DATA_BUFF[Index] = *(volatile UINT32 *) (SPI_BASE_ADDRESS + R_SB_SPI_FDAT0 + Index);
    }
  }
  //
  // WRITE command to SPI
  //
  if (FLASH_CYCLE == WriteCmd) {
    //
    // Copy Buffer to registers for writing
    //
    for (Index = 0; Index <= (BYTE_COUNT / 4); Index++) {
      *(volatile UINT32 *) (SPI_BASE_ADDRESS + R_SB_SPI_FDAT0 + Index) = DATA_BUFF[Index];
    }
    //
    // send Write Command
    //
    * (volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) |= FLASH_CYCLE;

    //
    // wait for Data to be ready
    // HSFSTS offset 0x4: Hardware Sequencing Flash Status
    // SPI Cycle in progress bit [5]
    //     Hardware sets this bit when software sets the flash cycle go
    //     this bit remains set until the cycle completes on SPI interface
    //
    while ((*(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1 << 5))
      ;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SpiBlockErase (
  UINT32 FLASH_ADDR
  )
{
  UINT16  Mask;

  Mask = (1 << 15) | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3);

  //
  // set Flash Address to get Descriptor
  //
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_FADDR) = (UINT16) FLASH_ADDR;

  //
  // HSFCTL offset 0x6: Hardware Sequencing Flash Control
  // need to clear following bits:
  //     bits [13:8] Flash Data Byte Count(FDBC), specifies the number of bytes
  //                 to shift in or out during data portion of SPI cycle
  //     bits [07:1] Flash Cycle, this field specifies the cycle type
  //                 00 Read     (1 to 64 defined by FDBC)
  //                 01 Reserved
  //                 10 Write    (1 to 64 defined by FDBC)
  //                 11 Block Erase
  //     bit  [0]    Flash Cycle Go, A write to this register with 1 will initiate
  //                 an spi cycle
  //
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) &= Mask;
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) |= 0x3F << 8;

  //
  // send Erase Command
  //
  *(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL) |= B_SB_SPI_BLOCK_ERASE;

  //
  // wait for Data to be ready
  //
  while ((*(volatile UINT16 *) (SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1 << 5))
    ;

  return EFI_SUCCESS;
}
//
// =============================================================================
// 	Access Soft Straps END.
// =============================================================================
//
