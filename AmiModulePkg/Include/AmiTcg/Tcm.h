/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  Tcm.h

   Product:    TCM DXE Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009
--*/

#ifndef _TCM_H_
#define _TCM_H_
#define _CRT_SECURE_NO_WARNINGS
//
// Structures are all packed on 1-byte alignment
//

#pragma pack (push)
#pragma pack (1)

//
// Part 2, section 2.2: Basic types & Helper redefinitions
//
typedef UINT8                       TCM_AUTH_DATA_USAGE;
typedef UINT8                       TCM_PAYLOAD_TYPE;
typedef UINT8                       TCM_VERSION_BYTE;
typedef UINT16                      TCM_TAG;
typedef UINT16                      TCM_PROTOCOL_ID;
typedef UINT16                      TCM_STARTUP_TYPE;
typedef UINT16                      TCM_ENC_SCHEME;
typedef UINT16                      TCM_SIG_SCHEME;
typedef UINT16                      TCM_MIGRATE_SCHEME;
typedef UINT16                      TCM_PHYSICAL_PRESENCE;
typedef UINT16                      TCM_ENTITY_TYPE;
typedef UINT16                      TCM_KEY_USAGE;
typedef UINT16                      TCM_EK_TYPE;
typedef UINT16                      TCM_STRUCTURE_TAG;
typedef UINT16                      TCM_PLATFORM_SPECIFIC;
typedef UINT32                      TCM_COMMAND_CODE;
typedef UINT32                      TCM_CAPABILITY_AREA;
typedef UINT32                      TCM_KEY_FLAGS;
typedef UINT32                      TCM_ALGORITHM_ID;
typedef UINT32                      TCM_MODIFIER_INDICATOR;
typedef UINT32                      TCM_ACTUAL_COUNT;
typedef UINT32                      TCM_TRANSPORT_ATTRIBUTES;
typedef UINT32                      TCM_AUTHHANDLE;
typedef UINT32                      TCM_DIRINDEX;
typedef UINT32                      TCM_KEY_HANDLE;
typedef UINT32                      TCM_PCRINDEX;
typedef UINT32                      TCM_RESULT;
typedef UINT32                      TCM_RESOURCE_TYPE;
typedef UINT32                      TCM_KEY_CONTROL;
typedef UINT32                      TCM_NV_INDEX;
typedef UINT32                      TCM_FAMILY_ID;
typedef UINT32                      TCM_FAMILY_VERIFICATION;
typedef UINT32                      TCM_STARTUP_EFFECTS;
typedef UINT32                      TCM_SYM_MODE;
typedef UINT32                      TCM_FAMILY_FLAGS;
typedef UINT32                      TCM_DELEGATE_INDEX;
typedef UINT32                      TCM_CMK_DELEGATE;
typedef UINT32                      TCM_COUNT_ID;
typedef UINT32                      TCM_REDIT_COMMAND;
typedef UINT32                      TCM_TRANSHANDLE;
typedef UINT32                      TCM_HANDLE;
typedef UINT32                      TCM_FAMILY_OPERATION;

//
// Part 2, section 2.2.4: Vendor specific
// The following defines allow for the quick specification of a
// vendor specific item.
//
#define TCM_Vendor_Specific32       ((UINT32) 0x00000400)
#define TCM_Vendor_Specific8        ((UINT8) 0x80)

//
// Part 2, section 3.1: Structure TAGs
//
#define TCM_TAG_CONTEXTBLOB         ((TCM_STRUCTURE_TAG) 0x0001)
#define TCM_TAG_CONTEXT_SENSITIVE   ((TCM_STRUCTURE_TAG) 0x0002)
#define TCM_TAG_CONTEXTPOINTER      ((TCM_STRUCTURE_TAG) 0x0003)
#define TCM_TAG_CONTEXTLIST         ((TCM_STRUCTURE_TAG) 0x0004)
#define TCM_TAG_SIGNINFO            ((TCM_STRUCTURE_TAG) 0x0005)
#define TCM_TAG_PCR_INFO_LONG       ((TCM_STRUCTURE_TAG) 0x0006)
#define TCM_TAG_PERSISTENT_FLAGS    ((TCM_STRUCTURE_TAG) 0x0007)
#define TCM_TAG_VOLATILE_FLAGS      ((TCM_STRUCTURE_TAG) 0x0008)
#define TCM_TAG_PERSISTENT_DATA     ((TCM_STRUCTURE_TAG) 0x0009)
#define TCM_TAG_VOLATILE_DATA       ((TCM_STRUCTURE_TAG) 0x000A)
#define TCM_TAG_SV_DATA             ((TCM_STRUCTURE_TAG) 0x000B)
#define TCM_TAG_EK_BLOB             ((TCM_STRUCTURE_TAG) 0x000C)
#define TCM_TAG_EK_BLOB_AUTH        ((TCM_STRUCTURE_TAG) 0x000D)
#define TCM_TAG_COUNTER_VALUE       ((TCM_STRUCTURE_TAG) 0x000E)
#define TCM_TAG_TRANSPORT_INTERNAL  ((TCM_STRUCTURE_TAG) 0x000F)
#define TCM_TAG_TRANSPORT_LOG_IN    ((TCM_STRUCTURE_TAG) 0x0010)
#define TCM_TAG_TRANSPORT_LOG_OUT   ((TCM_STRUCTURE_TAG) 0x0011)
#define TCM_TAG_AUDIT_EVENT_IN      ((TCM_STRUCTURE_TAG) 0x0012)
#define TCM_TAG_AUDIT_EVENT_OUT     ((TCM_STRUCTURE_TAG) 0x0013)
#define TCM_TAG_CURRENT_TICKS       ((TCM_STRUCTURE_TAG) 0x0014)
#define TCM_TAG_KEY                 ((TCM_STRUCTURE_TAG) 0x0015)
#define TCM_TAG_STORED_DATA12       ((TCM_STRUCTURE_TAG) 0x0016)
#define TCM_TAG_NV_ATTRIBUTES       ((TCM_STRUCTURE_TAG) 0x0017)
#define TCM_TAG_NV_DATA_PUBLIC      ((TCM_STRUCTURE_TAG) 0x0018)
#define TCM_TAG_NV_DATA_SENSITIVE   ((TCM_STRUCTURE_TAG) 0x0019)
#define TCM_TAG_DELEGATIONS         ((TCM_STRUCTURE_TAG) 0x001A)
#define TCM_TAG_DELEGATE_PUBLIC     ((TCM_STRUCTURE_TAG) 0x001B)
#define TCM_TAG_DELEGATE_TABLE_ROW  ((TCM_STRUCTURE_TAG) 0x001C)
#define TCM_TAG_TRANSPORT_AUTH      ((TCM_STRUCTURE_TAG) 0x001D)
#define TCM_TAG_TRANSPORT_PUBLIC    ((TCM_STRUCTURE_TAG) 0x001E)
#define TCM_TAG_PERMANENT_FLAGS     ((TCM_STRUCTURE_TAG) 0x001F)
#define TCM_TAG_STCLEAR_FLAGS       ((TCM_STRUCTURE_TAG) 0x0020)
#define TCM_TAG_STANY_FLAGS         ((TCM_STRUCTURE_TAG) 0x0021)
#define TCM_TAG_PERMANENT_DATA      ((TCM_STRUCTURE_TAG) 0x0022)
#define TCM_TAG_STCLEAR_DATA        ((TCM_STRUCTURE_TAG) 0x0023)
#define TCM_TAG_STANY_DATA          ((TCM_STRUCTURE_TAG) 0x0024)
#define TCM_TAG_FAMILY_TABLE_ENTRY  ((TCM_STRUCTURE_TAG) 0x0025)
#define TCM_TAG_DELEGATE_SENSITIVE  ((TCM_STRUCTURE_TAG) 0x0026)
#define TCM_TAG_DELG_KEY_BLOB       ((TCM_STRUCTURE_TAG) 0x0027)
#define TCM_TAG_KEY12               ((TCM_STRUCTURE_TAG) 0x0028)
#define TCM_TAG_CERTIFY_INFO2       ((TCM_STRUCTURE_TAG) 0x0029)
#define TCM_TAG_DELEGATE_OWNER_BLOB ((TCM_STRUCTURE_TAG) 0x002A)
#define TCM_TAG_EK_BLOB_ACTIVATE    ((TCM_STRUCTURE_TAG) 0x002B)
#define TCM_TAG_DAA_BLOB            ((TCM_STRUCTURE_TAG) 0x002C)
#define TCM_TAG_DAA_CONTEXT         ((TCM_STRUCTURE_TAG) 0x002D)
#define TCM_TAG_DAA_ENFORCE         ((TCM_STRUCTURE_TAG) 0x002E)
#define TCM_TAG_DAA_ISSUER          ((TCM_STRUCTURE_TAG) 0x002F)
#define TCM_TAG_CAP_VERSION_INFO    ((TCM_STRUCTURE_TAG) 0x0030)
#define TCM_TAG_DAA_SENSITIVE       ((TCM_STRUCTURE_TAG) 0x0031)
#define TCM_TAG_DAA_TCM             ((TCM_STRUCTURE_TAG) 0x0032)
#define TCM_TAG_CMK_MIGAUTH         ((TCM_STRUCTURE_TAG) 0x0033)
#define TCM_TAG_CMK_SIGTICKET       ((TCM_STRUCTURE_TAG) 0x0034)
#define TCM_TAG_CMK_MA_APPROVAL     ((TCM_STRUCTURE_TAG) 0x0035)
#define TCM_TAG_QUOTE_INFO2         ((TCM_STRUCTURE_TAG) 0x0036)

//
// Part 2, section 4: TCM Types
//

//
// Part 2, section 4.1: TCM_RESOURCE_TYPE
//
#define TCM_RT_KEY                  ((TCM_RESOURCE_TYPE) 0x00000001) // The handle is a key handle and is the result of a LoadKey type operation
#define TCM_RT_AUTH                 ((TCM_RESOURCE_TYPE) 0x00000002) // The handle is an authorization handle. Auth handles come from TCM_OIAP, TCM_OSAP and TCM_DSAP
#define TCM_RT_HASH                 ((TCM_RESOURCE_TYPE) 0x00000003) // Reserved for hashes
#define TCM_RT_TRANS                ((TCM_RESOURCE_TYPE) 0x00000004) // The handle is for a transport session. Transport handles come from TCM_EstablishTransport
#define TCM_RT_CONTEXT              ((TCM_RESOURCE_TYPE) 0x00000005) // Resource wrapped and held outside the TCM using the context save/restore commands
#define TCM_RT_COUNTER              ((TCM_RESOURCE_TYPE) 0x00000006) // Reserved for counters
#define TCM_RT_DELEGATE             ((TCM_RESOURCE_TYPE) 0x00000007) // The handle is for a delegate row. These are the internal rows held in NV storage by the TCM
#define TCM_RT_DAA_TCM              ((TCM_RESOURCE_TYPE) 0x00000008) // The value is a DAA TCM specific blob
#define TCM_RT_DAA_V0               ((TCM_RESOURCE_TYPE) 0x00000009) // The value is a DAA V0 parameter
#define TCM_RT_DAA_V1               ((TCM_RESOURCE_TYPE) 0x0000000A) // The value is a DAA V1 parameter

//
// Part 2, section 4.2: TCM_PAYLOAD_TYPE
//
#define TCM_PT_ASYM                 ((TCM_PAYLOAD_TYPE) 0x01) // The entity is an asymmetric key
#define TCM_PT_BIND                 ((TCM_PAYLOAD_TYPE) 0x02) // The entity is bound data
#define TCM_PT_MIGRATE              ((TCM_PAYLOAD_TYPE) 0x03) // The entity is a migration blob
#define TCM_PT_MAINT                ((TCM_PAYLOAD_TYPE) 0x04) // The entity is a maintenance blob
#define TCM_PT_SEAL                 ((TCM_PAYLOAD_TYPE) 0x05) // The entity is sealed data
#define TCM_PT_MIGRATE_RESTRICTED   ((TCM_PAYLOAD_TYPE) 0x06) // The entity is a restricted-migration asymmetric key
#define TCM_PT_MIGRATE_EXTERNAL     ((TCM_PAYLOAD_TYPE) 0x07) // The entity is a external migratable key
#define TCM_PT_CMK_MIGRATE          ((TCM_PAYLOAD_TYPE) 0x08) // The entity is a CMK migratable blob
#define TCM_PT_VENDOR_SPECIFIC      ((TCM_PAYLOAD_TYPE) 0x80) // 0x80 - 0xFF Vendor specific payloads

//
// Part 2, section 4.3: TCM_ENTIRY_TYPE
//
#define TCM_ET_KEYHANDLE            ((UINT16) 0x0001) // The entity is a keyHandle or key
#define TCM_ET_OWNER                ((UINT16) 0x0002) // The entity is the TCM Owner
#define TCM_ET_DATA                 ((UINT16) 0x0003) // The entity is some data
#define TCM_ET_SRK                  ((UINT16) 0x0004) // The entity is the SRK
#define TCM_ET_KEY                  ((UINT16) 0x0005) // The entity is a key or keyHandle
#define TCM_ET_REVOKE               ((UINT16) 0x0006) // The entity is the RevokeTrust value
#define TCM_ET_DEL_OWNER_BLOB       ((UINT16) 0x0007) // The entity is a delegate owner blob
#define TCM_ET_DEL_ROW              ((UINT16) 0x0008) // The entity is a delegate row
#define TCM_ET_DEL_KEY_BLOB         ((UINT16) 0x0009) // The entity is a delegate key blob
#define TCM_ET_COUNTER              ((UINT16) 0x000A) // The entity is a counter
#define TCM_ET_NV                   ((UINT16) 0x000B) // The entity is a NV index
#define TCM_ET_RESERVED_HANDLE      ((UINT16) 0x0040) // Reserved. This value avoids collisions with the handle MSB setting.
//
// TCM_ENTITY_TYPE MSB Values: The MSB is used to indicate the ADIP encryption sheme when applicable
//
#define TCM_ET_XOR                  ((UINT16) 0x0000) // ADIP encryption scheme: XOR
#define TCM_ET_AES128               ((UINT16) 0x0006) // ADIP encryption scheme: AES 128 bits

