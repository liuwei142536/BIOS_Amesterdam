//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:        BootScriptLib.c
//
// Description: The function implementation for EFI S3 boot script.
//
//<AMI_FHDR_END>
//*************************************************************************

//-------------------------------------------------------------------------
// Include(s)
//-------------------------------------------------------------------------
#include <PiDxe.h>
#include <Protocol/BootScriptSave.h>
#include <Protocol/S3SaveState.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>

//-------------------------------------------------------------------------
// Variable and External Declaration(s)
//-------------------------------------------------------------------------
EFI_S3_SAVE_STATE_PROTOCOL *mS3SaveState = NULL;


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveIoWrite
//
// Description: Save I/O write to boot script.
//
// Input:       Width   - The width of the I/O operations.
//                        Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address - The base address of the I/O operations.
//              Count   - The number of I/O operations to perform.
//              Buffer  - The source buffer from which to write data.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveIoWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH          Width,
  IN  UINT64                            Address,
  IN  UINTN                             Count,
  IN  VOID                              *Buffer
  )
{
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }     
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_IO_WRITE_OPCODE, Width, Address, Count, Buffer);

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveIoReadWrite
//
// Description: Adds a record for an I/O modify operation into a S3 boot script table.
//
// Input:       Width    - The width of the I/O operations.
//                         Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address  - The base address of the I/O operations.
//              Data     - A pointer to the data to be OR-ed.
//              DataMask - A pointer to the data mask to be AND-ed with the data 
//                         read from the register.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveIoReadWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH        Width,
  IN  UINT64                          Address,
  IN  VOID                            *Data,
  IN  VOID                            *DataMask
  )
{ 
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_IO_READ_WRITE_OPCODE, Width, Address, Data, DataMask);    

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveMemWrite
//
// Description: Adds a record for a memory write operation into a specified boot script table.
//
// Input:       Width   - The width of the I/O operations.
//                        Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address - The base address of the memory operations.
//              Count   - The number of memory operations to perform.
//              Buffer  - The source buffer from which to write the data.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveMemWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH          Width,
  IN  UINT64                            Address,
  IN  UINTN                             Count,
  IN  VOID                              *Buffer
  )
{
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_MEM_WRITE_OPCODE, Width, Address, Count, Buffer);
    
    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveMemReadWrite
//
// Description: Adds a record for a memory modify operation into a specified boot script table.
//
// Input:       Width    - The width of the I/O operations.
//                         Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address  - The base address of the memory operations. 
//                         Address needs alignment if required
//              Data     - A pointer to the data to be OR-ed.
//              DataMask - A pointer to the data mask to be AND-ed with the data 
//                         read from the register.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveMemReadWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH          Width,
  IN  UINT64                            Address,
  IN  VOID                              *Data,
  IN  VOID                              *DataMask
  )
{
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_MEM_READ_WRITE_OPCODE, Width, Address, Data, DataMask);    

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePciCfgWrite
//
// Description: Adds a record for a PCI configuration space write operation 
//              into a specified boot script table.
//
// Input:       Width   - The width of the I/O operations.
//                        Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address - The address within the PCI configuration space.
//              Count   - The number of PCI operations to perform.
//              Buffer  - The source buffer from which to write the data.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePciCfgWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH         Width,
  IN  UINT64                           Address,
  IN  UINTN                            Count,
  IN  VOID                             *Buffer
  )
{ 
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG_WRITE_OPCODE, Width, Address, Count, Buffer);    

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePciCfgReadWrite
//
// Description: Adds a record for a PCI configuration space modify operation 
//              into a specified boot script table.
//
// Input:       Width    - The width of the I/O operations.
//                         Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Address  - The address within the PCI configuration space.
//              Data     - A pointer to the data to be OR-ed.
//                         The size depends on Width.
//              DataMask - A pointer to the data mask to be AND-ed.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePciCfgReadWrite (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH          Width,
  IN  UINT64                            Address,
  IN  VOID                              *Data,
  IN  VOID                              *DataMask
  )
{  
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG_READ_WRITE_OPCODE, Width, Address, Data, DataMask);

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePciCfg2Write
//
// Description: Adds a record for a PCI configuration space modify operation 
//              into a specified boot script table.
//
// Input:       Width   - The width of the I/O operations.
//                        Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Segment - The PCI segment number for Address.
//              Address - The address within the PCI configuration space.
//              Count   - The number of PCI operations to perform.
//              Buffer  - The source buffer from which to write the data.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePciCfg2Write (
  IN S3_BOOT_SCRIPT_LIB_WIDTH        Width,
  IN UINT16                          Segment,
  IN UINT64                          Address,
  IN UINTN                           Count,
  IN VOID                            *Buffer
  )
{ 
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG2_WRITE_OPCODE, Width, Segment, Address, Count, Buffer);

    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePciCfg2ReadWrite
