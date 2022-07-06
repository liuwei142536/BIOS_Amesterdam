/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2013 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemrasS3Save.c

Abstract:

  MemrasS3 Save support

--*/

#include "Memras.h"
#include "Platform.h"
#include <Protocol/Decompress.h>
#include <Library/CompressLib.h>
#include <Library/UefiDecompressLib.h>


extern  SMMELOG_HOST                  mMemrasS3Param;

// Aptio V Server Override Start: AMI Use different way to for the GET/SET variable in SMM.
//EFI_SMM_VARIABLE_PROTOCOL             *mSmmVariable;

#define EFI_SMM_RUNTIME_SERVICES_TABLE_GUID \
    { 0x395c33fe, 0x287f, 0x413e, { 0xa0, 0x55, 0x80, 0x88, 0xc0, 0xe1, 0xd4, 0x3e } }

EFI_RUNTIME_SERVICES 					*SmmpRs = NULL;
EFI_CONFIGURATION_TABLE 				*SmmConfigurationTable = NULL;
EFI_GUID SmmRtServTableGuid 		= EFI_SMM_RUNTIME_SERVICES_TABLE_GUID; 
// Aptio V Server Override End: AMI Use different way to for the GET/SET variable in SMM.

EFI_HANDLE                                mDecompressHandle ;

EFI_STATUS
EFIAPI
SmmRasUefiDecompressGetInfo (
  IN EFI_DECOMPRESS_PROTOCOL            *This,
  IN   VOID                             *Source,
  IN   UINT32                           SourceSize,
  OUT  UINT32                           *DestinationSize,
  OUT  UINT32                           *ScratchSize
  );

EFI_STATUS
EFIAPI
SmmRasUefiDecompress (
  IN     EFI_DECOMPRESS_PROTOCOL          *This,
  IN     VOID                             *Source,
  IN     UINT32                           SourceSize,
  IN OUT VOID                             *Destination,
  IN     UINT32                           DestinationSize,
  IN OUT VOID                             *Scratch,
  IN     UINT32                           ScratchSize
  );


EFI_DECOMPRESS_PROTOCOL  gEfiDecompress = {
  SmmRasUefiDecompressGetInfo,
  SmmRasUefiDecompress
};