//
// Part 2, section 4.4.1: Reserved Key Handles
//
#define TCM_KH_SRK                  ((TCM_KEY_HANDLE) 0x40000000) // The handle points to the SRK
#define TCM_KH_OWNER                ((TCM_KEY_HANDLE) 0x40000001) // The handle points to the TCM Owner
#define TCM_KH_REVOKE               ((TCM_KEY_HANDLE) 0x40000002) // The handle points to the RevokeTrust value
#define TCM_KH_TRANSPORT            ((TCM_KEY_HANDLE) 0x40000003) // The handle points to the EstablishTransport static authorization
#define TCM_KH_OPERATOR             ((TCM_KEY_HANDLE) 0x40000004) // The handle points to the Operator auth
#define TCM_KH_ADMIN                ((TCM_KEY_HANDLE) 0x40000005) // The handle points to the delegation administration auth
#define TCM_KH_EK                   ((TCM_KEY_HANDLE) 0x40000006) // The handle points to the PUBEK, only usable with TCM_OwnerReadInternalPub

//
// Part 2, section 4.5: TCM_STARTUP_TYPE
//
#define TCM_ST_CLEAR                ((TCM_STARTUP_TYPE) 0x0001) // The TCM is starting up from a clean state
#define TCM_ST_STATE                ((TCM_STARTUP_TYPE) 0x0002) // The TCM is starting up from a saved state
#define TCM_ST_DEACTIVATED          ((TCM_STARTUP_TYPE) 0x0003) // The TCM is to startup and set the deactivated flag to TRUE

//
// Part 2, section 4.6: TCM_STATUP_EFFECTS
// The table makeup is still an open issue.
//

//
// Part 2, section 4.7: TCM_PROTOCOL_ID
//
#define TCM_PID_OIAP                ((TCM_PROTOCOL_ID) 0x0001) // The OIAP protocol.
#define TCM_PID_OSAP                ((TCM_PROTOCOL_ID) 0x0002) // The OSAP protocol.
#define TCM_PID_ADIP                ((TCM_PROTOCOL_ID) 0x0003) // The ADIP protocol.
#define TCM_PID_ADCP                ((TCM_PROTOCOL_ID) 0x0004) // The ADCP protocol.
#define TCM_PID_OWNER               ((TCM_PROTOCOL_ID) 0x0005) // The protocol for taking ownership of a TCM.
#define TCM_PID_DSAP                ((TCM_PROTOCOL_ID) 0x0006) // The DSAP protocol
#define TCM_PID_TRANSPORT           ((TCM_PROTOCOL_ID) 0x0007) // The transport protocol

//
// Part 2, section 4.8: TCM_ALGORITHM_ID
//   The TCM MUST support the algorithms TCM_ALG_RSA, TCM_ALG_SHA, TCM_ALG_HMAC,
//   TCM_ALG_MGF1
//
#define TCM_ALG_RSA                 ((TCM_ALGORITHM_ID) 0x00000001) // The RSA algorithm.
#define TCM_ALG_DES                 ((TCM_ALGORITHM_ID) 0x00000002) // The DES algorithm
#define TCM_ALG_3DES                ((TCM_ALGORITHM_ID) 0x00000003) // The 3DES algorithm in EDE mode
#define TCM_ALG_SHA                 ((TCM_ALGORITHM_ID) 0x00000004) // The SCH algorithm
#define TCM_ALG_HMAC                ((TCM_ALGORITHM_ID) 0x00000005) // The RFC 2104 HMAC algorithm
#define TCM_ALG_AES128              ((TCM_ALGORITHM_ID) 0x00000006) // The AES algorithm, key size 128
#define TCM_ALG_MGF1                ((TCM_ALGORITHM_ID) 0x00000007) // The XOR algorithm using MGF1 to create a string the size of the encrypted block
#define TCM_ALG_AES192              ((TCM_ALGORITHM_ID) 0x00000008) // AES, key size 192
#define TCM_ALG_AES256              ((TCM_ALGORITHM_ID) 0x00000009) // AES, key size 256
#define TCM_ALG_XOR                 ((TCM_ALGORITHM_ID) 0x0000000A) // XOR using the rolling nonces

//
// Part 2, section 4.9: TCM_PHYSICAL_PRESENCE
//
#define TCM_PHYSICAL_PRESENCE_HW_DISABLE    ((TCM_PHYSICAL_PRESENCE) 0x0200) // Sets the physicalPresenceHWEnable to FALSE
#define TCM_PHYSICAL_PRESENCE_CMD_DISABLE   ((TCM_PHYSICAL_PRESENCE) 0x0100) // Sets the physicalPresenceCMDEnable to FALSE
#define TCM_PHYSICAL_PRESENCE_LIFETIME_LOCK ((TCM_PHYSICAL_PRESENCE) 0x0080) // Sets the physicalPresenceLifetimeLock to TRUE
#define TCM_PHYSICAL_PRESENCE_HW_ENABLE     ((TCM_PHYSICAL_PRESENCE) 0x0040) // Sets the physicalPresenceHWEnable to TRUE
#define TCM_PHYSICAL_PRESENCE_CMD_ENABLE    ((TCM_PHYSICAL_PRESENCE) 0x0020) // Sets the physicalPresenceCMDEnable to TRUE
#define TCM_PHYSICAL_PRESENCE_NOTPRESENT    ((TCM_PHYSICAL_PRESENCE) 0x0010) // Sets PhysicalPresence = FALSE
#define TCM_PHYSICAL_PRESENCE_PRESENT       ((TCM_PHYSICAL_PRESENCE) 0x0008) // Sets PhysicalPresence = TRUE
#define TCM_PHYSICAL_PRESENCE_LOCK          ((TCM_PHYSICAL_PRESENCE) 0x0004) // Sets PhysicalPresenceLock = TRUE

//
// Part 2, section 4.10: TCM_MIGRATE_SCHEME
//
#define TCM_MS_MIGRATE                      ((TCM_MIGRATE_SCHEME) 0x0001) // A public key that can be used with all TCM migration commands other than 'ReWrap' mode.
#define TCM_MS_REWRAP                       ((TCM_MIGRATE_SCHEME) 0x0002) // A public key that can be used for the ReWrap mode of TCM_CreateMigrationBlob.
#define TCM_MS_MAINT                        ((TCM_MIGRATE_SCHEME) 0x0003) // A public key that can be used for the Maintenance commands
#define TCM_MS_RESTRICT_MIGRATE             ((TCM_MIGRATE_SCHEME) 0x0004) // The key is to be migrated to a Migration Authority.
#define TCM_MS_RESTRICT_APPROVE_DOUBLE      ((TCM_MIGRATE_SCHEME) 0x0005) // The key is to be migrated to an entity approved by a Migration Authority using double wrapping

//
// Part 2, section 4.11: TCM_EK_TYPE
//
#define TCM_EK_TYPE_ACTIVATE        ((TCM_EK_TYPE) 0x0001) // The blob MUST be TCM_EK_BLOB_ACTIVATE
#define TCM_EK_TYPE_AUTH            ((TCM_EK_TYPE) 0x0002) // The blob MUST be TCM_EK_BLOB_AUTH

//
// Part 2, section 4.12: TCM_PLATFORM_SPECIFIC
//
#define TCM_PS_PC_11                ((TCM_PLATFORM_SPECIFIC) 0x0001) // PC Specific version 1.1
#define TCM_PS_PC_12                ((TCM_PLATFORM_SPECIFIC) 0x0002) // PC Specific version 1.2
#define TCM_PS_PDA_12               ((TCM_PLATFORM_SPECIFIC) 0x0003) // PDA Specific version 1.2
#define TCM_PS_Server_12            ((TCM_PLATFORM_SPECIFIC) 0x0004) // Server Specific version 1.2
#define TCM_PS_Mobile_12            ((TCM_PLATFORM_SPECIFIC) 0x0005) // Mobil Specific version 1.2

//
// Part 2, section 5: Basic Structures
//

//
// Part 2, section 5.1: TCM_STRUCT_VER
//
typedef struct tdTCM_STRUCT_VER {
  UINT8                             major;
  UINT8                             minor;
  UINT8                             revMajor;
  UINT8                             revMinor;
} TCM_STRUCT_VER;

//
// Part 2, section 5.3: TCM_VERSION
//
typedef struct tdTCM_VERSION {
  TCM_VERSION_BYTE                  major;
  TCM_VERSION_BYTE                  minor;
  UINT8                             revMajor;
  UINT8                             revMinor;
} TCM_VERSION;

//
// Part 2, section 5.4: TCM_DIGEST
//
#define TCM_SCH_160_HASH_LEN       0x20
#define TCM_SCHBASED_NONCE_LEN     TCM_SCH_160_HASH_LEN

typedef struct tdTCM_DIGEST{
  UINT8                             digest[TCM_SCH_160_HASH_LEN];
} TCM_DIGEST;

typedef struct TCM_DIGEST_HASH{
  UINT8                             digest[32];
} TCM_DIGEST_HASH;

typedef TCM_DIGEST                  TCM_CHOSENID_HASH; // This SHALL be the digest of the chosen identityLabel and privacyCA for a new TCM identity.
typedef TCM_DIGEST                  TCM_COMPOSITE_HASH; // This SHALL be the hash of a list of PCR indexes and PCR values that a key or data is bound to.
typedef TCM_DIGEST                  TCM_DIRVALUE; // This SHALL be the value of a DIR register
typedef TCM_DIGEST                  TCM_HMAC;
typedef TCM_DIGEST                  TCM_PCRVALUE; // The value inside of the PCR
typedef TCM_DIGEST                  TCM_AUDITDIGEST; // This SHALL be the value of the current internal audit state
typedef TCM_DIGEST                  TCM_DAA_TCM_SEED; // This SHALL be a random value generated by a TCM immediately after the EK is installed in that TCM, whenever an EK is installed in that TCM
typedef TCM_DIGEST                  TCM_DAA_CONTEXT_SEED; // This SHALL be a random value


//
// Register set map as specified in TIS Chapter 10
//
typedef struct tdTCM_PC_REGISTERS {
  INT8                              access;             // 0
  UINT8                             reserved1[7];       // 1
  UINT32                            intEnable;          // 8
  UINT8                             intVector;          // 0ch
  UINT8                             reserved2[3];       // 0dh
  UINT32                            intSts;             // 10h
  UINT32                            intfCapability;     // 14h
  INT8                              status;             // 18h
  UINT16                            burstCount;         // 19h
  UINT8                             reserved3[9];
  UINT32                            dataFifo;           // 24
  UINT8                             reserved4[0xed8];   // 28h
  UINT16                            vid;                // 0f00h
  UINT16                            did;                // 0f02h
  UINT8                             rid;                // 0f04h
  UINT8                             TCMDefined[0x7b];   // 0f05h
  UINT32                            legacyAddress1;     // 0f80h
  UINT32                            legacyAddress1Ex;   // 0f84h
  UINT32                            legacyAddress2;     // 0f88h
  UINT32                            legacyAddress2Ex;   // 0f8ch
  UINT8                             vendorDefined[0x70];// 0f90h
} TCM_PC_REGISTERS;

//
// Define pointer types used to access TIS registers on PC
//
typedef volatile TCM_PC_REGISTERS   *TCM_PC_REGISTERS_PTR;

//
// Part 2, section 5.5: TCM_NONCE
//
typedef struct tdTCM_NONCE{
  UINT8                             nonce[20];
} TCM_NONCE;

//
// Part 2, section 5.6: TCM_AUTHDATA
//
typedef UINT8                       tdTCM_AUTHDATA[20];
typedef tdTCM_AUTHDATA              TCM_AUTHDATA;
typedef TCM_AUTHDATA                TCM_SECRET;
typedef TCM_AUTHDATA                TCM_ENCAUTH;

//
// Part 2, section 5.7: TCM_KEY_HANDLE_LIST
// Size of handle is loaded * sizeof(TCM_KEY_HANDLE)
//
typedef struct tdTCM_KEY_HANDLE_LIST {
  UINT16                            loaded;
  TCM_KEY_HANDLE                    handle[1];
} TCM_KEY_HANDLE_LIST;

//
// Part 2, section 5.8: TCM_KEY_USAGE values
//

#define TCM_KEY_SIGNING             ((UINT16) 0x0010)
// TCM_KEY_SIGNING SHALL indicate a signing key. The [private] key SHALL be
// used for signing operations, only. This means that it MUST be a leaf of the
// Protected Storage key hierarchy.

#define TCM_KEY_STORAGE             ((UINT16) 0x0011)
// TCM_KEY_STORAGE SHALL indicate a storage key. The key SHALL be used to wrap
// and unwrap other keys in the Protected Storage hierarchy

#define TCM_KEY_IDENTITY            ((UINT16) 0x0012)
// TCM_KEY_IDENTITY SHALL indicate an identity key. The key SHALL be used for
// operations that require a TCM identity, only.

#define TCM_KEY_AUTHCHANGE          ((UINT16) 0x0013)
// TCM_KEY_AUTHCHANGE SHALL indicate an ephemeral key that is in use during
// the ChangeAuthAsym process, only.