//
// Description: Adds a record for a PCI configuration space modify operation 
//              into a specified boot script table.
//
// Input:       Width    - The width of the I/O operations.
//                         Enumerated in S3_BOOT_SCRIPT_LIB_WIDTH.
//              Segment  - The PCI segment number for Address.
//              Address  - The address within the PCI configuration space. 
//              Data     - A pointer to the data to be OR-ed.
//              DataMask - A pointer to the data mask to be AND-ed.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePciCfg2ReadWrite (
  IN S3_BOOT_SCRIPT_LIB_WIDTH        Width,
  IN UINT16                          Segment,
  IN UINT64                          Address,
  IN VOID                            *Data,
  IN VOID                            *DataMask
  )
{    
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG2_READ_WRITE_OPCODE, Width, Segment, Address, Data, DataMask);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveSmbusExecute
//
// Description: Adds a record for an SMBus command execution into a specified boot script table.
//
// Input:       SmBusAddress - Address that encodes the SMBUS Slave Address, 
//                             SMBUS Command, SMBUS Data Length, and PEC.
//              Operation    - Indicates which particular SMBus protocol it will 
//                             use to execute the SMBus transactions.
//              Length       - A pointer to signify the number of bytes that this 
//                             operation will do.
//              Buffer       - Contains the value of data to execute to the SMBUS 
//                             slave device.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveSmbusExecute (
  IN  UINTN                             SmBusAddress, 
  IN  EFI_SMBUS_OPERATION               Operation,
  IN  UINTN                             *Length,
  IN  VOID                              *Buffer
  )
{   
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_SMBUS_EXECUTE_OPCODE, (((SmBusAddress) >> 1)  & 0x7f), (((SmBusAddress) >> 8)  & 0xff), Operation, (BOOLEAN) (((SmBusAddress) & (1 << 22)) != 0), Length, Buffer);

    return RETURN_SUCCESS;      
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveSmbusExecute
//
// Description: Adds a record for an execution stall on the processor into a specified boot script table.
//
// Input:       Duration - Duration in microseconds of the stall.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveStall (
  IN  UINTN                             Duration
  )
{    
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_STALL_OPCODE, Duration);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveDispatch
//
// Description: Adds a record for dispatching specified arbitrary code into a specified boot script table.
//
// Input:       EntryPoint - Entry point of the code to be dispatched.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveDispatch (
  IN  VOID *EntryPoint
  )
{  
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_DISPATCH_OPCODE, EntryPoint);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveDispatch2
//
// Description: Adds a record for an execution stall on the processor into a specified boot script table.
//
// Input:       EntryPoint - Entry point of the code to be dispatched.
//              Context    - Argument to be passed into the EntryPoint of the 
//                           code to be dispatched.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveDispatch2 (
  IN  VOID                              *EntryPoint,
  IN  VOID                              *Context
  )
{   
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_DISPATCH_2_OPCODE, EntryPoint, Context);

    return RETURN_SUCCESS;     
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveMemPoll
//
// Description: Adds a record for memory reads of the memory location and continues 
//              when the exit criteria is satisfied or after a defined duration.
//
// Input:       Width     - The width of the memory operations.
//              Address   - The base address of the memory operations.
//              BitMask   - A pointer to the bit mask to be AND-ed with the data 
//                          read from the register.
//              BitValue  - A pointer to the data value after to be Masked.
//              Duration  - Duration in microseconds of the stall.
//              LoopTimes - The times of the register polling.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveMemPoll (
  IN  S3_BOOT_SCRIPT_LIB_WIDTH          Width,
  IN  UINT64                            Address,
  IN  VOID                              *BitMask,
  IN  VOID                              *BitValue,
  IN  UINTN                             Duration,
  IN  UINTN                             LoopTimes
  )
{   
    EFI_STATUS    Status;
    UINT64        Delay;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    // Duration in microseconds of the stall, Delay is the number of 100ns units.
    Delay = (UINT64)(Duration * LoopTimes * 10);
    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_MEM_POLL_OPCODE, Width, Address, BitValue, BitMask, Delay);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveInformation
