//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2013 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      This file contains the QPI discovery specific information.        *
// *                                                                        *
// **************************************************************************
--*/

#ifndef _QPI_DISCOVERY_H_
#define _QPI_DISCOVERY_H_

#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#include "DataTypes.h"
#include "PlatformHost.h"
#include "QpiSi.h"

#define MAX_CPU_SOCKETS      MAX_SOCKET // Maximum CPU sockets supported by the system; must be 4 or 8
#define MAX_TREE_NODES       (MAX_CPU_SOCKETS + 2) // 2 additional nodes since a node will appear more than once in the tree when it is being constructed
#define MAX_RING_TREE_NODES  46  // A CPU with 3 links supported will have 1 + 1*3 + 3*2 + 6*2 + 12*2 = 46 nodes maximum in ring tree
#define MAX_RINGS            6      // Maximum number of rings possible in systems with upto 8 sockets (HyperCube)
#define VN0                  0
#define VN1                  1
#define TX                   0
#define RX                   1

#pragma pack(1)

//
// Generic Data structure to describe Link Exchange Parameter (LEP) info
//
typedef struct {
#ifndef ASM_INC
  UINT32  Valid : 1;        // TRUE, if the link is valid (i.e trained successfully for low speed, no validation override that disables it)
  UINT32  PeerSocId : 3;    // Socket ID
  UINT32  PeerSocType : 2;  // Socket Type
  UINT32  PeerPort : 4;     // Port of the peer socket
  UINT32  DualLink : 1;     // TRUE, if there is a second link to the same neighbor
  UINT32  Rsvd1 : 21;
#else
  UINT32  RawData;
#endif // ASM_INC
} QPI_LINK_DATA;

//
// Generic Data structure to describe socket info
//
typedef struct {
#ifndef ASM_INC
  UINT8         Valid : 1;
  UINT8         SocId : 3;                // Socket ID
  UINT8         Vn    : 1;                // VN used by traffic originating at this socket
  UINT8         Rsvd1 : 3;
#else
  UINT8         RawData;
#endif // ASM_INC
  QPI_LINK_DATA LinkData[MAX_QPI_PORTS];  // QPI Link data
} QPI_CPU_SOCKET_DATA;

//
// Generic Data structure to describe topology tree
//
typedef union {
#ifndef ASM_INC
  struct {
    UINT32  Valid : 1;
    UINT32  SocId : 3;        // Socket ID
    UINT32  SocType : 2;      // Socket Type
    UINT32  Rsvd1 : 2;
    UINT32  ParentPort : 4;   // Port that connects to the parent
    UINT32  Hop : 4;          // Depth of the node from root socket 
    UINT32  Rsvd2 : 3;
    UINT32  ParentIndex : 7;  // Index of immediate parent
    UINT32  Rsvd3 : 6;
  } Node;
#endif // ASM_INC
  UINT32  RawNode;
} TOPOLOGY_TREE_NODE;


typedef struct {
  UINT32  IgrVnSel;         // Ingress VN SEL setting. Same setting is used for both ports of this R3QPI
  UINT32  EgrVnSel;         // Engress VN SEL; [15:0] for port 0, [16:31] for port 1
}R3QPI_VN_SEL;

//
// Data structure to capture all the route setting
//
typedef struct {
  UINT32         CboOddQpiRt;
  UINT32         CboEvenQpiRt;
  UINT32         HaQpiRt;  
  UINT32         R2PcieQpiRt;
  UINT32         R3QpiRt[MAX_R3QPI];
  R3QPI_VN_SEL   R3QpiVnSel[MAX_R3QPI];
  UINT32         HaQpiFanOut;
#ifndef ASM_INC
  UINT32         R3QpiSnpFanOut[MAX_R3QPI][MAX_PORT_IN_R3QPI];
#else
  UINT32         R3QpiSnpFanOut[MAX_R3QPI * MAX_PORT_IN_R3QPI];
#endif
  UINT32         QpiAgentRt[MAX_QPI_PORTS];
} QPI_CPU_RT;

//
// Topology Tree info generated and consumed by Discovery engine.
// The Si specific data (currently only port number) is abstracted out using generic port number
// 0, 1, ... MAX_QPI_PORTS. If a Si family has port numbering that is contiguous, then this abtrasction
// has no effect.
//
typedef struct {
  UINT8               TotCpus;  // Total number of CPUs populated
  UINT32              CpuList;  // List of CPUs populated
  UINT8               TotRings;  // Total number of rings present in the topology
  UINT8               RingNodePairs[MAX_CPU_SOCKETS];  // Socket pair that is common to any two rings
  UINT8               SocMap[MAX_CPU_SOCKETS]; // Mapping of intel reference system socket id to this system's socket id
  QPI_CPU_SOCKET_DATA Cpu[MAX_CPU_SOCKETS]; // Si agnostic socket data (Port numbers are generalized)
#ifndef ASM_INC
  UINT8               Rings[MAX_RINGS][5];  // All the rings present in the topology
  UINT8               RingInfo[MAX_CPU_SOCKETS][MAX_RINGS];  // Number of rings (Element 0) and the ring numbers (Element 1 onwards) this socket is a part of
  UINT8               LinkLoad[MAX_CPU_SOCKETS][MAX_QPI_PORTS]; // Number of times this link is used; link number is Si agnostic
  TOPOLOGY_TREE_NODE  CpuTree[MAX_CPU_SOCKETS][MAX_TREE_NODES];  // Si agnostic topology tree (Port numbers are generalized)
#else
  UINT8               Rings[MAX_RINGS * 5];  // All the rings present in the topology
  UINT8               RingInfo[MAX_CPU_SOCKETS * MAX_RINGS];  // Number of rings (Element 0) and the ring numbers (Element 1 onwards) this socket is a part of
  UINT8               LinkLoad[MAX_CPU_SOCKETS * MAX_QPI_PORTS]; // Number of times this link is used; link number is Si agnostic
  TOPOLOGY_TREE_NODE  CpuTree[MAX_CPU_SOCKETS * MAX_TREE_NODES];  // Si agnostic topology tree (Port numbers are generalized)
#endif
} QPI_DISC_DATA;


//
// Topology Tree and Final Routing setting for each populated socket
//
typedef struct {
  QPI_CPU_SOCKET_DATA Cpu[MAX_CPU_SOCKETS];
  TOPOLOGY_TREE_NODE  SbspMinPathTree[MAX_TREE_NODES];
#ifndef ASM_INC
  TOPOLOGY_TREE_NODE  CpuTree[MAX_CPU_SOCKETS][MAX_TREE_NODES];
#else
  TOPOLOGY_TREE_NODE  CpuTree[MAX_CPU_SOCKETS * MAX_TREE_NODES];
#endif
  QPI_CPU_RT          Route[MAX_CPU_SOCKETS];
} QPI_SOCKET_DATA;

#pragma pack()

#endif // _QPI_DISCOVERY_H_