#define TCM_KEY_BIND                ((UINT16) 0x0014)
// TCM_KEY_BIND SHALL indicate a key that can be used for TCM_Bind and
// TCM_Unbind operations only.

#define TCM_KEY_LEGACY              ((UINT16) 0x0015)
// TCM_KEY_LEGACY SHALL indicate a key that can perform signing and binding
// operations. The key MAY be used for both signing and binding operations.
// The TCM_KEY_LEGACY key type is to allow for use by applications where both
// signing and encryption operations occur with the same key. The use of this
// key type is not recommended TCM_KEY_MIGRATE 0x0016 This SHALL indicate a
// key in use for TCM_MigrateKey

#define TCM_KEY_MIGRATE             ((UINT16) 0x0016)
// TCM_KEY_MIGRAGE SHALL indicate a key in use for TCM_MigrateKey

//
// Part 2, section 5.8.1: Encryption/Signature schemes
//

#define TCM_ES_NONE                 ((TCM_ENC_SCHEME) 0x0001)
#define TCM_ES_RSAESPKCSv15         ((TCM_ENC_SCHEME) 0x0002)
#define TCM_ES_RSAESOAEP_SCH_MGF1  ((TCM_ENC_SCHEME) 0x0003)
#define TCM_ES_SYM_CNT              ((TCM_ENC_SCHEME) 0x0004)
#define TCM_ES_SYM_OFB              ((TCM_ENC_SCHEME) 0x0005)

#define TCM_SS_NONE                 ((TCM_SIG_SCHEME) 0x0001)
#define TCM_SS_RSASSAPKCS1v15_SCH  ((TCM_SIG_SCHEME) 0x0002)
#define TCM_SS_RSASSAPKCS1v15_DER   ((TCM_SIG_SCHEME) 0x0003)
#define TCM_SS_RSASSAPKCS1v15_INFO  ((TCM_SIG_SCHEME) 0x0004)

//
// Part 2, section 5.9: TCM_AUTH_DATA_USAGE values
//
#define TCM_AUTH_NEVER              ((TCM_AUTH_DATA_USAGE) 0x00)
#define TCM_AUTH_ALWAYS             ((TCM_AUTH_DATA_USAGE) 0x01)
#define TCM_AUTH_PRIV_USE_ONLY      ((TCM_AUTH_DATA_USAGE) 0x03)

//
// Part 2, section 5.10: TCM_KEY_FLAGS
//
enum tdTCM_KEY_FLAGS {
  tcmredirection                       = 0x00000001,
  tcmmigratable                        = 0x00000002,
  tcmisVolatile                        = 0x00000004,
  tcmpcrIgnoredOnRead                  = 0x00000008,
  tcmmigrateAuthority                  = 0x00000010
};

//
// Part 2, section 5.11: TCM_CHANGEAUTH_VALIDATE
//
typedef struct tdTCM_CHANGEAUTH_VALIDATE {
  TCM_SECRET                        newAuthSecret;
  TCM_NONCE                         n1;
} TCM_CHANGEAUTH_VALIDATE;

//
// Part 2, section 5.12: TCM_MIGRATIONKEYAUTH
//   decalared after section 10 to catch declaration of TCM_PUBKEY
//
// Part 2 section 10.1: TCM_KEY_PARMS
//   [size_is(parmSize)] BYTE* parms;
//
typedef struct tdTCM_KEY_PARMS {
  TCM_ALGORITHM_ID                  algorithmID;
  TCM_ENC_SCHEME                    encScheme;
  TCM_SIG_SCHEME                    sigScheme;
  UINT32                            parmSize;
  UINT8                             *parms;
} TCM_KEY_PARMS;

//
// Part 2, section 10.4: TCM_STORE_PUBKEY
//
typedef struct tdTCM_STORE_PUBKEY {
  UINT32                            keyLength;
  UINT8                             key[1];
} TCM_STORE_PUBKEY;

//
// Part 2, section 10.5: TCM_PUBKEY
//
typedef struct tdTCM_PUBKEY{
  TCM_KEY_PARMS                     algorithmParms;
  TCM_STORE_PUBKEY                  pubKey;
} TCM_PUBKEY;

//
// Part 2, section 5.12: TCM_MIGRATIONKEYAUTH
//
typedef struct tdTCM_MIGRATIONKEYAUTH{
  TCM_PUBKEY                        migrationKey;
  TCM_MIGRATE_SCHEME                migrationScheme;
  TCM_DIGEST                        digest;
} TCM_MIGRATIONKEYAUTH;

//
// Part 2, section 5.13: TCM_COUNTER_VALUE
//
typedef struct tdTCM_COUNTER_VALUE{
  TCM_STRUCTURE_TAG                 tag;
  UINT8                             label[4];
  TCM_ACTUAL_COUNT                  counter;
} TCM_COUNTER_VALUE;

//
// Part 2, section 5.14: TCM_SIGN_INFO
//   Size of data indicated by dataLen
//
typedef struct tdTCM_SIGN_INFO {
  TCM_STRUCTURE_TAG                 tag;
  UINT8                             fixed[4];
  TCM_NONCE                         replay;
  UINT32                            dataLen;
  UINT8                             *data;
} TCM_SIGN_INFO;

//
// Part 2, section 5.15: TCM_MSA_COMPOSITE
//   Number of migAuthDigest indicated by MSAlist
//
typedef struct tdTCM_MSA_COMPOSITE {
  UINT32                            MSAlist;
  TCM_DIGEST                        migAuthDigest[1];
} TCM_MSA_COMPOSITE;

//
// Part 2, section 5.16: TCM_CMK_AUTH
//
typedef struct tdTCM_CMK_AUTH{
  TCM_DIGEST                        migrationAuthorityDigest;
  TCM_DIGEST                        destinationKeyDigest;
  TCM_DIGEST                        sourceKeyDigest;
} TCM_CMK_AUTH;

//
// Part 2, section 5.17: TCM_CMK_DELEGATE
//
#define TCM_CMK_DELEGATE_SIGNING    (((TCM_CMK_DELEGATE)1) << 31)
#define TCM_CMK_DELEGATE_STORAGE    (((TCM_CMK_DELEGATE)1) << 30)
#define TCM_CMK_DELEGATE_BIND       (((TCM_CMK_DELEGATE)1) << 29)
#define TCM_CMK_DELEGATE_LEGACY     (((TCM_CMK_DELEGATE)1) << 28)
#define TCM_CMK_DELEGATE_MIGRATE    (((TCM_CMK_DELEGATE)1) << 27)

//
// Part 2, section 5.18: TCM_SELECT_SIZE
//
typedef struct tdTCM_SELECT_SIZE {
  UINT8                             major;
  UINT8                             minor;
  UINT16                            reqSize;
} TCM_SELECT_SIZE;

//
// Part 2, section 5,19: TCM_CMK_MIGAUTH
//
typedef struct tdTCM_CMK_MIGAUTH{
  TCM_STRUCTURE_TAG                 tag;
  TCM_DIGEST                        msaDigest;
  TCM_DIGEST                        pubKeyDigest;
} TCM_CMK_MIGAUTH;

//
// Part 2, section 5.20: TCM_CMK_SIGTICKET
//
typedef struct tdTCM_CMK_SIGTICKET{
  TCM_STRUCTURE_TAG                 tag;
  TCM_DIGEST                        verKeyDigest;
  TCM_DIGEST                        signedData;
} TCM_CMK_SIGTICKET;

//
// Part 2, section 5.21: TCM_CMK_MA_APPROVAL
//
typedef struct tdTCM_CMK_MA_APPROVAL{
  TCM_STRUCTURE_TAG                 tag;
  TCM_DIGEST                        migrationAuthorityDigest;
} TCM_CMK_MA_APPROVAL;

//
// Part 2, section 6: Command Tags
//
#define TCM_TAG_RQU_COMMAND         ((TCM_STRUCTURE_TAG) 0x00C1)
#define TCM_TAG_RQU_AUTH1_COMMAND   ((TCM_STRUCTURE_TAG) 0x00C2)
#define TCM_TAG_RQU_AUTH2_COMMAND   ((TCM_STRUCTURE_TAG) 0x00C3)
#define TCM_TAG_RSP_COMMAND         ((TCM_STRUCTURE_TAG) 0x00C4)
#define TCM_TAG_RSP_AUTH1_COMMAND   ((TCM_STRUCTURE_TAG) 0x00C5)
#define TCM_TAG_RSP_AUTH2_COMMAND   ((TCM_STRUCTURE_TAG) 0x00C6)

//
// Part 2, section 7.1: TCM_PERMANENT_FLAGS
//
typedef struct tdTCM_PERMANENT_FLAGS{
  TCM_STRUCTURE_TAG                 tag;
  BOOLEAN                           disable;
  BOOLEAN                           ownership;
  BOOLEAN                           deactivated;
  BOOLEAN                           readPubek;
  BOOLEAN                           disableOwnerClear;
  BOOLEAN                           allowMaintenance;
  BOOLEAN                           physicalPresenceLifetimeLock;
  BOOLEAN                           physicalPresenceHWEnable;
  BOOLEAN                           physicalPresenceCMDEnable;
  BOOLEAN                           CEKPUsed;
  BOOLEAN                           TCMpost;
  BOOLEAN                           TCMpostLock;
  BOOLEAN                           FIPS;
  BOOLEAN                           operator;
  BOOLEAN                           enableRevokeEK;
  BOOLEAN                           nvLocked;
  BOOLEAN                           readSRKPub;
  BOOLEAN                           TCMEstablished;
  BOOLEAN                           maintenanceDone;
} TCM_PERMANENT_FLAGS;

//
// Part 2, section 7.1.1: PERMANENT_FLAGS Subcap for SetCapability
//
#define TCM_PF_DISABLE                      ((TCM_CAPABILITY_AREA) 1)
#define TCM_PF_OWNERSHIP                    ((TCM_CAPABILITY_AREA) 2)
#define TCM_PF_DEACTIVATED                  ((TCM_CAPABILITY_AREA) 3)
#define TCM_PF_READPUBEK                    ((TCM_CAPABILITY_AREA) 4)
#define TCM_PF_DISABLEOWNERCLEAR            ((TCM_CAPABILITY_AREA) 5)
#define TCM_PF_ALLOWMAINTENANCE             ((TCM_CAPABILITY_AREA) 6)
#define TCM_PF_PHYSICALPRESENCELIFETIMELOCK ((TCM_CAPABILITY_AREA) 7)
#define TCM_PF_PHYSICALPRESENCEHWENABLE     ((TCM_CAPABILITY_AREA) 8)
#define TCM_PF_PHYSICALPRESENCECMDENABLE    ((TCM_CAPABILITY_AREA) 9)
#define TCM_PF_CEKPUSED                     ((TCM_CAPABILITY_AREA) 10)
#define TCM_PF_TCMPOST                      ((TCM_CAPABILITY_AREA) 11)
#define TCM_PF_TCMPOSTLOCK                  ((TCM_CAPABILITY_AREA) 12)
#define TCM_PF_FIPS                         ((TCM_CAPABILITY_AREA) 13)
#define TCM_PF_OPERATOR                     ((TCM_CAPABILITY_AREA) 14)
#define TCM_PF_ENABLEREVOKEEK               ((TCM_CAPABILITY_AREA) 15)
#define TCM_PF_NV_LOCKED                    ((TCM_CAPABILITY_AREA) 16)
#define TCM_PF_READSRKPUB                   ((TCM_CAPABILITY_AREA) 17)
#define TCM_PF_TCMESTABLISHED               ((TCM_CAPABILITY_AREA) 18)
#define TCM_PF_MAINTENANCEDONE              ((TCM_CAPABILITY_AREA) 19)

//
// Part 2, section 7.2: TCM_STCLEAR_FLAGS
//
typedef struct tdTCM_STCLEAR_FLAGS{
  TCM_STRUCTURE_TAG                 tag;
  BOOLEAN                           deactivated;
  BOOLEAN                           disableForceClear;
  BOOLEAN                           physicalPresence;
  BOOLEAN                           physicalPresenceLock;
  BOOLEAN                           bGlobalLock;
} TCM_STCLEAR_FLAGS;

//
// Part 2, section 7.2.1: STCLEAR_FLAGS Subcap for SetCapability
//
#define TCM_SF_DEACTIVATED          ((TCM_CAPABILITY_AREA) 1)
#define TCM_SF_DISABLEFORCECLEAR    ((TCM_CAPABILITY_AREA) 2)
#define TCM_SF_PHYSICALPRESENCE     ((TCM_CAPABILITY_AREA) 3)
#define TCM_SF_PHYSICALPRESENCELOCK ((TCM_CAPABILITY_AREA) 4)
#define TCM_SF_BGLOBALLOCK          ((TCM_CAPABILITY_AREA) 5)

//
// Part 2, section 7.3: TCM_STANY_FLAGS
//
typedef struct tdTCM_STANY_FLAGS{
  TCM_STRUCTURE_TAG                 tag;
  BOOLEAN                           postInitialise;
  TCM_MODIFIER_INDICATOR            localityModifier;
  BOOLEAN                           transportExclusive;
  BOOLEAN                           TOSPresent;
} TCM_STANY_FLAGS;

//
// Part 2, section 7.3.1: STANY_FLAGS Subcap for SetCapability
//
#define TCM_AF_POSTINITIALISE       ((TCM_CAPABILITY_AREA) 1)
#define TCM_AF_LOCALITYMODIFIER     ((TCM_CAPABILITY_AREA) 2)
#define TCM_AF_TRANSPORTEXCLUSIVE   ((TCM_CAPABILITY_AREA) 3)
#define TCM_AF_TOSPRESENT           ((TCM_CAPABILITY_AREA) 4)