//
// Description: Store arbitrary information in the boot script table. This opcode 
//              is a no-op on dispatch and is only used for debugging script issues.
//
// Input:       InformationLength - Length of the data in bytes
//              Information       - Information to be logged in the boot scrpit.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveInformation (
  IN  UINT32                   InformationLength, 
  IN  VOID                     *Information
  )
{    
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_INFORMATION_OPCODE, InformationLength, (EFI_PHYSICAL_ADDRESS)Information);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveIoPoll
//
// Description: Adds a record for I/O reads the I/O location and continues when 
//              the exit criteria is satisfied or after a defined duration.
//
// Input:       Width    - The width of the I/O operations. 
//              Address  - The base address of the I/O operations.
//              Data     - The comparison value used for the polling exit criteria.
//              DataMask - Mask used for the polling criteria. The bits in the 
//                         bytes below Width which are zero in Data are ignored 
//                         when polling the memory address.
//              Delay    - The number of 100ns units to poll. Note that timer 
//                         available may be of poorer granularity so the delay may 
//                         be longer.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveIoPoll (
  IN S3_BOOT_SCRIPT_LIB_WIDTH   Width,
  IN UINT64                     Address,
  IN VOID                       *Data,
  IN VOID                       *DataMask, 
  IN UINT64                     Delay   
  )
{    
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_IO_POLL_OPCODE, Width, Address, Data, DataMask, Delay);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePciPoll
//
// Description: Adds a record for PCI configuration space reads and continues when 
//              the exit criteria is satisfied or after a defined duration.
//
// Input:       Width    - The width of the I/O operations. 
//              Address  - The address within the PCI configuration space.
//              Data     - The comparison value used for the polling exit criteria.
//              DataMask - Mask used for the polling criteria. The bits in the 
//                         bytes below Width which are zero in Data are ignored 
//                         when polling the memory address.
//              Delay    - The number of 100ns units to poll. Note that timer 
//                         available may be of poorer granularity so the delay may 
//                         be longer.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePciPoll (
   IN S3_BOOT_SCRIPT_LIB_WIDTH   Width,
   IN UINT64                     Address,
   IN VOID                       *Data,
   IN VOID                       *DataMask,
   IN UINT64                     Delay
 )
{    
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG_POLL_OPCODE, Width, Address, Data, DataMask, Delay);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSavePci2Poll
//
// Description: Adds a record for PCI configuration space reads and continues when 
//              the exit criteria is satisfied or after a defined duration.
//
// Input:       Width    - The width of the I/O operations. 
//              Segment  - The PCI segment number for Address.
//              Address  - The address within the PCI configuration space.
//              Data     - The comparison value used for the polling exit criteria.
//              DataMask - Mask used for the polling criteria. The bits in the 
//                         bytes below Width which are zero in Data are ignored 
//                         when polling the memory address.
//              Delay    - The number of 100ns units to poll. Note that timer 
//                         available may be of poorer granularity so the delay may 
//                         be longer.
//
// Output:      RETURN_OUT_OF_RESOURCES - Not enough memory for the table do operation.
//              RETURN_SUCCESS          - Opcode is added.
//
// Note:        A known Limitations in the implementation: When interpreting the opcode  
//              EFI_BOOT_SCRIPT_PCI_CONFIG2_WRITE_OPCODE, EFI_BOOT_SCRIPT_PCI_CONFIG2_READ_WRITE_OPCODE 
//              and EFI_BOOT_SCRIPT_PCI_CONFIG2_POLL_OPCODE, the 'Segment' parameter is 
//              assumed as Zero, or else, assert.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSavePci2Poll (
   IN S3_BOOT_SCRIPT_LIB_WIDTH      Width,
   IN UINT16                        Segment,
   IN UINT64                        Address,
   IN VOID                          *Data,
   IN VOID                          *DataMask,
   IN UINT64                        Delay
  )
{   
    EFI_STATUS    Status;
    
    if (mS3SaveState == NULL) {
      Status = gBS->LocateProtocol (&gEfiS3SaveStateProtocolGuid, NULL, &mS3SaveState);
      if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR (Status);
        return RETURN_PROTOCOL_ERROR;
      }
    }

    mS3SaveState->Write (mS3SaveState, EFI_BOOT_SCRIPT_PCI_CONFIG2_POLL_OPCODE, Width, Segment, Address, Data, DataMask, Delay);

    return RETURN_SUCCESS;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptSaveInformationAsciiString
//
// Description: Save ASCII string information specified by Buffer to boot script
//              with opcode EFI_BOOT_SCRIPT_INFORMATION_OPCODE
//
// Input:       String - the ascii string to store into the S3 boot script table
//
// Output:      RETURN_NOT_FOUND - BootScriptSave Protocol not exist.
//              RETURN_SUCCESS   - BootScriptSave Protocol exist, always returns 
//								   RETURN_SUCCESS.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptSaveInformationAsciiString (
  IN  CONST CHAR8               *String
  )
{
  return S3BootScriptSaveInformation (      
           (UINT32) AsciiStrLen (String) + 1, 
           (VOID *)String
           );
}