/**
  Given a compressed source buffer, this function retrieves the size of the
  uncompressed buffer and the size of the scratch buffer required to decompress
  the compressed source buffer.

  The GetInfo() function retrieves the size of the uncompressed buffer and the
  temporary scratch buffer required to decompress the buffer specified by Source
  and SourceSize. If the size of the uncompressed buffer or the size of the
  scratch buffer cannot be determined from the compressed data specified by
  Source and SourceData, then EFI_INVALID_PARAMETER is returned. Otherwise, the
  size of the uncompressed buffer is returned in DestinationSize, the size of
  the scratch buffer is returned in ScratchSize, and EFI_SUCCESS is returned.
  The GetInfo() function does not have scratch buffer available to perform a
  thorough checking of the validity of the source data. It just retrieves the
  "Original Size" field from the beginning bytes of the source data and output
  it as DestinationSize. And ScratchSize is specific to the decompression
  implementation.

  @param  This               A pointer to the EFI_DECOMPRESS_PROTOCOL instance.
  @param  Source             The source buffer containing the compressed data.
  @param  SourceSize         The size, in bytes, of the source buffer.
  @param  DestinationSize    A pointer to the size, in bytes, of the
                             uncompressed buffer that will be generated when the
                             compressed buffer specified by Source and
                             SourceSize is decompressed.
  @param  ScratchSize        A pointer to the size, in bytes, of the scratch
                             buffer that is required to decompress the
                             compressed buffer specified by Source and
                             SourceSize.

  @retval EFI_SUCCESS        The size of the uncompressed data was returned in
                             DestinationSize and the size of the scratch buffer
                             was returned in ScratchSize.
  @retval EFI_INVALID_PARAMETER The size of the uncompressed data or the size of
                                the scratch buffer cannot be determined from the
                                compressed data specified by Source and
                                SourceSize.

**/
EFI_STATUS
EFIAPI
SmmRasUefiDecompressGetInfo (
  IN EFI_DECOMPRESS_PROTOCOL            *This,
  IN   VOID                             *Source,
  IN   UINT32                           SourceSize,
  OUT  UINT32                           *DestinationSize,
  OUT  UINT32                           *ScratchSize
  )
{
  if (Source == NULL || DestinationSize == NULL || ScratchSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return UefiDecompressGetInfo (Source, SourceSize, DestinationSize, ScratchSize);
}


/**
  Decompresses a compressed source buffer.

  The Decompress() function extracts decompressed data to its original form.
  This protocol is designed so that the decompression algorithm can be
  implemented without using any memory services. As a result, the Decompress()
  Function is not allowed to call AllocatePool() or AllocatePages() in its
  implementation. It is the caller's responsibility to allocate and free the
  Destination and Scratch buffers.
  If the compressed source data specified by Source and SourceSize is
  sucessfully decompressed into Destination, then EFI_SUCCESS is returned. If
  the compressed source data specified by Source and SourceSize is not in a
  valid compressed data format, then EFI_INVALID_PARAMETER is returned.

  @param  This                A pointer to the EFI_DECOMPRESS_PROTOCOL instance.
  @param  Source              The source buffer containing the compressed data.
  @param  SourceSize          SourceSizeThe size of source data.
  @param  Destination         On output, the destination buffer that contains
                              the uncompressed data.
  @param  DestinationSize     The size of the destination buffer.  The size of
                              the destination buffer needed is obtained from
                              EFI_DECOMPRESS_PROTOCOL.GetInfo().
  @param  Scratch             A temporary scratch buffer that is used to perform
                              the decompression.
  @param  ScratchSize         The size of scratch buffer. The size of the
                              scratch buffer needed is obtained from GetInfo().

  @retval EFI_SUCCESS         Decompression completed successfully, and the
                              uncompressed buffer is returned in Destination.
  @retval EFI_INVALID_PARAMETER  The source buffer specified by Source and
                                 SourceSize is corrupted (not in a valid
                                 compressed format).
**/
EFI_STATUS
EFIAPI
SmmRasUefiDecompress (
  IN     EFI_DECOMPRESS_PROTOCOL          *This,
  IN     VOID                             *Source,
  IN     UINT32                           SourceSize,
  IN OUT VOID                             *Destination,
  IN     UINT32                           DestinationSize,
  IN OUT VOID                             *Scratch,
  IN     UINT32                           ScratchSize
  )
{
  if (Source == NULL || Destination== NULL || Scratch == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return UefiDecompress (Source, Destination, Scratch);
}




INT16 Crc16 (char *ptr, int count)
{
  INT16 crc, i;
  crc = 0;
  while (--count >= 0)
  {
    crc = crc ^ (INT16)(int)*ptr++ << 8;
    for (i = 0; i < 8; ++i)
    {
      if (crc & 0x8000)
      {
        crc = crc << 1 ^ 0x1021;
      }
      else
      {
        crc = crc << 1;
      }
    }
  }
  return (crc & 0xFFFF);
}

/*++

Routine Description:
    Get or update MemoryConfigX variables.
    This routine will not create the variables if they do not already exist.

Arguments:

  GetVars    - Flag is TRUE  to indicate routine is to get the variable data.
               Flag is FALSE to indicate routine is to update the variable data.
  *Buffer    - Pointer to caller-supplied buffer for the variable data.
  BufferSize - Size of the buffer.

Returns:

    Status   - EFI_SUCCESS if no failures
               non-zero error code for failures
--*/
EFI_STATUS
MemoryConfigVars (
  IN     BOOLEAN GetVars,
  IN     UINTN   BufferSize,
  IN OUT CHAR8   *Buffer
  )
{
  #define MEM_CFG_VARS_ATTRIBS (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_RUNTIME_ALLOW_RECLAIM)

  EFI_STATUS     Status;
  UINTN          VariableSize;
  CHAR16         EfiMemoryConfigVariable[] = L"MemoryConfig0";

  UINTN                      CompressedVariableSize;
  UINT32                     ScratchSize;
  VOID                       *CompressedData = NULL;
  VOID                       *Scratch = NULL;
  EFI_DECOMPRESS_PROTOCOL    *Decompress = NULL;

  VOID                            *CompressedVariableData = NULL;
  UINTN                           CompressedBufferSize;

  Status = EFI_SUCCESS; // Satisfy compiler.

  if (Buffer == NULL) {
    BufferSize = 0;
  }

  if (BufferSize == 0) {
    Status = EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiDecompressProtocolGuid, NULL, (VOID **) &Decompress);
  if (EFI_ERROR (Status)) {
    return EFI_PROTOCOL_ERROR;
  }

  while (BufferSize) {

    //
    // Get the compressed variable size.
    //
    VariableSize = 0 ;
    CompressedVariableSize = 0;
    ScratchSize = 0;

//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
/*    
    Status = mSmmVariable->SmmGetVariable (
                        EfiMemoryConfigVariable,
                        &gEfiMemoryConfigDataGuid,
                        NULL,
                        &CompressedVariableSize,
                        NULL
                        );
*/
    
    Status = SmmpRs->GetVariable(
    					EfiMemoryConfigVariable,
                        &gEfiMemoryConfigDataGuid,
                        NULL,
                        &CompressedVariableSize,
                        NULL
                        );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
    
    if (Status == EFI_BUFFER_TOO_SMALL) {
      Status = gSmst->SmmAllocatePool (
                             EfiRuntimeServicesData,
                             CompressedVariableSize,
                             &CompressedData
                             );

      //
      // Get compressed variable.
      //
      if(!EFI_ERROR (Status)){

//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions   
/*
        Status = mSmmVariable->SmmGetVariable (
                    EfiMemoryConfigVariable,
                    &gEfiMemoryConfigDataGuid,
                    NULL,
                    &CompressedVariableSize,
                    CompressedData
                    );
*/     
    	  Status = SmmpRs->GetVariable(
    	      					  EfiMemoryConfigVariable,
    	                          &gEfiMemoryConfigDataGuid,
    	                          NULL,
    	                          &CompressedVariableSize,
    	                          CompressedData
    	                          );
//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions        
      }

      //
      // Get info about buffers needed to compression and decompression
      //
      if(!EFI_ERROR (Status)) {
          Status = Decompress->GetInfo (
                         Decompress,
                         CompressedData,
                         (UINT32)CompressedVariableSize,
                         &(UINT32)VariableSize,
                         &ScratchSize
                         );
      }

      if (VariableSize > BufferSize) {
        Status = EFI_BAD_BUFFER_SIZE;
      } else {

        if (GetVars) {
            //
            // Allocate memory for additional buffer needed to decompression
            //
            if(Status == EFI_SUCCESS) {
              Status = gSmst->SmmAllocatePool (
                     EfiRuntimeServicesData,
                     ScratchSize,
                     &Scratch
                     );
            }

            //
            // Do decompression
            //
            if(!EFI_ERROR (Status)) {
              Status = Decompress->Decompress (
                             Decompress,
                             CompressedData,
                             (UINT32)CompressedVariableSize,
                             Buffer,
                             (UINT32)VariableSize,
                             Scratch,
                             ScratchSize
                             );
            }

            if(Scratch != NULL) {
              gSmst->SmmFreePool (Scratch);
              Scratch = NULL;
            }
            if(CompressedData != NULL) {
              gSmst->SmmFreePool (CompressedData);
              CompressedData = NULL;
            }
          } else {
            if(CompressedData != NULL) {
              gSmst->SmmFreePool (CompressedData);
              CompressedData = NULL;
            }
            //
            // Update the variable.
            //

            //
            // Allocate memory for compressed data.
            //

            CompressedBufferSize = VariableSize;

            if(!EFI_ERROR (Status)) {
              Status = gSmst->SmmAllocatePool (
                            EfiRuntimeServicesData,
                            CompressedBufferSize,
                            &CompressedVariableData
                            );
            }

            if(!EFI_ERROR (Status)) {
              Status = Compress(Buffer, VariableSize, CompressedVariableData, &CompressedBufferSize);
            }

          if(!EFI_ERROR (Status)) {
        	  
/*  //AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions

            Status = mSmmVariable->SmmSetVariable (
                      EfiMemoryConfigVariable,
                      &gEfiMemoryConfigDataGuid,
                      MEM_CFG_VARS_ATTRIBS,
                      CompressedBufferSize,
                      CompressedVariableData
                      );
*/
        	  Status = SmmpRs->SetVariable(
        			              EfiMemoryConfigVariable,
        	                      &gEfiMemoryConfigDataGuid,
        	                      MEM_CFG_VARS_ATTRIBS,
        	                      CompressedBufferSize,
        	                      CompressedVariableData 
        	                      );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions        	  
            if (Status == EFI_OUT_OF_RESOURCES) {
              //
              // Delete variable and try again.
              //
//AptioV Server Override start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions             	
/*            	
              Status = mSmmVariable->SmmSetVariable (
                        EfiMemoryConfigVariable,
                        &gEfiMemoryConfigDataGuid,
                        MEM_CFG_VARS_ATTRIBS,
                        0,
                        CompressedVariableData
                        );
*/
            	Status = SmmpRs->SetVariable(
            	        			              EfiMemoryConfigVariable,
            	        	                      &gEfiMemoryConfigDataGuid,
            	        	                      MEM_CFG_VARS_ATTRIBS,
            	        	                      0,
            	        	                      CompressedVariableData 
            	        	                      );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions 
              if (!EFI_ERROR (Status)) {
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions             	  
/*            	  
                Status = mSmmVariable->SmmSetVariable (
                          EfiMemoryConfigVariable,
                          &gEfiMemoryConfigDataGuid,
                          MEM_CFG_VARS_ATTRIBS,
                          CompressedBufferSize,
                          CompressedVariableData
                          );
*/

            	  Status = SmmpRs->SetVariable(
            	              	        		EfiMemoryConfigVariable,
            	              	        	    &gEfiMemoryConfigDataGuid,
            	              	        	    MEM_CFG_VARS_ATTRIBS,
            	              	        	    CompressedBufferSize,
            	              	        	    CompressedVariableData 
            	              	        	    );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions 
              }
            }
          }
          if(CompressedVariableData != NULL) {
            gSmst->SmmFreePool (CompressedVariableData);
            CompressedVariableData = NULL;
          }
        }
      }
      if(CompressedData != NULL) { //for security in casae of some errors
        gSmst->SmmFreePool (CompressedData); 
        CompressedData = NULL;
      }
    }

    if (!EFI_ERROR (Status)) {
      //
      // Move the buffer pointer.
      //
      Buffer += VariableSize;

      //
      // Update the remaining buffer size.
      //
      BufferSize -= VariableSize;

      //
      // Change to the next variable name.
      //
      EfiMemoryConfigVariable[12]++;
    } else {
      BufferSize = 0;
    }
  }

  return Status;
}

EFI_STATUS
MemRASS3EntryCallback (
  IN      EFI_HANDLE                         DispatchHandle,
  IN      CONST EFI_SMM_SX_REGISTER_CONTEXT  *DispatchContext,
  IN OUT  VOID                               *CommBuffer,     OPTIONAL
  IN OUT  UINTN                              *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                  Status;
  struct sysNvram             *MemCfgData;
  UINTN                       DataSize;
  UINT8                       ch;
  UINT8                       dimm;
  UINT8                       node;
  UINT8                       socket;
  UINT8                       haNum;
  UINT8                       DdrCh;
  UINT8                       rank;
  UINT8                       logicalRank;
  SCRUBCTL_MC_MAINEXT_STRUCT  ScrubCtlReg;
  UINT8                       spareInUse;
  INT16                       data16;
  char                        *memNvramPtr;
  UINT8                       i;

  //AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
  
  UINTN                    NumberOfTableEntries;
  UINT8					   Index;

  //
  // Locate SmmVariableProtocol.
  //
 /* Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&mSmmVariable);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR(Status)) return EFI_SUCCESS;
  */
  
  //AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
  
  //AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
      if (SmmConfigurationTable == NULL) {
        SmmConfigurationTable = gSmst->SmmConfigurationTable;
        NumberOfTableEntries = gSmst->NumberOfTableEntries;
        for (Index = 0; Index < NumberOfTableEntries; Index++) {
          if (CompareGuid (&SmmConfigurationTable[Index].VendorGuid, &SmmRtServTableGuid)) {
      	  SmmpRs = SmmConfigurationTable[Index].VendorTable;
          }
        }
      }
      if (SmmpRs == NULL) {
        ASSERT_EFI_ERROR (EFI_NOT_FOUND);
        return EFI_SUCCESS;
      }
  //AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions    

  DataSize = sizeof (struct sysNvram);

  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      DataSize,
                      &MemCfgData
                      );
  if (EFI_ERROR(Status)) return EFI_SUCCESS;

  //
  // Get the memory configuration variables.
  //
  Status = MemoryConfigVars (TRUE, DataSize, (UINT8 *) (MemCfgData));
  if (EFI_ERROR(Status)) {
    gSmst->SmmFreePool (MemCfgData);
    return EFI_SUCCESS;
  }

  for (node = 0; node < MC_MAX_NODE; node++) {
    if (mSystemMemoryMap->NodeInfo[node].Enabled == 0) continue;

    socket = NodeToSkt(node);
    haNum  = NodeToHaNum(node);
    ScrubCtlReg.Data = MemReadPciCfg (node, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
    if (ScrubCtlReg.Bits.scrub_en == 1) {
      // Turn off scrubbing.
      ScrubCtlReg.Bits.scrub_en = 0;
      MemWritePciCfg (node, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG, ScrubCtlReg.Data);
      // Update The Scrub address Low and High fields
      MemCfgData->mem.socket[socket].imc[haNum].scrubAddrLo = MemReadPciCfg (node, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG);
      MemCfgData->mem.socket[socket].imc[haNum].scrubAddrHi = MemReadPciCfg (node, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
    }

    //
    // Leaky bucket configuration and counter.
    //
    MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCfg = MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CFG_MC_MAINEXT_REG);
    MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCounterHi = MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG);
    MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCounterLo = MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_REG);
    if (MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCounterHi != MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG)) {
      //
      // Low part of counter rolled over into high part.  Read counter again.
      //
      MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCounterHi = MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG);
      MemCfgData->mem.socket[socket].imc[haNum].leakyBucketCounterLo = MemReadPciCfg (node, PARAM_UNUSED, LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_REG);
    }

    for (ch = 0; ch < MAX_CH; ch++) {
      if (mSystemMemoryMap->NodeInfo[node].ChannelInfo[ch].Enabled == 0) continue;

      DdrCh = NodeChToDdrCh(node, ch);
      if (DdrCh == 0xFF) continue;

      spareInUse = mMemrasS3Param.imc[node].channelList[ch].spareInUse;
      MemCfgData->mem.socket[socket].channelList[DdrCh].spareInUse = spareInUse;
      MemCfgData->mem.socket[socket].channelList[DdrCh].chFailed   = mMemrasS3Param.imc[node].channelList[ch].chFailed;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].dimmPresent == 0) continue;

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].rankList[rank].enabled == 0) continue;
          if (MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].mapOut[rank] == 1) continue;

          //if (mDevTaggingStatus[node][ddrCh][rank] == DevTaggingAvailable) {
            logicalRank = MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].rankList[rank].logicalRank;

            //
            // Update dev tag info in nvram from dev tag logical rank info
            //
            MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].rankList[rank].devTagInfo =
                                                           mMemrasS3Param.imc[node].channelList[ch].devTag[logicalRank];
          //}
          //
          // Save physical rank id of old spared rank in nvram
          //
          if (spareInUse == 1) {
            if (logicalRank ==  mMemrasS3Param.imc[node].channelList[ch].oldLogicalRank) {
              MemCfgData->mem.socket[socket].channelList[DdrCh].oldSparePhysicalRank =
                    MemCfgData->mem.socket[socket].channelList[DdrCh].dimmList[dimm].rankList[rank].phyRank;

              // Loop to add the oldSpare rank and New spare rank incase of multi rank support.
              //MemCfgData->nvram.mem.socket[socket].channelList[DdrCh].oldSparePhysicalRank[mMemrasS3Param.imc[NodeId].channelList[ch].MultiSpareRankCnt] = SrcRank;
              //(mMemrasS3Param.imc[NodeId].channelList[ch].MultiSpareRankCnt)++;
              //MemCfgData->nvram.common.imc[node].channelList[ch].newSparePhysRank =
              //      MemCfgData->nvram.common.imc[node].channelList[ch].dimmList[dimm].rankList[rank].phyRank;
            }
          }
        } // rank
      } // dimm

      //
      // Per rank error counts and thresholds
      //
      for (i = 0; i < MAX_RANK_CH/2; i++) {
        MemCfgData->mem.socket[socket].channelList[DdrCh].rankErrCountInfo[i] = MemReadPciCfg (node, ch, CORRERRCNT_0_MCDDC_DP_REG + i*4);
        MemCfgData->mem.socket[socket].channelList[DdrCh].rankErrThresholdInfo[i] = MemReadPciCfg (node, ch, CORRERRTHRSHLD_0_MCDDC_DP_REG + i*4);
      } // i

      //
      // Leaky bucket 2nd counters
      //
      MemCfgData->mem.socket[socket].channelList[DdrCh].leakyBucket2ndCounter = MemReadPciCfg (node, ch, LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG);
    } // ch

    //
    // SMI settings.
    //
    MemCfgData->mem.socket[socket].smiCtrlUboxMisc = MemReadPciCfg (socket, 0, SMICTRL_UBOX_MISC_REG);
    MemCfgData->mem.socket[socket].imc[haNum].EmcaLtCtlMcMainExt   = (UINT8) MemReadPciCfg (node, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG);
    MemCfgData->mem.socket[socket].imc[haNum].ExRasConfigHaCfg     = MemReadPciCfg (socket, haNum, EXRAS_CONFIG_HA_CFG_REG);
    MemCfgData->mem.socket[socket].imc[haNum].SmiSpareCtlMcMainExt = MemReadPciCfg (node, PARAM_UNUSED, SMISPARECTL_MC_MAINEXT_REG);
  } // node

  //
  // Update the CRC checksum of the memory nvram section.
  //
  MemCfgData->mem.crc16 = 0;
  memNvramPtr = (char *)(&(MemCfgData->mem));
  data16 = Crc16 (memNvramPtr, sizeof(MemCfgData->mem));
  MemCfgData->mem.crc16 = data16;

  //
  // Update the memory configuration variables.
  //
  Status = MemoryConfigVars (FALSE, DataSize, (UINT8 *) (MemCfgData));

  gSmst->SmmFreePool (MemCfgData);

  return EFI_SUCCESS;
}