//
// All those structures (section 7.4, 7.5, 7.6) are not normative and 
// thus no definitions here
//
// Part 2, section 7.4: TCM_PERMANENT_DATA
//
//#define TCM_MIN_COUNTERS            4   // the minimum number of counters is 4
//#define TCM_DELEGATE_KEY            TCM_KEY
//#define TCM_NUM_PCR                 16
//#define TCM_MAX_NV_WRITE_NOOWNER    64
//typedef struct tdTCM_PERMANENT_DATA
//{
//    TCM_STRUCTURE_TAG               tag;
//    UINT8                           revMajor;
//    UINT8                           revMinor;
//    TCM_NONCE                       TCMProof;
//    TCM_NONCE                       ekReset;
//    TCM_SECRET                      ownerAuth;
//    TCM_SECRET                      operatorAuth;
//    TCM_DIRVALUE                    authDIR[1];
//    TCM_PUBKEY                      manuMaintPub;
//    TCM_KEY                         endorsementKey;
//    TCM_KEY                         srk;
//    TCM_KEY                         contextKey;
//    TCM_KEY                         delegateKey;
//    TCM_COUNTER_VALUE               auditMonotonicCounter;
//    TCM_COUNTER_VALUE               monitonicCounter[TCM_MIN_COUNTERS];
//    TCM_PCR_ATTRIBUTES              pcrAttrib[TCM_NUM_PCR];
//    UINT8                           ordinalAuditStatus[];
//    UINT8                           *rngState;
//    TCM_FAMILY_TABLE                familyTable;
//    TCM_DELEGATE_TABLE              delegateTable;
//    UINT32                          maxNVBufSize;
//    UINT32                          lastFamilyID;
//    UINT32                          noOwnerNVWrite;
//    TCM_CMK_DELEGATE                restrictDelegate;
//    TCM_DAA_TCM_SEED                TCMDAASeed;
//} TCM_PERMANENT_DATA;

//
// Part 2, section 7.5: TCM_STCLEAR_DATA
//   available inside TCM only
//
//typedef struct tdTCM_STCLEAR_DATA
//{
//    TCM_STRUCTURE_TAG               tag;
//    TCM_NONCE                       contextNonceKey;
//    TCM_COUNT_ID                    countID;
//    UINT32                          ownerReference;
//    BOOLEAN                         disableResetLock;
//} TCM_STCLEAR_DATA;

//
// Part 2, section 7.6: TCM_STANY_DATA
//   available inside TCM only
//
//typedef struct tdTCM_STANY_DATA
//{
//    TCM_STRUCTURE_TAG               tag;
//    TCM_NONCE                       contextNonceSession;
//    TCM_DIGEST                      auditDigest;
//    TCM_CURRENT_TICKS               currentTicks;
//    UINT32                          contextCount;
//    UINT32                          contextList[TCM_MIN_SESSION_LIST];
//    TCM_SESSION_DATA                sessions[TCM_MIN_SESSIONS];
//} TCM_STANY_DATA;

//
// Part 2, section 8: PCR Structures
// 

//
// Part 2, section 8.1: TCM_PCR_SELECTION
//   Size of pcrSelect[] indicated by sizeOfSelect
//
typedef struct tdTCM_PCR_SELECTION {
  UINT16                            sizeOfSelect;
  UINT8                             pcrSelect[1];
} TCM_PCR_SELECTION;

//
// Part 2, section 8.2: TCM_PCR_COMPOSITE
//   Size of pcrValue[] indicated by valueSize
//
typedef struct tdTCM_PCR_COMPOSITE {
  TCM_PCR_SELECTION                 select;
  UINT32                            valueSize;
  TCM_PCRVALUE                      pcrValue[1];
} TCM_PCR_COMPOSITE;

//
// Part 2, section 8.3: TCM_PCR_INFO
//
typedef struct tdTCM_PCR_INFO {
  TCM_PCR_SELECTION                 pcrSelection;
  TCM_COMPOSITE_HASH                digestAtRelease;
  TCM_COMPOSITE_HASH                digestAtCreation;
} TCM_PCR_INFO;

//
// Part 2, section 8.6: TCM_LOCALITY_SELECTION
//
typedef UINT8                       TCM_LOCALITY_SELECTION;

#define TCM_LOC_FOUR                ((UINT8) 0x10)
#define TCM_LOC_THREE               ((UINT8) 0x08)
#define TCM_LOC_TWO                 ((UINT8) 0x04)
#define TCM_LOC_ONE                 ((UINT8) 0x02)
#define TCM_LOC_ZERO                ((UINT8) 0x01)

//
// Part 2, section 8.4: TCM_PCR_INFO_LONG
//
typedef struct tdTCM_PCR_INFO_LONG {
  TCM_STRUCTURE_TAG                 tag;
  TCM_LOCALITY_SELECTION            localityAtCreation;
  TCM_LOCALITY_SELECTION            localityAtRelease;
  TCM_PCR_SELECTION                 creationPCRSelection;
  TCM_PCR_SELECTION                 releasePCRSelection;
  TCM_COMPOSITE_HASH                digestAtCreation;
  TCM_COMPOSITE_HASH                digestAtRelease;
} TCM_PCR_INFO_LONG;

//
// Part 2, section 8.5: TCM_PCR_INFO_SHORT
//
typedef struct tdTCM_PCR_INFO_SHORT{
  TCM_PCR_SELECTION                 pcrSelection;
  TCM_LOCALITY_SELECTION            localityAtRelease;
  TCM_COMPOSITE_HASH                digestAtRelease;
} TCM_PCR_INFO_SHORT;

//
// Part 2, section 8.8: TCM_PCR_ATTRIBUTES
//
typedef struct tdTCM_PCR_ATTRIBUTES{
  BOOLEAN                           pcrReset;
  TCM_LOCALITY_SELECTION            pcrExtendLocal;
  TCM_LOCALITY_SELECTION            pcrResetLocal;
} TCM_PCR_ATTRIBUTES;

//
// Part 2, section 9: Storage Structures
//

//
// Part 2, section 9.1: TCM_STORED_DATA
//   [size_is(sealInfoSize)] BYTE* sealInfo;
//   [size_is(encDataSize)] BYTE* encData;
//
typedef struct tdTCM_STORED_DATA {
  TCM_STRUCT_VER                    ver;
  UINT32                            sealInfoSize;
  UINT8                             *sealInfo;
  UINT32                            encDataSize;
  UINT8                             *encData;
} TCM_STORED_DATA;

//
// Part 2, section 9.2: TCM_STORED_DATA12
//   [size_is(sealInfoSize)] BYTE* sealInfo;
//   [size_is(encDataSize)] BYTE* encData;
//
typedef struct tdTCM_STORED_DATA12 {
  TCM_STRUCTURE_TAG                 tag;
  TCM_ENTITY_TYPE                   et;
  UINT32                            sealInfoSize;
  UINT8                             *sealInfo;
  UINT32                            encDataSize;
  UINT8                             *encData;
} TCM_STORED_DATA12;

//
// Part 2, section 9.3: TCM_SEALED_DATA
//   [size_is(dataSize)] BYTE* data;
//
typedef struct tdTCM_SEALED_DATA {
  TCM_PAYLOAD_TYPE                  payload;
  TCM_SECRET                        authData;
  TCM_NONCE                         TCMProof;
  TCM_DIGEST                        storedDigest;
  UINT32                            dataSize;
  UINT8                             *data;
} TCM_SEALED_DATA;

//
// Part 2, section 9.4: TCM_SYMMETRIC_KEY
//   [size_is(size)] BYTE* data;
//
typedef struct tdTCM_SYMMETRIC_KEY {
  TCM_ALGORITHM_ID                  algId;
  TCM_ENC_SCHEME                    encScheme;
  UINT16                            dataSize;
  UINT8                             *data;
} TCM_SYMMETRIC_KEY;

//
// Part 2, section 9.5: TCM_BOUND_DATA
//
typedef struct tdTCM_BOUND_DATA {
  TCM_STRUCT_VER                    ver;
  TCM_PAYLOAD_TYPE                  payload;
  UINT8                             payloadData[1];
} TCM_BOUND_DATA;

//
// Part 2 section 10: TCM_KEY complex
// 

//
// Part 2, section 10.2: TCM_KEY
//   [size_is(encDataSize)] BYTE* encData;
//
typedef struct tdTCM_KEY{
  TCM_STRUCT_VER                    ver;
  TCM_KEY_USAGE                     keyUsage;
  TCM_KEY_FLAGS                     keyFlags;
  TCM_AUTH_DATA_USAGE               authDataUsage;
  TCM_KEY_PARMS                     algorithmParms;
  UINT32                            PCRInfoSize;
  UINT8                             *PCRInfo;
  TCM_STORE_PUBKEY                  pubKey;
  UINT32                            encDataSize;
  UINT8                             *encData;
} TCM_KEY;

//
// Part 2, section 10.3: TCM_KEY12
//   [size_is(encDataSize)] BYTE* encData;
//
typedef struct tdTCM_KEY12{
  TCM_STRUCTURE_TAG                 tag;
  UINT16                            fill;
  TCM_KEY_USAGE                     keyUsage;
  TCM_KEY_FLAGS                     keyFlags;
  TCM_AUTH_DATA_USAGE               authDataUsage;
  TCM_KEY_PARMS                     algorithmParms;
  UINT32                            PCRInfoSize;
  UINT8                             *PCRInfo;
  TCM_STORE_PUBKEY                  pubKey;
  UINT32                            encDataSize;
  UINT8                             *encData;
} TCM_KEY12;

//
// Part 2, section 10.7: TCM_STORE_PRIVKEY
//   [size_is(keyLength)] BYTE* key;
//
typedef struct tdTCM_STORE_PRIVKEY {
  UINT32                            keyLength;
  UINT8                             *key;
} TCM_STORE_PRIVKEY;

//
// Part 2, section 10.6: TCM_STORE_ASYMKEY
//
typedef struct tdTCM_STORE_ASYMKEY {                // pos len total
  TCM_PAYLOAD_TYPE                  payload;        // 0    1   1
  TCM_SECRET                        usageAuth;      // 1    20  21
  TCM_SECRET                        migrationAuth;  // 21   20  41
  TCM_DIGEST                        pubDataDigest;  // 41   20  61
  TCM_STORE_PRIVKEY                 privKey;        // 61 132-151 193-214
} TCM_STORE_ASYMKEY;

//
// Part 2, section 10.8: TCM_MIGRATE_ASYMKEY
//   [size_is(partPrivKeyLen)] BYTE* partPrivKey;
//
typedef struct tdTCM_MIGRATE_ASYMKEY {              // pos  len  total
  TCM_PAYLOAD_TYPE                  payload;        //   0    1       1
  TCM_SECRET                        usageAuth;      //   1   20      21
  TCM_DIGEST                        pubDataDigest;  //  21   20      41
  UINT32                            partPrivKeyLen; //  41    4      45
  UINT8                             *partPrivKey;   //  45 112-127 157-172
} TCM_MIGRATE_ASYMKEY;

//
// Part 2, section 10.9: TCM_KEY_CONTROL
//
#define TCM_KEY_CONTROL_OWNER_EVICT ((UINT32) 0x00000001)

//
// Part 2, section 11: Signed Structures
//

typedef struct tdTCM_CERTIFY_INFO                           
{
    TCM_STRUCT_VER                  version;
    TCM_KEY_USAGE                   keyUsage;
    TCM_KEY_FLAGS                   keyFlags;
    TCM_AUTH_DATA_USAGE             authDataUsage;
    TCM_KEY_PARMS                   algorithmParms;
    TCM_DIGEST                      pubkeyDigest;
    TCM_NONCE                       data;
    BOOLEAN                         parentPCRStatus;
    UINT32                          PCRInfoSize;
    UINT8                           *PCRInfo;
} TCM_CERTIFY_INFO;

typedef struct tdTCM_CERTIFY_INFO2
{
    TCM_STRUCTURE_TAG               tag;
    UINT8                           fill;
    TCM_PAYLOAD_TYPE                payloadType;
    TCM_KEY_USAGE                   keyUsage;
    TCM_KEY_FLAGS                   keyFlags;
    TCM_AUTH_DATA_USAGE             authDataUsage;
    TCM_KEY_PARMS                   algorithmParms;
    TCM_DIGEST                      pubkeyDigest;
    TCM_NONCE                       data;
    BOOLEAN                         parentPCRStatus;
    UINT32                          PCRInfoSize;
    UINT8                           *PCRInfo;
    UINT32                          migrationAuthoritySize;
    UINT8                           *migrationAuthority;
} TCM_CERTIFY_INFO2;

typedef struct tdTCM_QUOTE_INFO                            
{
    TCM_STRUCT_VER                  version;
    UINT8                           fixed[4];
    TCM_COMPOSITE_HASH              digestValue;
    TCM_NONCE                       externalData;
} TCM_QUOTE_INFO;

typedef struct tdTCM_QUOTE_INFO2
{
    TCM_STRUCTURE_TAG               tag;
    UINT8                           fixed[4];
    TCM_NONCE                       externalData;
    TCM_PCR_INFO_SHORT              infoShort;
} TCM_QUOTE_INFO2;

//
// Part 2, section 12: Identity Structures
//

typedef struct tdTCM_EK_BLOB
{
    TCM_STRUCTURE_TAG               tag;
    TCM_EK_TYPE                     ekType;
    UINT32                          blobSize;
    UINT8                           *blob;
} TCM_EK_BLOB;

typedef struct tdTCM_EK_BLOB_ACTIVATE
{
    TCM_STRUCTURE_TAG               tag;
    TCM_SYMMETRIC_KEY               sessionKey;
    TCM_DIGEST                      idDigest;
    TCM_PCR_INFO_SHORT              pcrInfo;
} TCM_EK_BLOB_ACTIVATE;