/**
  This is an function to close the S3 boot script table. The function could only be called in 
  BOOT time phase. To comply with the Framework spec definition on 
  EFI_BOOT_SCRIPT_SAVE_PROTOCOL.CloseTable(), this function will fulfill following things:
  1. Closes the specified boot script table
  2. It allocates a new memory pool to duplicate all the boot scripts in the specified table. 
     Once this function is called, the table maintained by the library will be destroyed 
     after it is copied into the allocated pool.
  3. Any attempts to add a script record after calling this function will cause a new table 
     to be created by the library.
  4. The base address of the allocated pool will be returned in Address. Note that after 
     using the boot script table, the CALLER is responsible for freeing the pool that is allocated
     by this function. 

  In Spec PI1.1, this EFI_BOOT_SCRIPT_SAVE_PROTOCOL.CloseTable() is retired. By then it is not
  necessary to provide this API in BootScriptLib. To provides this API for now is only to meet
  the requirement from Framework Spec.
  
  If anyone does call CloseTable() on a real platform, then the caller is responsible for figuring out 
  how to get the script to run on an S3 resume because the boot script maintained by the lib will be 
  destroyed.
 
  @return the base address of the new copy of the boot script tble.   

**/
UINT8*
EFIAPI
S3BootScriptCloseTable (
  VOID
  )
{
    return 0;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptExecute
//
// Description: Executes the S3 boot script table.
//
// Output:      RETURN_SUCCESS     - The boot script table was executed successfully.
//              RETURN_UNSUPPORTED - Invalid script table or opcode.  
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptExecute (
   VOID
  )
{
    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptMoveLastOpcode
//
// Description: Move the last boot script entry to the position 
//
// Input:       BeforeOrAfter - Specifies whether the opcode is stored before (TRUE) 
//                              or after (FALSE) the position in the boot script table 
//                              specified by Position. If Position is NULL or points 
//                              to NULL then the new opcode is inserted at the beginning
//                              of the table (if TRUE) or end of the table (if FALSE).
//              Position      - On entry, specifies the position in the boot script 
//                              table where the opcode will be inserted, either before 
//                              or after, depending on BeforeOrAfter. On exit, specifies
//                              the position of the inserted opcode in the boot script table.
//                                    
// Output:      RETURN_OUT_OF_RESOURCES  - The table is not available.
//              RETURN_INVALID_PARAMETER - The Position is not a valid position in the 
//                                         boot script table.
//              RETURN_SUCCESS           - Opcode is inserted.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI
S3BootScriptMoveLastOpcode (
  IN     BOOLEAN                        BeforeOrAfter,
  IN OUT VOID                           **Position OPTIONAL
)
{
    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptLabel
//
// Description: Find a label within the boot script table and, if not present, 
//              optionally create it.
//
// Input:       BeforeOrAfter    - Specifies whether the opcode is stored before (TRUE) 
//                                 or after (FALSE) the position in the boot script table 
//                                 specified by Position.
//              CreateIfNotFound - Specifies whether the label will be created if the label 
//                                 does not exists (TRUE) or not (FALSE).
//              Position         - On entry, specifies the position in the boot script table
//                                 where the opcode will be inserted, either before or after,
//                                 depending on BeforeOrAfter. On exit, specifies the position
//                                 of the inserted opcode in the boot script table.
//              Label            - Points to the label which will be inserted in the boot script table.
//                                    
// Output:      EFI_SUCCESS           - The operation succeeded. A record was added into the
//                                      specified script table.
//              EFI_INVALID_PARAMETER - The parameter is illegal or the given boot script is 
//                                      not supported. If the opcode is unknow or not supported 
//                                      because of the PCD Feature Flags.
//              EFI_OUT_OF_RESOURCES  - There is insufficient memory to store the boot script.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI 
S3BootScriptLabel (
  IN       BOOLEAN                      BeforeOrAfter,
  IN       BOOLEAN                      CreateIfNotFound,
  IN OUT   VOID                         **Position OPTIONAL,
  IN CONST CHAR8                        *Label
  )
{
    return RETURN_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   S3BootScriptCompare
//
// Description: Compare two positions in the boot script table and return their 
//              relative position.
//
// Input:       Position1        - The positions in the boot script table to compare
//              Position2        - The positions in the boot script table to compare
//              RelativePosition - On return, points to the result of the comparison
//                                    
// Output:      EFI_SUCCESS           - The operation succeeded. A record was added into the
//                                      specified script table.
//              EFI_INVALID_PARAMETER - The parameter is illegal or the given boot script is 
//                                      not supported. If the opcode is unknow or not supported 
//                                      because of the PCD Feature Flags.
//              EFI_OUT_OF_RESOURCES  - There is insufficient memory to store the boot script.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

RETURN_STATUS
EFIAPI 
S3BootScriptCompare (
  IN  UINT8                       *Position1,
  IN  UINT8                       *Position2,
  OUT UINTN                       *RelativePosition
  )
{
    return RETURN_SUCCESS;
}