VOID
IniMemRasS3 (VOID)
{

  EFI_STATUS                      Status;
  EFI_SMM_SX_REGISTER_CONTEXT     *EntryS3DispatchContext;
  EFI_SMM_SX_DISPATCH2_PROTOCOL   *SxDispatch;
  EFI_HANDLE                      SxDispatchHandle;
  UINTN                           Length;

  Length = sizeof ( EFI_SMM_SX_REGISTER_CONTEXT);
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      Length,
                      &EntryS3DispatchContext
                      );
  if (EFI_ERROR (Status))  return ;
  ZeroMem (EntryS3DispatchContext, sizeof (EFI_SMM_SX_REGISTER_CONTEXT));

  Status = gSmst->SmmLocateProtocol (
                &gEfiSmmSxDispatch2ProtocolGuid,
                NULL,
                &SxDispatch
                );
  if (EFI_ERROR (Status))  return;

  //
  // Register the callback for S3 entry
  //
  EntryS3DispatchContext->Type  = SxS3;
  EntryS3DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                                SxDispatch,
                                MemRASS3EntryCallback,
                                EntryS3DispatchContext,
                                &SxDispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);

  mDecompressHandle = NULL;

  Status = gSmst->SmmInstallProtocolInterface (
                      &mDecompressHandle,
                      &gEfiDecompressProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &gEfiDecompress
                      );
  return;
}