typedef struct tdTCM_EK_BLOB_AUTH
{
    TCM_STRUCTURE_TAG               tag;
    TCM_SECRET                      authValue;
} TCM_EK_BLOB_AUTH;


// TCM_CHOSENID_HASH = SHA(identityLabel || privacyCA)
typedef TCM_DIGEST                  TCM_CHOSENID_HASH;

typedef struct tdTCM_IDENTITY_CONTENTS
{
    TCM_STRUCT_VER                  ver;
    UINT32                          ordinal;
    TCM_CHOSENID_HASH               labelPrivCADigest;
    TCM_PUBKEY                      identityPubKey;
} TCM_IDENTITY_CONTENTS;

typedef struct tdTCM_IDENTITY_REQ                          
{
    UINT32                          asymSize;
    UINT32                          symSize;
    TCM_KEY_PARMS                   asymAlgorithm;
    TCM_KEY_PARMS                   symAlgorithm;
    UINT8                           *asymBlob;
    UINT8                           *symBlob;
} TCM_IDENTITY_REQ;

typedef struct tdTCM_IDENTITY_PROOF                        
{
    TCM_STRUCT_VER                  ver;
    UINT32                          labelSize;
    UINT32                          identityBindingSize;
    UINT32                          endorsementSize;
    UINT32                          platformSize;
    UINT32                          conformanceSize;
    TCM_PUBKEY                      identityKey;
    UINT8                           *labelArea;
    UINT8                           *identityBinding;
    UINT8                           *endorsementCredential;
    UINT8                           *platformCredential;
    UINT8                           *conformanceCredential;
} TCM_IDENTITY_PROOF;

typedef struct tdTCM_ASYM_CA_CONTENTS                      
{
    TCM_SYMMETRIC_KEY               sessionKey;
    TCM_DIGEST                      idDigest;
} TCM_ASYM_CA_CONTENTS;

typedef struct tdTCM_SYM_CA_ATTESTATION
{
    UINT32                          credSize;
    TCM_KEY_PARMS                   algorithm;
    UINT8                           *credential;
} TCM_SYM_CA_ATTESTATION;

//
// Part 2, section 15: TCM_CURRENT_TICKS
//   Placed here out of order because definitions are used in section 13.
//
typedef struct tdTCM_CURRENT_TICKS {
  TCM_STRUCTURE_TAG                 tag;
  UINT64                            currentTicks;
  UINT16                            tickRate;
  TCM_NONCE                         tickNonce;
} TCM_CURRENT_TICKS;

//
// Part 2, section 13: Transport structures
//

#define TCM_TRANSPORT_ENCRYPT       ((UINT32)0x00000001)
#define TCM_TRANSPORT_LOG           ((UINT32)0x00000002)
#define TCM_TRANSPORT_EXCLUSIVE     ((UINT32)0x00000004)

typedef struct tdTCM_TRANSPORT_PUBLIC
{
    TCM_STRUCTURE_TAG               tag;
    TCM_TRANSPORT_ATTRIBUTES        transAttributes;
    TCM_ALGORITHM_ID                algId;
    TCM_ENC_SCHEME                  encScheme;
} TCM_TRANSPORT_PUBLIC;

typedef struct tdTCM_TRANSPORT_INTERNAL
{
    TCM_STRUCTURE_TAG               tag;
    TCM_AUTHDATA                    authData;
    TCM_TRANSPORT_PUBLIC            transPublic;
    TCM_TRANSHANDLE                 transHandle;
    TCM_NONCE                       transNonceEven;
    TCM_DIGEST                      transDigest;
} TCM_TRANSPORT_INTERNAL;

typedef struct tdTCM_TRANSPORT_LOG_IN
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DIGEST                      parameters;
    TCM_DIGEST                      pubKeyHash;
} TCM_TRANSPORT_LOG_IN;

typedef struct tdTCM_TRANSPORT_LOG_OUT
{
    TCM_STRUCTURE_TAG               tag;
    TCM_CURRENT_TICKS               currentTicks;
    TCM_DIGEST                      parameters;
    TCM_MODIFIER_INDICATOR          locality;
} TCM_TRANSPORT_LOG_OUT;

typedef struct tdTCM_TRANSPORT_AUTH
{
    TCM_STRUCTURE_TAG               tag;
    TCM_AUTHDATA                    authData;
} TCM_TRANSPORT_AUTH;

//
// Part 2, section 14: Audit Structures
//

typedef struct tdTCM_AUDIT_EVENT_IN
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DIGEST                      inputParms;
    TCM_COUNTER_VALUE               auditCount;
} TCM_AUDIT_EVENT_IN;

typedef struct tdTCM_AUDIT_EVENT_OUT
{
    TCM_STRUCTURE_TAG               tag;
    TCM_COMMAND_CODE                ordinal;
    TCM_DIGEST                      outputParms;
    TCM_COUNTER_VALUE               auditCount;
    TCM_RESULT                      returnCode;
} TCM_AUDIT_EVENT_OUT;

//
// Part 2, section 16: Return Codes
//
#define TCM_BASE                    (EFI_MAX_BIT + (EFI_MAX_BIT >> 1))

#ifndef TCM_BASE
#error "TCM Error Codes require definition of TCM_BASE"
#endif

#define TCM_VENDOR_ERROR            TCM_Vendor_Specific32
#define TCM_NON_FATAL               0x00000800

#define TCM_SUCCESS                 ((TCM_RESULT) TCM_BASE)
#define TCM_AUTHFAIL                ((TCM_RESULT) (TCM_BASE + 1))
#define TCM_BADINDEX                ((TCM_RESULT) (TCM_BASE + 2))
#define TCM_BAD_PARAMETER           ((TCM_RESULT) (TCM_BASE + 3))
#define TCM_AUDITFAILURE            ((TCM_RESULT) (TCM_BASE + 4))
#define TCM_CLEAR_DISABLED          ((TCM_RESULT) (TCM_BASE + 5))
#define TCM_DEACTIVATED             ((TCM_RESULT) (TCM_BASE + 6))
#define TCM_DISABLED                ((TCM_RESULT) (TCM_BASE + 7))
#define TCM_DISABLED_CMD            ((TCM_RESULT) (TCM_BASE + 8))
#define TCM_FAIL                    ((TCM_RESULT) (TCM_BASE + 9))
#define TCM_BAD_ORDINAL             ((TCM_RESULT) (TCM_BASE + 10))
#define TCM_INSTALL_DISABLED        ((TCM_RESULT) (TCM_BASE + 11))
#define TCM_INVALID_KEYHANDLE       ((TCM_RESULT) (TCM_BASE + 12))
#define TCM_KEYNOTFOUND             ((TCM_RESULT) (TCM_BASE + 13))
#define TCM_INAPPROPRIATE_ENC       ((TCM_RESULT) (TCM_BASE + 14))
#define TCM_MIGRATEFAIL             ((TCM_RESULT) (TCM_BASE + 15))
#define TCM_INVALID_PCR_INFO        ((TCM_RESULT) (TCM_BASE + 16))
#define TCM_NOSPACE                 ((TCM_RESULT) (TCM_BASE + 17))
#define TCM_NOSRK                   ((TCM_RESULT) (TCM_BASE + 18))
#define TCM_NOTSEALED_BLOB          ((TCM_RESULT) (TCM_BASE + 19))
#define TCM_OWNER_SET               ((TCM_RESULT) (TCM_BASE + 20))
#define TCM_RESOURCES               ((TCM_RESULT) (TCM_BASE + 21))
#define TCM_SHORTRANDOM             ((TCM_RESULT) (TCM_BASE + 22))
#define TCM_SIZE                    ((TCM_RESULT) (TCM_BASE + 23))
#define TCM_WRONGPCRVAL             ((TCM_RESULT) (TCM_BASE + 24))
#define TCM_BAD_PARAM_SIZE          ((TCM_RESULT) (TCM_BASE + 25))
#define TCM_SHA_THREAD              ((TCM_RESULT) (TCM_BASE + 26))
#define TCM_SHA_ERROR               ((TCM_RESULT) (TCM_BASE + 27))
#define TCM_FAILEDSELFTEST          ((TCM_RESULT) (TCM_BASE + 28))
#define TCM_AUTH2FAIL               ((TCM_RESULT) (TCM_BASE + 29))
#define TCM_BADTAG                  ((TCM_RESULT) (TCM_BASE + 30))
#define TCM_IOERROR                 ((TCM_RESULT) (TCM_BASE + 31))
#define TCM_ENCRYPT_ERROR           ((TCM_RESULT) (TCM_BASE + 32))
#define TCM_DECRYPT_ERROR           ((TCM_RESULT) (TCM_BASE + 33))
#define TCM_INVALID_AUTHHANDLE      ((TCM_RESULT) (TCM_BASE + 34))
#define TCM_NO_ENDORSEMENT          ((TCM_RESULT) (TCM_BASE + 35))
#define TCM_INVALID_KEYUSAGE        ((TCM_RESULT) (TCM_BASE + 36))
#define TCM_WRONG_ENTITYTYPE        ((TCM_RESULT) (TCM_BASE + 37))
#define TCM_INVALID_POSTINIT        ((TCM_RESULT) (TCM_BASE + 38))
#define TCM_INAPPROPRIATE_SIG       ((TCM_RESULT) (TCM_BASE + 39))
#define TCM_BAD_KEY_PROPERTY        ((TCM_RESULT) (TCM_BASE + 40))
#define TCM_BAD_MIGRATION           ((TCM_RESULT) (TCM_BASE + 41))
#define TCM_BAD_SCHEME              ((TCM_RESULT) (TCM_BASE + 42))
#define TCM_BAD_DATASIZE            ((TCM_RESULT) (TCM_BASE + 43))
#define TCM_BAD_MODE                ((TCM_RESULT) (TCM_BASE + 44))
#define TCM_BAD_PRESENCE            ((TCM_RESULT) (TCM_BASE + 45))
#define TCM_BAD_VERSION             ((TCM_RESULT) (TCM_BASE + 46))
#define TCM_NO_WRAP_TRANSPORT       ((TCM_RESULT) (TCM_BASE + 47))
#define TCM_AUDITFAIL_UNSUCCESSFUL  ((TCM_RESULT) (TCM_BASE + 48))
#define TCM_AUDITFAIL_SUCCESSFUL    ((TCM_RESULT) (TCM_BASE + 49))
#define TCM_NOTRESETABLE            ((TCM_RESULT) (TCM_BASE + 50))
#define TCM_NOTLOCAL                ((TCM_RESULT) (TCM_BASE + 51))
#define TCM_BAD_TYPE                ((TCM_RESULT) (TCM_BASE + 52))
#define TCM_INVALID_RESOURCE        ((TCM_RESULT) (TCM_BASE + 53))
#define TCM_NOTFIPS                 ((TCM_RESULT) (TCM_BASE + 54))
#define TCM_INVALID_FAMILY          ((TCM_RESULT) (TCM_BASE + 55))
#define TCM_NO_NV_PERMISSION        ((TCM_RESULT) (TCM_BASE + 56))
#define TCM_REQUIRES_SIGN           ((TCM_RESULT) (TCM_BASE + 57))
#define TCM_KEY_NOTSUPPORTED        ((TCM_RESULT) (TCM_BASE + 58))
#define TCM_AUTH_CONFLICT           ((TCM_RESULT) (TCM_BASE + 59))
#define TCM_AREA_LOCKED             ((TCM_RESULT) (TCM_BASE + 60))
#define TCM_BAD_LOCALITY            ((TCM_RESULT) (TCM_BASE + 61))
#define TCM_READ_ONLY               ((TCM_RESULT) (TCM_BASE + 62))
#define TCM_PER_NOWRITE             ((TCM_RESULT) (TCM_BASE + 63))
#define TCM_FAMILYCOUNT             ((TCM_RESULT) (TCM_BASE + 64))
#define TCM_WRITE_LOCKED            ((TCM_RESULT) (TCM_BASE + 65))
#define TCM_BAD_ATTRIBUTES          ((TCM_RESULT) (TCM_BASE + 66))
#define TCM_INVALID_STRUCTURE       ((TCM_RESULT) (TCM_BASE + 67))
#define TCM_KEY_OWNER_CONTROL       ((TCM_RESULT) (TCM_BASE + 68))
#define TCM_BAD_COUNTER             ((TCM_RESULT) (TCM_BASE + 69))
#define TCM_NOT_FULLWRITE           ((TCM_RESULT) (TCM_BASE + 70))
#define TCM_CONTEXT_GAP             ((TCM_RESULT) (TCM_BASE + 71))
#define TCM_MAXNVWRITES             ((TCM_RESULT) (TCM_BASE + 72))
#define TCM_NOOPERATOR              ((TCM_RESULT) (TCM_BASE + 73))
#define TCM_RESOURCEMISSING         ((TCM_RESULT) (TCM_BASE + 74))
#define TCM_DELEGATE_LOCK           ((TCM_RESULT) (TCM_BASE + 75))
#define TCM_DELEGATE_FAMILY         ((TCM_RESULT) (TCM_BASE + 76))
#define TCM_DELEGATE_ADMIN          ((TCM_RESULT) (TCM_BASE + 77))
#define TCM_TRANSPORT_NOTEXCLUSIVE  ((TCM_RESULT) (TCM_BASE + 78))
#define TCM_OWNER_CONTROL           ((TCM_RESULT) (TCM_BASE + 79))
#define TCM_DAA_RESOURCES           ((TCM_RESULT) (TCM_BASE + 80))
#define TCM_DAA_INPUT_DATA0         ((TCM_RESULT) (TCM_BASE + 81))
#define TCM_DAA_INPUT_DATA1         ((TCM_RESULT) (TCM_BASE + 82))
#define TCM_DAA_ISSUER_SETTINGS     ((TCM_RESULT) (TCM_BASE + 83))
#define TCM_DAA_TCM_SETTINGS        ((TCM_RESULT) (TCM_BASE + 84))
#define TCM_DAA_STAGE               ((TCM_RESULT) (TCM_BASE + 85))
#define TCM_DAA_ISSUER_VALIDITY     ((TCM_RESULT) (TCM_BASE + 86))
#define TCM_DAA_WRONG_W             ((TCM_RESULT) (TCM_BASE + 87))
#define TCM_BAD_HANDLE              ((TCM_RESULT) (TCM_BASE + 88))
#define TCM_BAD_DELEGATE            ((TCM_RESULT) (TCM_BASE + 89))
#define TCM_BADCONTEXT              ((TCM_RESULT) (TCM_BASE + 90))
#define TCM_TOOMANYCONTEXTS         ((TCM_RESULT) (TCM_BASE + 91))
#define TCM_MA_TICKET_SIGNATURE     ((TCM_RESULT) (TCM_BASE + 92))
#define TCM_MA_DESTINATION          ((TCM_RESULT) (TCM_BASE + 93))
#define TCM_MA_SOURCE               ((TCM_RESULT) (TCM_BASE + 94))
#define TCM_MA_AUTHORITY            ((TCM_RESULT) (TCM_BASE + 95))
#define TCM_PERMANENTEK             ((TCM_RESULT) (TCM_BASE + 97))
#define TCM_BAD_SIGNATURE           ((TCM_RESULT) (TCM_BASE + 98))
#define TCM_NOCONTEXTSPACE          ((TCM_RESULT) (TCM_BASE + 99))

