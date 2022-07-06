//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
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
  @brief AMI SMM Communication Mailbox Header File.

  The file contains definitions of the AMI SMM Communication Mailbox data structures,
  as well as API to access the mailbox.
  SMM Communication Mailbox is used by OS applications to access SMM Communication buffer.
**/

#ifndef __AMI_SMM_COMMUNICATION_MAILBOX__H__
#define __AMI_SMM_COMMUNICATION_MAILBOX__H__

/// Defines format of the SMM Communication Mailbox
typedef struct {
    UINT64 Revision; ///< Revision of this data structure
    UINT64 PhysicalAddress; ///< Physical address of the SMM Communication buffer
    UINT32 Status; ///< Status of the last SMI call
    UINT32 Token; ///< Token that identifies SMI caller
} AMI_SMM_COMMUNICATION_MAILBOX;

#define AMI_SMM_COMMUNICATION_MAILBOX_REVISION 1

#define AMI_SMM_COMMUNICATION_MAILBOX_COMMAND_LOCK 0xC0000001
#define AMI_SMM_COMMUNICATION_MAILBOX_COMMAND_UNLOCK 0xC0000002

#define AMI_SMM_COMMUNICATION_MAILBOX_LOCK_FLAG_FORCE_LOCK 1

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