#define TCM_RETRY                   ((TCM_RESULT) (TCM_BASE + TCM_NON_FATAL))
#define TCM_NEEDS_SELFTEST          ((TCM_RESULT) (TCM_BASE + TCM_NON_FATAL + 1))
#define TCM_DOING_SELFTEST          ((TCM_RESULT) (TCM_BASE + TCM_NON_FATAL + 2))
#define TCM_DEFEND_LOCK_RUNNING     ((TCM_RESULT) (TCM_BASE + TCM_NON_FATAL + 3))

//
// Part 2, section 17: Ordinals
//
// Ordinals are 32 bit values. The upper byte contains values that serve as
// flag indicators, the next byte contains values indicating what committee
// designated the ordinal, and the final two bytes contain the Command
// Ordinal Index.
//      3                   2                   1
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |P|C|V| Reserved| Purview |     Command Ordinal Index           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//  Where:
//
//    * P is Protected/Unprotected command. When 0 the command is a Protected
//      command, when 1 the command is an Unprotected command.
//
//    * C is Non-Connection/Connection related command. When 0 this command
//      passes through to either the protected (TCM) or unprotected (TSS)
//      components.
//
//    * V is TCM/Vendor command. When 0 the command is TCM defined, when 1 the
//      command is vendor defined.
//
//    * All reserved area bits are set to 0.
//

#define TCM_ORD_ActivateIdentity                  ((TCM_COMMAND_CODE) 0x0000807A)
#define TCM_ORD_AuthorizeMigrationKey             ((TCM_COMMAND_CODE) 0x0000802B)
#define TCM_ORD_CertifyKey                        ((TCM_COMMAND_CODE) 0x00008032)
#define TCM_ORD_CertifyKey2                       ((TCM_COMMAND_CODE) 0x00008033)
#define TCM_ORD_CertifySelfTest                   ((TCM_COMMAND_CODE) 0x00008052)
#define TCM_ORD_ChangeAuth                        ((TCM_COMMAND_CODE) 0x0000800C)
#define TCM_ORD_ChangeAuthAsymFinish              ((TCM_COMMAND_CODE) 0x0000800F)
#define TCM_ORD_ChangeAuthAsymStart               ((TCM_COMMAND_CODE) 0x0000800E)
#define TCM_ORD_ChangeAuthOwner                   ((TCM_COMMAND_CODE) 0x00008010)
#define TCM_ORD_CMK_ApproveMA                     ((TCM_COMMAND_CODE) 0x0000801D)
#define TCM_ORD_CMK_ConvertMigration              ((TCM_COMMAND_CODE) 0x00008024)
#define TCM_ORD_CMK_CreateBlob                    ((TCM_COMMAND_CODE) 0x0000801B)
#define TCM_ORD_CMK_CreateKey                     ((TCM_COMMAND_CODE) 0x00008013)
#define TCM_ORD_CMK_CreateTicket                  ((TCM_COMMAND_CODE) 0x00008012)
#define TCM_ORD_CMK_SetRestrictions               ((TCM_COMMAND_CODE) 0x0000801C)
#define TCM_ORD_ContinueSelfTest                  ((TCM_COMMAND_CODE) 0x00008053)
#define TCM_ORD_ConvertMigrationBlob              ((TCM_COMMAND_CODE) 0x0000802A)
#define TCM_ORD_CreateCounter                     ((TCM_COMMAND_CODE) 0x000080DC)
#define TCM_ORD_CreateEndorsementKeyPair          ((TCM_COMMAND_CODE) 0x00008078)
#define TCM_ORD_CreateMaintenanceArchive          ((TCM_COMMAND_CODE) 0x0000802C)
#define TCM_ORD_CreateMigrationBlob               ((TCM_COMMAND_CODE) 0x00008028)
#define TCM_ORD_CreateRevocableEK                 ((TCM_COMMAND_CODE) 0x0000807F)
#define TCM_ORD_CreateWrapKey                     ((TCM_COMMAND_CODE) 0x0000801F)
#define TCM_ORD_DAA_JOIN                          ((TCM_COMMAND_CODE) 0x00008029)
#define TCM_ORD_DAA_SIGN                          ((TCM_COMMAND_CODE) 0x00008031)
#define TCM_ORD_Delegate_CreateKeyDelegation      ((TCM_COMMAND_CODE) 0x000080D4)
#define TCM_ORD_Delegate_CreateOwnerDelegation    ((TCM_COMMAND_CODE) 0x000080D5)
#define TCM_ORD_Delegate_LoadOwnerDelegation      ((TCM_COMMAND_CODE) 0x000080D8)
#define TCM_ORD_Delegate_Manage                   ((TCM_COMMAND_CODE) 0x000080D2)
#define TCM_ORD_Delegate_ReadTable                ((TCM_COMMAND_CODE) 0x000080DB)
#define TCM_ORD_Delegate_UpdateVerification       ((TCM_COMMAND_CODE) 0x000080D1)
#define TCM_ORD_Delegate_VerifyDelegation         ((TCM_COMMAND_CODE) 0x000080D6)
#define TCM_ORD_DirRead                           ((TCM_COMMAND_CODE) 0x0000801A)
#define TCM_ORD_DirWriteAuth                      ((TCM_COMMAND_CODE) 0x00008019)
#define TCM_ORD_DisableForceClear                 ((TCM_COMMAND_CODE) 0x0000805E)
#define TCM_ORD_DisableOwnerClear                 ((TCM_COMMAND_CODE) 0x0000805C)
#define TCM_ORD_DisablePubekRead                  ((TCM_COMMAND_CODE) 0x0000807E)
#define TCM_ORD_DSAP                              ((TCM_COMMAND_CODE) 0x00008011)
#define TCM_ORD_EstablishTransport                ((TCM_COMMAND_CODE) 0x000080E6)
#define TCM_ORD_EvictKey                          ((TCM_COMMAND_CODE) 0x00008022)
#define TCM_ORD_ExecuteTransport                  ((TCM_COMMAND_CODE) 0x000080E7)
#define TCM_ORD_Extend                            ((TCM_COMMAND_CODE) 0x00008014)
#define TCM_ORD_FieldUpgrade                      ((TCM_COMMAND_CODE) 0x000080AA)
#define TCM_ORD_FlushSpecific                     ((TCM_COMMAND_CODE) 0x000080BA)
#define TCM_ORD_ForceClear                        ((TCM_COMMAND_CODE) 0x0000805D)
#define TCM_ORD_GetAuditDigest                    ((TCM_COMMAND_CODE) 0x00008085)
#define TCM_ORD_GetAuditDigestSigned              ((TCM_COMMAND_CODE) 0x00008086)
#define TCM_ORD_GetAuditEvent                     ((TCM_COMMAND_CODE) 0x00008082)
#define TCM_ORD_GetAuditEventSigned               ((TCM_COMMAND_CODE) 0x00008083)
#define TCM_ORD_GetCapability                     ((TCM_COMMAND_CODE) 0x00008065)
#define TCM_ORD_GetCapabilityOwner                ((TCM_COMMAND_CODE) 0x00008066)
#define TCM_ORD_GetCapabilitySigned               ((TCM_COMMAND_CODE) 0x00008064)
#define TCM_ORD_GetOrdinalAuditStatus             ((TCM_COMMAND_CODE) 0x0000808C)
#define TCM_ORD_GetPubKey                         ((TCM_COMMAND_CODE) 0x00008021)
#define TCM_ORD_GetRandom                         ((TCM_COMMAND_CODE) 0x00008046)
#define TCM_ORD_GetTestResult                     ((TCM_COMMAND_CODE) 0x00008054)
#define TCM_ORD_GetTicks                          ((TCM_COMMAND_CODE) 0x000080F1)
#define TCM_ORD_IncrementCounter                  ((TCM_COMMAND_CODE) 0x000080DD)
#define TCM_ORD_Init                              ((TCM_COMMAND_CODE) 0x00008097)
#define TCM_ORD_KeyControlOwner                   ((TCM_COMMAND_CODE) 0x00008023)
#define TCM_ORD_KillMaintenanceFeature            ((TCM_COMMAND_CODE) 0x0000802E)
#define TCM_ORD_LoadAuthContext                   ((TCM_COMMAND_CODE) 0x000080B7)
#define TCM_ORD_LoadContext                       ((TCM_COMMAND_CODE) 0x000080B9)
#define TCM_ORD_LoadKey                           ((TCM_COMMAND_CODE) 0x00008020)
#define TCM_ORD_LoadKey2                          ((TCM_COMMAND_CODE) 0x00008041)
#define TCM_ORD_LoadKeyContext                    ((TCM_COMMAND_CODE) 0x000080B5)
#define TCM_ORD_LoadMaintenanceArchive            ((TCM_COMMAND_CODE) 0x0000802D)
#define TCM_ORD_LoadManuMaintPub                  ((TCM_COMMAND_CODE) 0x0000802F)
#define TCM_ORD_MakeIdentity                      ((TCM_COMMAND_CODE) 0x00008079)
#define TCM_ORD_MigrateKey                        ((TCM_COMMAND_CODE) 0x00008025)
#define TCM_ORD_NV_DefineSpace                    ((TCM_COMMAND_CODE) 0x000080CC)
#define TCM_ORD_NV_ReadValue                      ((TCM_COMMAND_CODE) 0x000080CF)
#define TCM_ORD_NV_ReadValueAuth                  ((TCM_COMMAND_CODE) 0x000080D0)
#define TCM_ORD_NV_WriteValue                     ((TCM_COMMAND_CODE) 0x000080CD)
#define TCM_ORD_NV_WriteValueAuth                 ((TCM_COMMAND_CODE) 0x000080CE)
#define TCM_ORD_OIAP                              ((TCM_COMMAND_CODE) 0x0000800A)
#define TCM_ORD_OSAP                              ((TCM_COMMAND_CODE) 0x0000800B)
#define TCM_ORD_OwnerClear                        ((TCM_COMMAND_CODE) 0x0000805B)
#define TCM_ORD_OwnerReadInternalPub              ((TCM_COMMAND_CODE) 0x00008081)
#define TCM_ORD_OwnerReadPubek                    ((TCM_COMMAND_CODE) 0x0000807D)
#define TCM_ORD_OwnerSetDisable                   ((TCM_COMMAND_CODE) 0x0000806E)
#define TCM_ORD_PCR_Reset                         ((TCM_COMMAND_CODE) 0x000080C8)
#define TCM_ORD_PcrRead                           ((TCM_COMMAND_CODE) 0x00008015)
#define TCM_ORD_PhysicalDisable                   ((TCM_COMMAND_CODE) 0x00008070)
#define TCM_ORD_PhysicalEnable                    ((TCM_COMMAND_CODE) 0x0000806F)
#define TCM_ORD_PhysicalSetDeactivated            ((TCM_COMMAND_CODE) 0x00008072)
#define TCM_ORD_Quote                             ((TCM_COMMAND_CODE) 0x00008016)
#define TCM_ORD_Quote2                            ((TCM_COMMAND_CODE) 0x0000803E)
#define TCM_ORD_ReadCounter                       ((TCM_COMMAND_CODE) 0x000080DE)
#define TCM_ORD_ReadManuMaintPub                  ((TCM_COMMAND_CODE) 0x00008030)
#define TCM_ORD_ReadPubek                         ((TCM_COMMAND_CODE) 0x0000807C)
#define TCM_ORD_ReleaseCounter                    ((TCM_COMMAND_CODE) 0x000080DF)
#define TCM_ORD_ReleaseCounterOwner               ((TCM_COMMAND_CODE) 0x000080E0)
#define TCM_ORD_ReleaseTransportSigned            ((TCM_COMMAND_CODE) 0x000080E8)
#define TCM_ORD_Reset                             ((TCM_COMMAND_CODE) 0x0000805A)
#define TCM_ORD_ResetLockValue                    ((TCM_COMMAND_CODE) 0x00008040)
#define TCM_ORD_RevokeTrust                       ((TCM_COMMAND_CODE) 0x00008080)
#define TCM_ORD_SaveAuthContext                   ((TCM_COMMAND_CODE) 0x000080B6)
#define TCM_ORD_SaveContext                       ((TCM_COMMAND_CODE) 0x000080B8)
#define TCM_ORD_SaveKeyContext                    ((TCM_COMMAND_CODE) 0x000080B4)
#define TCM_ORD_SaveState                         ((TCM_COMMAND_CODE) 0x00008098)
#define TCM_ORD_Seal                              ((TCM_COMMAND_CODE) 0x00008017)
#define TCM_ORD_Sealx                             ((TCM_COMMAND_CODE) 0x0000803D)
#define TCM_ORD_SelfTestFull                      ((TCM_COMMAND_CODE) 0x00008050)
#define TCM_ORD_SetCapability                     ((TCM_COMMAND_CODE) 0x0000803F)
#define TCM_ORD_SetOperatorAuth                   ((TCM_COMMAND_CODE) 0x00008074)
#define TCM_ORD_SetOrdinalAuditStatus             ((TCM_COMMAND_CODE) 0x0000808D)
#define TCM_ORD_SetOwnerInstall                   ((TCM_COMMAND_CODE) 0x00008071)
#define TCM_ORD_SetOwnerPointer                   ((TCM_COMMAND_CODE) 0x00008075)
#define TCM_ORD_SetRedirection                    ((TCM_COMMAND_CODE) 0x0000809A)
#define TCM_ORD_SetTempDeactivated                ((TCM_COMMAND_CODE) 0x00008073)
#define TCM_ORD_SCHComplete                      ((TCM_COMMAND_CODE) 0x000080EC)
#define TCM_ORD_SCHCompleteExtend                ((TCM_COMMAND_CODE) 0x000080ED)
#define TCM_ORD_SCHStart                         ((TCM_COMMAND_CODE) 0x000080EA)
#define TCM_ORD_SCHUpdate                        ((TCM_COMMAND_CODE) 0x000080EB)
#define TCM_ORD_Sign                              ((TCM_COMMAND_CODE) 0x0000803C)
#define TCM_ORD_Startup                           ((TCM_COMMAND_CODE) 0x00008099)			//0x00000099)
#define TCM_ORD_StirRandom                        ((TCM_COMMAND_CODE) 0x00008047)
#define TCM_ORD_TakeOwnership                     ((TCM_COMMAND_CODE) 0x0000800D)
#define TCM_ORD_Terminate_Handle                  ((TCM_COMMAND_CODE) 0x00008096)
#define TCM_ORD_TickStampBlob                     ((TCM_COMMAND_CODE) 0x000080F2)
#define TCM_ORD_UnBind                            ((TCM_COMMAND_CODE) 0x0000801E)
#define TCM_ORD_Unseal                            ((TCM_COMMAND_CODE) 0x00008018)
#define TCM_TSC_ORD_PhysicalPresence                  ((TCM_COMMAND_CODE) 0x4000800A)
#define TCM_TSC_ORD_ResetEstablishmentBit             ((TCM_COMMAND_CODE) 0x4000800B)

//
// Part 2, section 18: Context structures
//

typedef struct tdTCM_CONTEXT_BLOB
{
    TCM_STRUCTURE_TAG               tag;
    TCM_RESOURCE_TYPE               resourceType;
    TCM_HANDLE                      handle;
    UINT8                           label[16];
    UINT32                          contextCount;
    TCM_DIGEST                      integrityDigest;
    UINT32                          additionalSize;
    UINT8                           *additionalData;
    UINT32                          sensitiveSize;
    UINT8                           *sensitiveData;
} TCM_CONTEXT_BLOB;

typedef struct tdTCM_CONTEXT_SENSITIVE
{
    TCM_STRUCTURE_TAG               tag;
    TCM_NONCE                       contextNonce;
    UINT32                          internalSize;
    UINT8                           *internalData;
} TCM_CONTEXT_SENSITIVE;

//
// Part 2, section 19: NV Structures
//

#define TCM_NV_INDEX_LOCK              ((UINT32)0xffffffff)
#define TCM_NV_INDEX0                  ((UINT32)0x00000000)
#define TCM_NV_INDEX_DIR               ((UINT32)0x10000001)
#define TCM_NV_INDEX_EKCert            ((UINT32)0x0000f000)
#define TCM_NV_INDEX_TCM_CC            ((UINT32)0x0000f001)
#define TCM_NV_INDEX_PlatformCert      ((UINT32)0x0000f002)
#define TCM_NV_INDEX_Platform_CC       ((UINT32)0x0000f003)
// The following define ranges of reserved indices.
#define TCM_NV_INDEX_TSS_BASE          ((UINT32)0x00011100)
#define TCM_NV_INDEX_PC_BASE           ((UINT32)0x00011200)
#define TCM_NV_INDEX_SERVER_BASE       ((UINT32)0x00011300)
#define TCM_NV_INDEX_MOBILE_BASE       ((UINT32)0x00011400)
#define TCM_NV_INDEX_PERIPHERAL_BASE   ((UINT32)0x00011500)
#define TCM_NV_INDEX_GROUP_RESV_BASE   ((UINT32)0x00010000)

typedef UINT32 TCM_NV_PER_ATTRIBUTES;
// The typedefs TCM_NV_PER_ATTRIBUTES (not present in TCM 1.2 Spec. have been added 
// and structure fields that were to hold the following values
#define TCM_NV_PER_READ_STCLEAR        (((UINT32)1)<<31)
#define TCM_NV_PER_AUTHREAD            (((UINT32)1)<<18)
#define TCM_NV_PER_OWNERREAD           (((UINT32)1)<<17)
#define TCM_NV_PER_PPREAD              (((UINT32)1)<<16)
#define TCM_NV_PER_GLOBALLOCK          (((UINT32)1)<<15)
#define TCM_NV_PER_WRITE_STCLEAR       (((UINT32)1)<<14)
#define TCM_NV_PER_WRITEDEFINE         (((UINT32)1)<<13)
#define TCM_NV_PER_WRITEALL            (((UINT32)1)<<12)
#define TCM_NV_PER_AUTHWRITE           (((UINT32)1)<<2)
#define TCM_NV_PER_OWNERWRITE          (((UINT32)1)<<1)
#define TCM_NV_PER_PPWRITE             (((UINT32)1)<<0)

typedef struct tdTCM_NV_ATTRIBUTES
{
    TCM_STRUCTURE_TAG               tag;
    TCM_NV_PER_ATTRIBUTES           attributes;
} TCM_NV_ATTRIBUTES;


typedef struct tdTCM_NV_DATA_PUBLIC
{
    TCM_STRUCTURE_TAG               tag;
    TCM_NV_INDEX                    nvIndex;
    TCM_PCR_INFO_SHORT              pcrInfoRead;
    TCM_PCR_INFO_SHORT              pcrInfoWrite;
    TCM_NV_ATTRIBUTES               permission;
    BOOLEAN                         bReadSTClear;
    BOOLEAN                         bWriteSTClear;
    BOOLEAN                         bWriteDefine;
    UINT32                          dataSize;
} TCM_NV_DATA_PUBLIC;



// Internal to TCM:
//typedef struct tdTCM_NV_DATA_SENSITIVE
//{
//    TCM_STRUCTURE_TAG               tag;
//    TCM_NV_DATA_PUBLIC              pubInfo;
//   TCM_AUTHDATA                    authValue;
//    UINT8                           *data;
//} TCM_NV_DATA_SENSITIVE;


//
// Part 2, section 20: Delegation
//

//
// Part 2, section 20.3: Owner Permissions Settings for per1 bits
//
#define TCM_DELEGATE_SetOrdinalAuditStatus          (((UINT32)1)<<30)
#define TCM_DELEGATE_DirWriteAuth                   (((UINT32)1)<<29)
#define TCM_DELEGATE_CMK_ApproveMA                  (((UINT32)1)<<28)
#define TCM_DELEGATE_CMK_CreateTicket               (((UINT32)1)<<26)
#define TCM_DELEGATE_Delegate_LoadOwnerDelegation   (((UINT32)1)<<24)
#define TCM_DELEGATE_DAA_Join                       (((UINT32)1)<<23)
#define TCM_DELEGATE_AuthorizeMigrationKey          (((UINT32)1)<<22)
#define TCM_DELEGATE_CreateMaintenanceArchive       (((UINT32)1)<<21)
#define TCM_DELEGATE_LoadMaintenanceArchive         (((UINT32)1)<<20)
#define TCM_DELEGATE_KillMaintenanceFeature         (((UINT32)1)<<19)
#define TCM_DELEGATE_OwnerReadInteralPub            (((UINT32)1)<<18)
#define TCM_DELEGATE_ResetLockValue                 (((UINT32)1)<<17)
#define TCM_DELEGATE_OwnerClear                     (((UINT32)1)<<16)
#define TCM_DELEGATE_DisableOwnerClear              (((UINT32)1)<<15)
#define TCM_DELEGATE_OwnerSetDisable                (((UINT32)1)<<13)
#define TCM_DELEGATE_SetCapability                  (((UINT32)1)<<12)
#define TCM_DELEGATE_MakeIdentity                   (((UINT32)1)<<11)
#define TCM_DELEGATE_ActivateIdentity               (((UINT32)1)<<10)
#define TCM_DELEGATE_OwnerReadPubek                 (((UINT32)1)<<9)
#define TCM_DELEGATE_DisablePubekRead               (((UINT32)1)<<8)
#define TCM_DELEGATE_SetRedirection                 (((UINT32)1)<<7)
#define TCM_DELEGATE_FieldUpgrade                   (((UINT32)1)<<6)
#define TCM_DELEGATE_Delegate_UpdateVerification    (((UINT32)1)<<5)
#define TCM_DELEGATE_CreateCounter                  (((UINT32)1)<<4)
#define TCM_DELEGATE_ReleaseCounterOwner            (((UINT32)1)<<3)
#define TCM_DELEGATE_DelegateManage                 (((UINT32)1)<<2)
#define TCM_DELEGATE_Delegate_CreateOwnerDelegation (((UINT32)1)<<1)
#define TCM_DELEGATE_DAA_Sign                       (((UINT32)1)<<0)

//
// Part 2, section 20.3: Key Permissions Settings for per1 bits
//
#define TCM_KEY_DELEGATE_CMK_ConvertMigration       (((UINT32)1)<<28)
#define TCM_KEY_DELEGATE_TickStampBlob              (((UINT32)1)<<27)
#define TCM_KEY_DELEGATE_ChangeAuthAsymStart        (((UINT32)1)<<26)
#define TCM_KEY_DELEGATE_ChangeAuthAsymFinish       (((UINT32)1)<<25)
#define TCM_KEY_DELEGATE_CMK_CreateKey              (((UINT32)1)<<24)
#define TCM_KEY_DELEGATE_MigrateKey                 (((UINT32)1)<<23)
#define TCM_KEY_DELEGATE_LoadKey2                   (((UINT32)1)<<22)
#define TCM_KEY_DELEGATE_EstablishTransport         (((UINT32)1)<<21)
#define TCM_KEY_DELEGATE_ReleaseTransportSigned     (((UINT32)1)<<20)
#define TCM_KEY_DELEGATE_Quote2                     (((UINT32)1)<<19)
#define TCM_KEY_DELEGATE_Sealx                      (((UINT32)1)<<18)
#define TCM_KEY_DELEGATE_MakeIdentity               (((UINT32)1)<<17)
#define TCM_KEY_DELEGATE_ActivateIdentity           (((UINT32)1)<<16)
#define TCM_KEY_DELEGATE_GetAuditDigestSigned       (((UINT32)1)<<15)
#define TCM_KEY_DELEGATE_Sign                       (((UINT32)1)<<14)
#define TCM_KEY_DELEGATE_CertifyKey2                (((UINT32)1)<<13)
#define TCM_KEY_DELEGATE_CertifyKey                 (((UINT32)1)<<12)
#define TCM_KEY_DELEGATE_CreateWrapKey              (((UINT32)1)<<11)
#define TCM_KEY_DELEGATE_CMK_CreateBlob             (((UINT32)1)<<10)
#define TCM_KEY_DELEGATE_CreateMigrationBlob        (((UINT32)1)<<9)
#define TCM_KEY_DELEGATE_ConvertMigrationBlob       (((UINT32)1)<<8)
#define TCM_KEY_DELEGATE_CreateKeyDelegation        (((UINT32)1)<<7)
#define TCM_KEY_DELEGATE_ChangeAuth                 (((UINT32)1)<<6)
#define TCM_KEY_DELEGATE_GetPubKey                  (((UINT32)1)<<5)
#define TCM_KEY_DELEGATE_UnBind                     (((UINT32)1)<<4)
#define TCM_KEY_DELEGATE_Quote                      (((UINT32)1)<<3)
#define TCM_KEY_DELEGATE_Unseal                     (((UINT32)1)<<2)
#define TCM_KEY_DELEGATE_Seal                       (((UINT32)1)<<1)
#define TCM_KEY_DELEGATE_LoadKey                    (((UINT32)1)<<0)

#define TCM_FAMILY_CREATE                 ((UINT32)0x00000001)
#define TCM_FAMILY_ENABLE                 ((UINT32)0x00000002)
#define TCM_FAMILY_ADMIN                  ((UINT32)0x00000003)
#define TCM_FAMILY_INVALIDATE             ((UINT32)0x00000004)

#define TCM_FAMFLAG_DELEGATE_ADMIN_LOCK   (((UINT32)1)<<1)
#define TCM_FAMFLAG_ENABLE                (((UINT32)1)<<0)

typedef struct tdTCM_FAMILY_LABEL
{
    UINT8                           label;
} TCM_FAMILY_LABEL;

typedef struct tdTCM_FAMILY_TABLE_ENTRY
{
    TCM_STRUCTURE_TAG               tag;
    TCM_FAMILY_LABEL                label;
    TCM_FAMILY_ID                   familyID;
    TCM_FAMILY_VERIFICATION         verificationCount;
    TCM_FAMILY_FLAGS                flags;
} TCM_FAMILY_TABLE_ENTRY;

#define TCM_FAMILY_TABLE_ENTRY_MIN 8
//typedef struct tdTCM_FAMILY_TABLE
//{
//    TCM_FAMILY_TABLE_ENTRY          FamTableRow[TCM_NUM_FAMILY_TABLE_ENTRY_MIN];
//} TCM_FAMILY_TABLE;


typedef struct tdTCM_DELEGATE_LABEL
{
    UINT8                           label;
} TCM_DELEGATE_LABEL;


typedef UINT32 TCM_DELEGATE_TYPE;
#define TCM_DEL_OWNER_BITS          ((UINT32)0x00000001)
#define TCM_DEL_KEY_BITS            ((UINT32)0x00000002)

typedef struct tdTCM_DELEGATIONS
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DELEGATE_TYPE               delegateType;
    UINT32                          per1;
    UINT32                          per2;
} TCM_DELEGATIONS;

typedef struct tdTCM_DELEGATE_PUBLIC
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DELEGATE_LABEL              label;
    TCM_PCR_INFO_SHORT              pcrInfo;
    TCM_DELEGATIONS                 permissions;
    TCM_FAMILY_ID                   familyID;
    TCM_FAMILY_VERIFICATION         verificationCount;
} TCM_DELEGATE_PUBLIC;

typedef struct tdTCM_DELEGATE_TABLE_ROW
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DELEGATE_PUBLIC             pub;
    TCM_SECRET                      authValue;
} TCM_DELEGATE_TABLE_ROW;


#define TCM_NUM_DELEGATE_TABLE_ENTRY_MIN 2
//typedef struct tdTCM_DELEGATE_TABLE
//{
//    TCM_DELEGATE_TABLE_ROW          delRow[TCM_NUM_DELEGATE_TABLE_ENTRY_MIN];
//} TCM_DELEGATE_TABLE;

typedef struct tdTCM_DELEGATE_SENSITIVE
{
    TCM_STRUCTURE_TAG               tag;
    TCM_SECRET                      authValue;
} TCM_DELEGATE_SENSITIVE;

typedef struct tdTCM_DELEGATE_OWNER_BLOB
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DELEGATE_PUBLIC             pub;
    TCM_DIGEST                      integrityDigest;
    UINT32                          additionalSize;
    UINT8                           *additionalArea;
    UINT32                          sensitiveSize;
    UINT8                           *sensitiveArea;
} TCM_DELEGATE_OWNER_BLOB;

typedef struct tdTCM_DELEGATE_KEY_BLOB
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DELEGATE_PUBLIC             pub;
    TCM_DIGEST                      integrityDigest;
    TCM_DIGEST                      pubKeyDigest;
    UINT32                          additionalSize;
    UINT8                           *additionalArea;
    UINT32                          sensitiveSize;
    UINT8                           *sensitiveArea;
} TCM_DELEGATE_KEY_BLOB;

//
// Part 2, section 21.1: TCM_CAPABILITY_AREA for GetCapability
//
#define TCM_CAP_ORD                     ((TCM_CAPABILITY_AREA) 0x00000001)
#define TCM_CAP_ALG                     ((TCM_CAPABILITY_AREA) 0x00000002)
#define TCM_CAP_PID                     ((TCM_CAPABILITY_AREA) 0x00000003)
#define TCM_CAP_FLAG                    ((TCM_CAPABILITY_AREA) 0x00000004)
#define TCM_CAP_PROPERTY                ((TCM_CAPABILITY_AREA) 0x00000005)
#define TCM_CAP_VERSION                 ((TCM_CAPABILITY_AREA) 0x00000006)
#define TCM_CAP_KEY_HANDLE              ((TCM_CAPABILITY_AREA) 0x00000007)
#define TCM_CAP_CHECK_LOADED            ((TCM_CAPABILITY_AREA) 0x00000008)
#define TCM_CAP_SYM_MODE                ((TCM_CAPABILITY_AREA) 0x00000009)
#define TCM_CAP_KEY_STATUS              ((TCM_CAPABILITY_AREA) 0x0000000C)
#define TCM_CAP_NV_LIST                 ((TCM_CAPABILITY_AREA) 0x0000000D)
#define TCM_CAP_MFR                     ((TCM_CAPABILITY_AREA) 0x00000010)
#define TCM_CAP_NV_INDEX                ((TCM_CAPABILITY_AREA) 0x00000011)
#define TCM_CAP_TRANS_ALG               ((TCM_CAPABILITY_AREA) 0x00000012)
#define TCM_CAP_HANDLE                  ((TCM_CAPABILITY_AREA) 0x00000014)
#define TCM_CAP_TRANS_ES                ((TCM_CAPABILITY_AREA) 0x00000015)
#define TCM_CAP_AUTH_ENCRYPT            ((TCM_CAPABILITY_AREA) 0x00000017)
#define TCM_CAP_SELECT_SIZE             ((TCM_CAPABILITY_AREA) 0x00000018)
#define TCM_CAP_VERSION_VAL             ((TCM_CAPABILITY_AREA) 0x0000001A)

#define TCM_CAP_FLAG_PERMANENT          ((TCM_CAPABILITY_AREA) 0x00000108)
#define TCM_CAP_FLAG_VOLATILE           ((TCM_CAPABILITY_AREA) 0x00000109)

//
// Part 2, section 21.2: CAP_PROPERTY Subcap values for GetCapability
//
#define TCM_CAP_PROP_PCR                ((TCM_CAPABILITY_AREA) 0x00000101)
#define TCM_CAP_PROP_DIR                ((TCM_CAPABILITY_AREA) 0x00000102)
#define TCM_CAP_PROP_MANUFACTURER       ((TCM_CAPABILITY_AREA) 0x00000103)
#define TCM_CAP_PROP_KEYS               ((TCM_CAPABILITY_AREA) 0x00000104)
#define TCM_CAP_PROP_MIN_COUNTER        ((TCM_CAPABILITY_AREA) 0x00000107)
#define TCM_CAP_PROP_AUTHSESS           ((TCM_CAPABILITY_AREA) 0x0000010A)
#define TCM_CAP_PROP_TRANSESS           ((TCM_CAPABILITY_AREA) 0x0000010B)
#define TCM_CAP_PROP_COUNTERS           ((TCM_CAPABILITY_AREA) 0x0000010C)
#define TCM_CAP_PROP_MAX_AUTHSESS       ((TCM_CAPABILITY_AREA) 0x0000010D)
#define TCM_CAP_PROP_MAX_TRANSESS       ((TCM_CAPABILITY_AREA) 0x0000010E)
#define TCM_CAP_PROP_MAX_COUNTERS       ((TCM_CAPABILITY_AREA) 0x0000010F)
#define TCM_CAP_PROP_MAX_KEYS           ((TCM_CAPABILITY_AREA) 0x00000110)
#define TCM_CAP_PROP_OWNER              ((TCM_CAPABILITY_AREA) 0x00000111)
#define TCM_CAP_PROP_CONTEXT            ((TCM_CAPABILITY_AREA) 0x00000112)
#define TCM_CAP_PROP_MAX_CONTEXT        ((TCM_CAPABILITY_AREA) 0x00000113)
#define TCM_CAP_PROP_FAMILYROWS         ((TCM_CAPABILITY_AREA) 0x00000114)
#define TCM_CAP_PROP_TIS_TIMEOUT        ((TCM_CAPABILITY_AREA) 0x00000115)
#define TCM_CAP_PROP_STARTUP_EFFECT     ((TCM_CAPABILITY_AREA) 0x00000116)
#define TCM_CAP_PROP_DELEGATE_ROW       ((TCM_CAPABILITY_AREA) 0x00000117)
#define TCM_CAP_PROP_DAA_MAX            ((TCM_CAPABILITY_AREA) 0x00000119)
#define TCM_CAP_PROP_SESSION_DAA        ((TCM_CAPABILITY_AREA) 0x0000011A)
#define TCM_CAP_PROP_CONTEXT_DIST       ((TCM_CAPABILITY_AREA) 0x0000011B)
#define TCM_CAP_PROP_DAA_INTERRUPT      ((TCM_CAPABILITY_AREA) 0x0000011C)
#define TCM_CAP_PROP_SESSIONS           ((TCM_CAPABILITY_AREA) 0x0000011D)
#define TCM_CAP_PROP_MAX_SESSIONS       ((TCM_CAPABILITY_AREA) 0x0000011E)
#define TCM_CAP_PROP_CMK_RESTRICTION    ((TCM_CAPABILITY_AREA) 0x0000011F)
#define TCM_CAP_PROP_DURATION           ((TCM_CAPABILITY_AREA) 0x00000120)
#define TCM_CAP_PROP_ACTIVE_COUNTER     ((TCM_CAPABILITY_AREA) 0x00000122)
#define TCM_CAP_PROP_MAX_NV_AVAILABLE   ((TCM_CAPABILITY_AREA) 0x00000123)
#define TCM_CAP_PROP_INPUT_BUFFER       ((TCM_CAPABILITY_AREA) 0x00000124)

//
// Part 2, section 21.6: TCM_CAP_VERSION_INFO
//   [size_is(vendorSpecificSize)] BYTE* vendorSpecific;
//
typedef struct tdTCM_CAP_VERSION_INFO {
  TCM_STRUCTURE_TAG                 tag;
  TCM_VERSION                       version;
  UINT16                            specLevel;
  UINT8                             errataRev;
  UINT8                             TCMVendorID[4];
  UINT16                            vendorSpecificSize;
  UINT8                             *vendorSpecific;
} TCM_CAP_VERSION_INFO;

//
// Part 2, section 22: DAA Structures
//

#define TCM_DAA_SIZE_r0                (43)
#define TCM_DAA_SIZE_r1                (43)
#define TCM_DAA_SIZE_r2                (128)
#define TCM_DAA_SIZE_r3                (168)
#define TCM_DAA_SIZE_r4                (219)
#define TCM_DAA_SIZE_NT                (20)
#define TCM_DAA_SIZE_v0                (128)
#define TCM_DAA_SIZE_v1                (192)
#define TCM_DAA_SIZE_NE                (256)
#define TCM_DAA_SIZE_w                 (256)
#define TCM_DAA_SIZE_issuerModulus     (256)
#define TCM_DAA_power0                 (104)
#define TCM_DAA_power1                 (1024)

typedef struct tdTCM_DAA_ISSUER
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DIGEST                      DAA_digest_R0;
    TCM_DIGEST                      DAA_digest_R1;
    TCM_DIGEST                      DAA_digest_S0;
    TCM_DIGEST                      DAA_digest_S1;
    TCM_DIGEST                      DAA_digest_n;
    TCM_DIGEST                      DAA_digest_gamma;
    UINT8                           DAA_generic_q[26];
} TCM_DAA_ISSUER;


typedef struct tdTCM_DAA_TCM
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DIGEST                      DAA_digestIssuer;
    TCM_DIGEST                      DAA_digest_v0;
    TCM_DIGEST                      DAA_digest_v1;
    TCM_DIGEST                      DAA_rekey;
    UINT32                          DAA_count;
} TCM_DAA_TCM;

typedef struct tdTCM_DAA_CONTEXT
{
    TCM_STRUCTURE_TAG               tag;
    TCM_DIGEST                      DAA_digestContext;
    TCM_DIGEST                      DAA_digest;
    TCM_DAA_CONTEXT_SEED            DAA_contextSeed;
    UINT8                           DAA_scratch[256];
    UINT8                           DAA_stage;
} TCM_DAA_CONTEXT;

typedef struct tdTCM_DAA_JOINDATA
{
    UINT8                           DAA_join_u0[128];
    UINT8                           DAA_join_u1[138];
    TCM_DIGEST                      DAA_digest_n0;
} TCM_DAA_JOINDATA;

typedef struct tdTCM_DAA_BLOB
{
    TCM_STRUCTURE_TAG               tag;
    TCM_RESOURCE_TYPE               resourceType;
    UINT8                           label[16];
    TCM_DIGEST                      blobIntegrity;
    UINT32                          additionalSize;
    UINT8                           *additionalData;
    UINT32                          sensitiveSize;
    UINT8                           *sensitiveData;
} TCM_DAA_BLOB;

typedef struct tdTCM_DAA_SENSITIVE
{
    TCM_STRUCTURE_TAG               tag;
    UINT32                          internalSize;
    UINT8                           *internalData;
} TCM_DAA_SENSITIVE;


//
// Part 2, section 23: Redirection
//

// This section of the TCM spec defines exactly one value but does not
// give it a name. The definition of TCM_SetRedirection in Part3
// refers to exactly one name but does not give its value. We join
// them here.
#define TCM_REDIR_GPIO              (0x00000001)

//
// TCM Command & Response Headers
//
typedef struct tdTCM_RQU_COMMAND_HDR {
  TCM_STRUCTURE_TAG                 tag;
  UINT32                            paramSize;
  TCM_COMMAND_CODE                  ordinal;
} TCM_RQU_COMMAND_HDR;

typedef struct tdTCM_RSP_COMMAND_HDR {
  TCM_STRUCTURE_TAG                 tag;
  UINT32                            paramSize;
  TCM_RESULT                        returnCode;
} TCM_RSP_COMMAND_HDR;

#pragma pack (pop)

#endif  // _TCM_H_
