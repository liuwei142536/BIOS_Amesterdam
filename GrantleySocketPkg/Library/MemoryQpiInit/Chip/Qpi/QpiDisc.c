/*++
  This file contains 'Framework Code' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may not be modified, except as allowed by
  additional terms of your license agreement.
--*/
 /*++
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2014 Intel Corp.                             *
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
// *      This file contains QPI system topology discovery and route        *
// *      calculation code.                                                 *
// *                                                                        *
// **************************************************************************
--*/

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"
#include "QpiLib.h"
#include "QpiMisc.h"
#include "SysFunc.h"

QPI_STATUS
PrepareDiscEngData (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_DISC_DATA       *DiscEngData
  );

QPI_STATUS
LocateRings (
  struct sysHost      *host,
  QPI_DISC_DATA       *DiscEngData
  );

STATIC
QPI_STATUS
ConstructRingTree (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  );

STATIC
QPI_STATUS
CheckForRings (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  );

STATIC
QPI_STATUS
ParseRings (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData
  );

STATIC
QPI_STATUS
AssignVn (
  struct sysHost          *host,
  QPI_DISC_DATA           *DiscEngData
  );

STATIC
QPI_STATUS
ComputeTopologyTree (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA            *DiscEngData
  );

STATIC
QPI_STATUS
ExploreThisNode (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                      RootSocId,
  TOPOLOGY_TREE_NODE  *Parent,
  UINT8                      ParentIdx,
  UINT8                      Hop
  );

STATIC
QPI_STATUS
CheckAndAddNodeToTopologyTree (
  struct sysHost           *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                    RootSocId,
  TOPOLOGY_TREE_NODE        *Parent,
  TOPOLOGY_TREE_NODE       *LeafNode,
  UINT8                      Hop
  );

STATIC
QPI_STATUS
TrackPathLoad (
  struct sysHost       *host,
  QPI_DISC_DATA        *DiscEngData,
  TOPOLOGY_TREE_NODE   *Tree,
  TOPOLOGY_TREE_NODE   *LeafNode,
  UINT8                      Adjust
  );

STATIC
UINT8
CalculatePathLoad (
  struct sysHost       *host,
  QPI_DISC_DATA        *DiscEngData,
  TOPOLOGY_TREE_NODE   *Tree,
  TOPOLOGY_TREE_NODE   *LeafNode
  );

STATIC
QPI_STATUS
DetectDeadlock (
  struct sysHost      *host,
  QPI_DISC_DATA       *DiscEngData
  );

STATIC
QPI_STATUS
CopyTopologyTree (
  struct sysHost      *host,
  QPI_SOCKET_DATA      *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA       *DiscEngData
  );

#if MAX_CPU_SOCKETS > 4
STATIC
QPI_STATUS
ComputeTopologyTree8S (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA            *DiscEngData
  );

STATIC
QPI_STATUS
Map8SNodes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  );

STATIC
QPI_STATUS
Map8SRoutes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][2]
  );

STATIC
QPI_STATUS
Map8SRoutes_VN1Disable (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][3]
  );

STATIC
QPI_STATUS
ComputeTopologyTree6S (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  );

STATIC
QPI_STATUS
Map6SNodes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  );

STATIC
QPI_STATUS
Map6SRoutes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8         (*OemRoute)[3][3]
  );
#endif


STATIC
QPI_STATUS
CalculateRT (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal
  );

STATIC
QPI_STATUS
SetupNonBCRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE       *TopologyTree,
  TOPOLOGY_TREE_NODE       *Root,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest
  );

STATIC
QPI_STATUS
SetupBCRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Root,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  BOOLEAN                  PassThruFanout
  );

STATIC
QPI_STATUS
SetRootNodeRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                    SocId,
  UINT8                    DestSocId,
  UINT8                    OutPort1,
  UINT8                    OutPort2,
  BOOLEAN                  DualLink,
  UINT8                    RootVn
  );

STATIC
QPI_STATUS
SetPassThruRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  UINT8                    SocId,
  UINT8                    DestSocId,
  UINT8                    InPort,
  UINT8                    OutPort,
  UINT8                    RootVn
  );

STATIC
QPI_STATUS
SetRootNodeFanoutRoute (
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    OutPort
  );

STATIC
QPI_STATUS
SetPassThruFanoutRoute (
  struct sysHost          *host,
  QPI_SOCKET_DATA          *SocketData,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    InPort,
  UINT8                    OutPort
  );

STATIC
VOID
CheckAndDisableRbtUpdate (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *Cpu,
  TOPOLOGY_TREE_NODE      (*CpuTree)[MAX_TREE_NODES],
  QPI_CPU_RT              *Route, 
  BOOLEAN                 SiPort
  );

STATIC
VOID
OptimizeHaFanoutSnoop (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  );


QPI_STATUS
QpiTopologyDiscovery (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
   Perform the QPI Topology Discovery of the system. It discovers the system
   topology from each socket's perspective and calculates the route. This 
   function must be invoked only by SBSP after collecting LEP of each socket populated.
   This function doesn't program any H/W registers. Route Table of each socket is
   returned in SocketData. The caller is responsible for porgramming the Route Table into
   the appropriate registers upon successful exit from this function.

Arguments:
   
   host  - Pointer to the system host (root) structure
   SocketData - Pointer to socket specific data
   QpiInternalGlobal - QPIRC internal variables.

Returns:

   0 - Success
   Other - Failure

--*/
{
  QPI_DISC_DATA           DiscEngData;

  // Prepare the discovery engine data structure
  PrepareDiscEngData (host, SocketData, &DiscEngData);

  // Check and identify the rings present in the topology
  LocateRings (host, &DiscEngData);

  // Assign VN to each node that is part of a ring
  if (QpiInternalGlobal->Vn1En == TRUE)  {
    AssignVn (host, &DiscEngData);
  }

  // Discover the topology and construct the topology tree.
  ComputeTopologyTree (host, QpiInternalGlobal, &DiscEngData);

  // Check to make sure the route is deadlock free
  DetectDeadlock (host, &DiscEngData);

  // Copy the topology tree info into host/SocketData
  CopyTopologyTree (host, SocketData, QpiInternalGlobal, &DiscEngData);

  // Sanity check discoverd topology
  if(SanityCheckDiscoveredTopology (host, SocketData, QpiInternalGlobal) != QPI_SUCCESS) {
    return QPI_FAILURE;
  } 

  // Calculate the route setting
  CalculateRT (host, SocketData, QpiInternalGlobal);

  return QPI_SUCCESS;
}

QPI_STATUS
PrepareDiscEngData (
  struct sysHost      *host,
  QPI_SOCKET_DATA      *SocketData,
  QPI_DISC_DATA       *DiscEngData
  )
/*++

Routine Description:
  Copies the socket data to disc engine structure and applies the QPI port mapping to make 
  it Si agnostic.

Arguments:
   
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  DiscEngData - Pointer to socket specific data

Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               Idx1, Idx2;
  UINT8               TempPort;

  //
  // Copy the Si specific CPU socket data and convert that into Si agnostic data using the port mapping.
  //
  MemSetLocal ((VOID *) DiscEngData, 0x00, sizeof (QPI_DISC_DATA));
  MemCopy ((UINT8 *) DiscEngData->Cpu, (UINT8 *) SocketData->Cpu, sizeof (DiscEngData->Cpu));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      ++DiscEngData->TotCpus;
      DiscEngData->CpuList = DiscEngData->CpuList | (1 << Idx1);
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (DiscEngData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          TempPort = (UINT8) DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerPort;
          DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerPort = TranslateToGenPort (host, TempPort);
        }
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
LocateRings (
  struct sysHost      *host,
  QPI_DISC_DATA       *DiscEngData
  )
/*++

Routine Description:
  Identifies all the rings present in a topology. A ring is defined as 4 sockets
  forming a circle. Each ring is assigned a unique number and the sockets
  are marked with the number of rings that they belong to and ring number that they
  belong to. This routine also identifies the pair of nodes that belong to more than
  one ring.

Arguments:
   
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data

Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  RingTree[MAX_RING_TREE_NODES];
  UINT8               Index;

  QpiCheckPoint (0xFF, 0xFF, 0xFF, STS_TOPOLOGY_DISCOVERY, MINOR_STS_DETECT_RING, host);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;  Locating the Rings Present in the Topology\n"));

  //
  // Locate all the rings present in the system
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (DiscEngData->Cpu[Index].Valid == TRUE) {
      MemSetLocal ((VOID *) (RingTree), 0x00, sizeof (RingTree));
      ConstructRingTree (host, DiscEngData, RingTree, Index);
      CheckForRings (host, DiscEngData, RingTree, Index);
    }
  }

  //
  // Dump all the rings present in the system
  //
  if (DiscEngData->TotRings == 0) {
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  No Rings Found"));
  } else {
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  %u Ring(s) Found", DiscEngData->TotRings));
    for (Index = 0; Index < DiscEngData->TotRings; ++Index) {
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Ring %u:   CPU%u -> CPU%u -> CPU%u -> CPU%u -> CPU%u",
                  Index, DiscEngData->Rings[Index][0], DiscEngData->Rings[Index][1], 
                  DiscEngData->Rings[Index][2], DiscEngData->Rings[Index][3], DiscEngData->Rings[Index][4]));
    }
  }

  //
  // Parse the rings and identify the pair of nodes that belong to more than 1 ring
  //
  if (DiscEngData->TotRings > 0) {
    ParseRings (host, DiscEngData);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;  Ring Pairs:"));
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (DiscEngData->Cpu[Index].Valid == TRUE && DiscEngData->RingNodePairs[Index] != 0xFF) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    %u  -  %u", Index, DiscEngData->RingNodePairs[Index]));
      }
    }
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ConstructRingTree (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  )
/*++

Routine Description:
  Construct a simple tree for each socket with nodes spanning upto 4 hops. Then parse
  the tree to look for any rings. 

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data
   RingTree - Pointer to full system QPI fabric tree
   RootSocId - Root Socket Id
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  Child, Parent, GrandParent, RootSocket;
  QPI_LINK_DATA       ParentLep;
  QPI_STATUS          Status;
  UINT8               Index;
  UINT8               QpiLink;

  GrandParent.RawNode        = 0; 
  //
  //  Add this socket as Root of the topology tree
  //
  RootSocket.RawNode          = 0;
  RootSocket.Node.Valid       = TRUE;
  RootSocket.Node.SocId       = RootSocId;
  RootSocket.Node.SocType     = SOCKET_TYPE_CPU;
  RootSocket.Node.ParentPort  = 0xF;
  RootSocket.Node.Hop         = 0;
  RootSocket.Node.ParentIndex = 0x7F;
  if ((Status = AddNodeToRingTree (host, RingTree, &RootSocket)) != QPI_SUCCESS) {
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Adding CPU%u to Ring Tree of CPU%u failed - Data Structure Overflow.",
                    RootSocket.Node.SocId, RootSocket.Node.SocId));
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  //
  // Do a Breadth First Search (BSF) and construct the tree. To start with, the RingTree must
  // have the root socket already. The RingTree will grow as we discover new sockets in BSF manner.
  //
  for (Index = 0; RingTree[Index].Node.Valid != FALSE; ++Index) {

    //
    // Stop constructing the tree when we reach hop level 4 (hop 0 being root) since the ring formation
    // invovles only 4 nodes
    //
    Parent = RingTree[Index];
    if (Parent.Node.Hop == 4) {
      break;
    }

    if (Index != 0) {
      GrandParent.RawNode = RingTree[Parent.Node.ParentIndex].RawNode;
    }

    Child.RawNode     = 0;
    Child.Node.Valid  = TRUE;
    Child.Node.Hop    = Parent.Node.Hop + 1;
    Child.Node.ParentIndex = Index;
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
        ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[QpiLink];

        //
        // Skip this link if it is marked as "Not Valid" or "DualLink".
        //
        if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE) {
          continue;
        }

        if (Index != 0 && ParentLep.PeerSocId == GrandParent.Node.SocId) {
          continue; // Skip if the link if it is pointing towards root
        }

        //
        // Get the Child Socket Id, Type and port that connects it to the parent form LEP
        //
        Child.Node.SocId      = ParentLep.PeerSocId;
        Child.Node.SocType    = ParentLep.PeerSocType;
        Child.Node.ParentPort = ParentLep.PeerPort;

        // Add the node into the Ring Tree
        if ((Status = AddNodeToRingTree (host, RingTree, &Child)) != QPI_SUCCESS) {
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Adding CPU%u to Ring Tree of CPU%u failed - Data Structure Overflow.",
                     Child.Node.SocId, RootSocket.Node.SocId));
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      } // for(QpiLink...)
    } else {
      //
      // This is a fatal error
      //
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Unknown Socket Type found in RingTree of CPU%u", RootSocId));
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Topology not supported\n"));
      QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }
  } // for(Index...)

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
CheckForRings (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData,
  TOPOLOGY_TREE_NODE         *RingTree,
  UINT8                      RootSocId
  )
/*++

Routine Description:
  Identify the rings present in the system and store if it is not found already.   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data
   RingTree - Pointer to full system QPI fabric tree
   RootSocId - Root Socket Id
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  RingNode;
  UINT8               RingSocs[5];
  UINT8               Idx1, Idx2, Idx3;
  BOOLEAN             ValidRing;

  // Identify the index of the last node in the ring tree
  QPI_ASSERT((RingTree[0].Node.SocId == RootSocId), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  for (Idx1 = MAX_RING_TREE_NODES - 1; Idx1 != 0; --Idx1) {
    if (RingTree[Idx1].Node.Valid == TRUE) {
      break;
    }
  }

  //
  // The ring tree is a tree that spans 4 hops. By traversing from each leaf node towards the root node and checking
  // if the leaf node is same as root node (i.e the loop completes) and nodes don't repeat we can find if it is a ring or not.
  //
  for (Idx2 = Idx1; Idx2  != 0; --Idx2) {

    RingNode = RingTree[Idx2];

    // 
    // We need to explore only the leaf nodes. If we explored all leaf nodes (i.e all nodes at hop level 4) break out.
    // 
    if (RingNode.Node.Hop != 4) {
      break;
    }

    // If the leaf node is not same as root node, it can form a ring, so continue to neaxt leaf
    if (RingNode.Node.SocId  !=  RootSocId) {
      continue;
    }

    // 
    // We found a leaf node which is same as root node. Traverse back to the root to find out the nodes of the (possible) ring
    // 
    Idx3 = (UINT8)RingNode.Node.Hop;
    while (Idx3 != 0) {
      RingSocs[Idx3] = (UINT8)RingNode.Node.SocId;
      RingNode.Node = RingTree[RingNode.Node.ParentIndex].Node;
      --Idx3;
      QPI_ASSERT((Idx3 == RingNode.Node.Hop), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
    RingSocs[Idx3] = RootSocId;

    // 
    // Make sure it is a valid ring. For a ring to be valid, the nodes 0 to 3 should not repeat and nodes 0 & 2, nodes 1 & 3
    // should not have any direct link between them.
    // 

    // Make sure the 4 nodes of the ring are unique
    if ((RingSocs[0] == RingSocs[1]) || (RingSocs[0] == RingSocs[2]) || (RingSocs[0] == RingSocs[3]) ||
        (RingSocs[1] == RingSocs[2]) || (RingSocs[1] == RingSocs[3]) || (RingSocs[2] == RingSocs[3])) {
      continue;
    }

    // Make sure the disagonally opposite nodes dosen't have connection between them
    ValidRing = TRUE;
    for (Idx3 = 0; Idx3  < MAX_QPI_PORTS; ++Idx3) {
      if ((DiscEngData->Cpu[RingSocs[0]].LinkData[Idx3].PeerSocId == RingSocs[2]) ||
          (DiscEngData->Cpu[RingSocs[1]].LinkData[Idx3].PeerSocId == RingSocs[3])) {
        ValidRing = FALSE;
        break;
      }
    }
    if (ValidRing == FALSE) {
      continue;
    }

    // 
    // Now that we found a valid ring, make sure this ring is not detected already as part of other root sockets
    // 
    ValidRing = TRUE;
    for (Idx3 = 0; Idx3 < DiscEngData->TotRings; ++Idx3) {
      if (CompareRing (host, RingSocs, DiscEngData->Rings[Idx3]) == TRUE) {
        ValidRing = FALSE;
        break;
      }
    }
    if (ValidRing == TRUE) {
      MemCopy ((UINT8 *) DiscEngData->Rings[DiscEngData->TotRings], (UINT8 *) RingSocs, sizeof (DiscEngData->Rings[DiscEngData->TotRings]));
      ++DiscEngData->TotRings;
    }
  } // for(Idx...)

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ParseRings (
  struct sysHost             *host,
  QPI_DISC_DATA              *DiscEngData
  )
/*++

Routine Description:
  Parse the rings and identify the number of rings and the ring number that each socket belongs to.

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               TmpSocId;
  UINT8               Idx1, Idx2, TmpIdx;

  //
  // Identify the number of rings each socket is part of and the ring number
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; ++Idx1) {
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      TmpSocId = DiscEngData->Rings[Idx1][Idx2];
      TmpIdx = DiscEngData->RingInfo[TmpSocId][0]; // Element 0 contains the number of rings this socket is already part of
      DiscEngData->RingInfo[TmpSocId][TmpIdx+1] = Idx1; // Element 1 to x contains the number of the ring which this socket is part
      ++DiscEngData->RingInfo[TmpSocId][0]; // Increase the number of ring counts of which this socket is part of
    }
  }

  //
  // Identify the pair of sockets that is part of any 2 rings in the topology
  //
  MemSetLocal ((UINT8 *) DiscEngData->RingNodePairs, 0xFF, sizeof (DiscEngData->RingNodePairs));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->RingInfo[Idx1][0] == 2) { // If this socket is part of 2 different rings
      for (Idx2 = 0; Idx2 < MAX_CPU_SOCKETS; ++Idx2) { // Identify the other socket that is part of the same 2 rings
        if (Idx2 != Idx1 && DiscEngData->RingInfo[Idx2][0] == 2) {
          if ((DiscEngData->RingInfo[Idx1][1] == DiscEngData->RingInfo[Idx2][1] &&
               DiscEngData->RingInfo[Idx1][2] == DiscEngData->RingInfo[Idx2][2]) ||
              (DiscEngData->RingInfo[Idx1][1] == DiscEngData->RingInfo[Idx2][2] &&
               DiscEngData->RingInfo[Idx1][2] == DiscEngData->RingInfo[Idx2][1])) {
            DiscEngData->RingNodePairs[Idx1] = Idx2;
            break;
          }
        }
      }
    }
  }

  return QPI_SUCCESS;
}



STATIC
QPI_STATUS
AssignVn (
  struct sysHost          *host,
  QPI_DISC_DATA           *DiscEngData
  )
/*++

Routine Description:

  Assign VN to the nodes. It is assigned to only the nodes that are part of a ring. Two adjacent nodes of the ring 
  are assigned VN0 while the other two nodes are assigned VN1. If a pair of nodes is part of more than one ring
  such as in 8SG topology, the VN will be assigned such that each pair of nodes in all rings are different.

   For example:
    +-------------------------+
     |                         |
    S0 ----- S1       S6 ----- S7
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S2 ----- S3       S4 ----- S5
     |                         |
     +-------------------------+

   VN Assignment:
      S0  -  VN0
      S1  -  VN1
      S2  -  VN0
      S3  -  VN1
      S4  -  VN0
      S5  -  VN1
      S6  -  VN0
      S7  -  VN1

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               TmpSoc0, TmpSoc1, TmpSoc2;
  UINT8               Idx1, Idx2, Idx3;
  UINT8               SocVnAssigned, VnAssigned, TmpVn;

  //
  // For each ring, identify the pair of nodes that is also part of another ring and assign same VN number to those pair of nodes
  //
  SocVnAssigned = 0; // Sockets for which VN is assignment
  VnAssigned = 0; // VN (i.e VN0/VN1) assigned to each socket
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; ++Idx1) {
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      TmpSoc1 = DiscEngData->RingNodePairs[TmpSoc0];

      // 
      // If the node is not assigned VN already and it has a pair with which it shares another ring, assign VN to the pair of sockets.
      // Make sure the VN assigned to the pair is different from the VN, if any, already assigned to other 2 nodes of the ring.
      // Since VN is assigned to pair of nodes, just checkig the VN assigned for just of the other 2 nodes is sufficient.
      // 
      if (((SocVnAssigned & (1 << TmpSoc0)) == 0) && (TmpSoc1 != 0xFF)) {
        for (Idx3 = 0; Idx3 < 4; ++Idx3) {
          TmpSoc2 = DiscEngData->Rings[Idx1][Idx3];
          if (TmpSoc2 != TmpSoc0 && TmpSoc2 != TmpSoc1) {
            if (SocVnAssigned & (1 << TmpSoc2)) {
              TmpVn = (((VnAssigned >> TmpSoc2) & 0x1) == VN0) ? VN1 : VN0;
            } else {
              TmpVn = VN0;
            } 
            VnAssigned = VnAssigned | (TmpVn << TmpSoc0) | (TmpVn << TmpSoc1);
            SocVnAssigned = SocVnAssigned | (1 << TmpSoc0) | (1 << TmpSoc1);
            break;
          }
        }
      }
    }
  }

  //
  // For the nodes that are part of only one ring, pair it with immediate neighbor and assign the VN to both of them.
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; ++Idx1) {
    // 
    // Check if any of the nodes of this ring is assigned a VN already
    // 
    TmpVn = 0xFF;
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      if (SocVnAssigned & (1 << TmpSoc0)) {
        TmpVn = (((VnAssigned >> TmpSoc0) & 0x1) == VN0) ? VN1 : VN0;
        break;
      }
    }

    if (TmpVn == 0xFF) {
      // 
      // If none of the nodes of this ring is assigned an VN, then this ring has nodes that are part of only this ring.
      // Assign VN0 to first two nodes of the ring and VN1 to the remaining two nodes of the ring.
      // 
      for (Idx2 = 0; Idx2 < 4; ++Idx2) {
        TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
        VnAssigned = VnAssigned | ((Idx2 /2) << TmpSoc0);
        SocVnAssigned = SocVnAssigned | (1 << TmpSoc0);
      }
    } else {
      // 
      // This means either two nodes or all 4 nodes are already assigned VN. In case 1, assign VN to the 
      // remaining 2 nodes. For case 2, nothing to do.
      //
      for (Idx2 = 0; Idx2 < 4; ++Idx2) {
        TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
        if ((SocVnAssigned & (1 << TmpSoc0)) == 0) {
          VnAssigned = VnAssigned | (TmpVn << TmpSoc0);
          SocVnAssigned = SocVnAssigned | (1 << TmpSoc0);
        }
      }
    }
  }

  //
  // Verify that all nodes of a ring is assigned a VN; otherwise it is fatal condition.
  //
  for (Idx1 = 0; Idx1 < DiscEngData->TotRings; ++Idx1) {
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      TmpSoc0 = DiscEngData->Rings[Idx1][Idx2];
      if ((SocVnAssigned & (1 << TmpSoc0)) == 0) {
        QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n  CPU%u VN Allocation Failed - Topology Not Supported!!! \n  System Halted", TmpSoc0));
        QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
    }
  }

  // Copy the assignedVN into Disc Eng structure
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      DiscEngData->Cpu[Idx1].Vn = (VnAssigned >> Idx1) & 0x1;
    }
  }

  //
  // If there are no rings, all sockets would have been assigned VN0 which will cause deadlock in some
  // degraded topology scenarios.  Assign VN1 for half of the populated sockets.
  // (Non-degraded topologies will not have this problem since they will have rings and VN will be assigned correctly).
  //
  VnAssigned = VN1;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if ((DiscEngData->Cpu[Idx1].Valid == TRUE) && ((SocVnAssigned & (1 << Idx1)) == 0)) { 
        DiscEngData->Cpu[Idx1].Vn = VnAssigned;
        (VnAssigned == VN0) ? (VnAssigned = VN1) : (VnAssigned = VN0); // Toggle between VN0& VN1
      }
  }

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  VN Assignment:"));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    %u - VN%u", Idx1, DiscEngData->Cpu[Idx1].Vn));
    }
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ComputeTopologyTree (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA            *DiscEngData
  )
/*++

Routine Description:

  Computes the QPI CPU topology found in the system   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  Parent;
  UINT8               Idx1, Idx2, Hop, RootSocId, Ring4S;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  Constructing Topology Tree"));
  QpiCheckPoint (0xFF,  SOCKET_TYPE_CPU, 0, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CONSTRUCT_TOPOLOGY_TREE, host);

  //
  // Add each populated socket as Root Node in its topology tree
  //
  Parent.RawNode          = 0;
  Parent.Node.Valid       = TRUE;
  Parent.Node.SocType     = SOCKET_TYPE_CPU;
  Parent.Node.ParentPort  = 0xF;
  Parent.Node.Hop         = 0;
  Parent.Node.ParentIndex = 0x7F;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      Parent.Node.SocId = Idx1;
      if (AddNodeToTopologyTree (host, DiscEngData->CpuTree[Idx1], &Parent) != QPI_SUCCESS) {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n;  Adding CPU%u to Topology Discovery Tree of CPU%u failed - Data Structure Overflow.",
                          Parent.Node.SocId, Parent.Node.SocId));
        QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
    }
  }

  //
  // Do the topology discovery using Breadth First Search (BSF). To start with, the Topology Tree must
  // have the root socket already added. The TopologyTree will grow as we discover new sockets in BSF manner.
  // 4S ring topology is a special case; to avoid deadlock, we need to construct the tree for sockets in the
  // order they appear in the ring. For non-degraded 6S/8S configs, we will use the intel suggested
  // base line routing.
  //

#if MAX_CPU_SOCKETS > 4
  if (DiscEngData->TotRings == 4) { // 8S
    ComputeTopologyTree8S (host, QpiInternalGlobal, DiscEngData);
  } else if (DiscEngData->TotRings == 2 && DiscEngData->TotCpus == 6) { // 6S
    ComputeTopologyTree6S (host, DiscEngData);
  } else {
#endif

    //
    // Identify if this is a 4S with ring topology
    //
    if (DiscEngData->TotRings == 1 && DiscEngData->CpuList == 0xF) {
      Ring4S = TRUE;
    } else {
      Ring4S = FALSE;
    }
    for (Hop = 0; Hop < MAX_CPU_SOCKETS; ++Hop) {
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if (Ring4S == TRUE) {
          if (Idx1 < 4) {
            RootSocId = DiscEngData->Rings[0][Idx1];
          } else {
            break;
          }
        } else {
          RootSocId = Idx1;
        }
        if (DiscEngData->Cpu[RootSocId].Valid == TRUE) {
          for (Idx2 = 0; DiscEngData->CpuTree[RootSocId][Idx2].Node.Valid != FALSE; ++Idx2) {
            Parent = DiscEngData->CpuTree[RootSocId][Idx2];
            if (Parent.Node.Hop < Hop) {
              continue;
            } else  if (Parent.Node.Hop > Hop) {
              break;
            }
            ExploreThisNode (host, DiscEngData, RootSocId, &Parent, Idx2, Hop);
          }
        }
      }
    }

#if MAX_CPU_SOCKETS > 4
  }
#endif

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ExploreThisNode (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                      RootSocId,
  TOPOLOGY_TREE_NODE  *Parent,
  UINT8                      ParentIdx,
  UINT8                      Hop
  )
/*++

Routine Description:

  Computes the QPI CPU topology found in the system   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   RootSocId - Root Socket ID
   Parent - QPI fabric tree structure
   ParentIdx - parent ID
   Hop - Hops in path
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  Child;
  UINT8               QpiLink;
  QPI_LINK_DATA       ParentLep;

    Child.RawNode    = 0;
    Child.Node.Valid = TRUE;
    Child.Node.Hop = Parent->Node.Hop + 1; // One level down from parent
    Child.Node.ParentIndex = ParentIdx; // Remember who is the immediate parent of this child

    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) { // Explore all the links of this parent
      ParentLep = DiscEngData->Cpu[Parent->Node.SocId].LinkData[QpiLink];

      //
      // Skip this link if it is marked as "Not Valid" or "DualLink".
      // A Link is marked as "Not Valid" (before entering this function) when we don't
      // want to explore it. Also, for SV validation purpose, we might want to mark a link as "Not Valid"
      // to simulate different topology. When there is a dual-link exists between any two CPU sockets,
      // one of the link is marked as "DualLink" at both the ends.
      // Exploring only one link of a dual-link is sufficient to identify the neighbor.
      //
      if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE) {
        continue;
      }

      //
      // Get the Child Socket Id, Type and port that connects it to the parent form LEP
      //
      Child.Node.SocId      = ParentLep.PeerSocId;
      Child.Node.SocType    = ParentLep.PeerSocType;
      Child.Node.ParentPort = ParentLep.PeerPort;

      //
      // Make sure that this socket is not already found in the tree between hop level 0 and parent's hop level.
      // The rational is that, if we find child socket already at that level, then we have a shorter path to this child.
      //
      if (ShorterPathExists (host, DiscEngData->CpuTree[RootSocId], &Child) == TRUE) {
        continue;
      }

      //
      // Finally, if we have multiple paths that are of equal hop length, then choose
      // the one that is used least number of times; if they are used equal number of times, then
      // choose the one that goes thru the minimum numberd socket.
      //
      if (CheckAndAddNodeToTopologyTree (host, DiscEngData, RootSocId, Parent, &Child, Hop) != QPI_SUCCESS) {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n;  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                              Child.Node.SocId, RootSocId));
        QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
    } // for(QpiLink...)

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
CheckAndAddNodeToTopologyTree (
  struct sysHost           *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                    RootSocId,
  TOPOLOGY_TREE_NODE        *Parent,
  TOPOLOGY_TREE_NODE       *LeafNode,
  UINT8                      Hop
  )
/*++

Routine Description:

  Adds current QPI node to QPI system fabric

Arguments:

  host  - Pointer to the system host (root) structure
  DiscEngData - Pointer to socket specific data 
  RootSocId - Root Socket ID
  Parent      - QPI fabric tree structure
  LeafNode    - QPI fabric tree structure
  Hop - Hops in path

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8                 Idx1;
  UINT8                 Path1Usage, Path2Usage;
  TOPOLOGY_TREE_NODE    *Tree;

  QPI_ASSERT ((DiscEngData != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((LeafNode != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  Tree = DiscEngData->CpuTree[RootSocId];

  for (Idx1 = 0; Idx1 < MAX_TREE_NODES; ++Idx1) {
    //
    // If the LeafNode is not already found in the tree, add it to the tree
    //
    if (Tree[Idx1].Node.Valid == FALSE) {
      Tree[Idx1] = *LeafNode;
      if (Hop != 0) {
        TrackPathLoad (host, DiscEngData, Tree, LeafNode, 0);
      }
      return QPI_SUCCESS;
    }

    //
    // If Socket ID, Type & Hop level of the node to be added matches a node in the tree 
    // (i.e the LeafNode is already in the tree with different path to the root), then choose 
    // the path that is used minimum number of times. 
    //
    if (Tree[Idx1].Node.SocId   == LeafNode->Node.SocId   && 
        Tree[Idx1].Node.SocType == LeafNode->Node.SocType &&
        Tree[Idx1].Node.Hop     == LeafNode->Node.Hop) {

      //
      // Find the immediate parent of the leaf nodes and check how many times the immediate
      // parent used the link that connects it to the leaf node
      //
      Path1Usage = CalculatePathLoad (host, DiscEngData, Tree, &Tree[Idx1]);

      Path2Usage = CalculatePathLoad (host, DiscEngData, Tree, Parent) + (UINT8)LeafNode->Node.Hop;

      //
      // Select the path that is used minimum number of times.
      //
      if (Path2Usage < Path1Usage) {
        //
        // Replace the existing node with the leaf node
        //
        TrackPathLoad (host, DiscEngData, Tree, &Tree[Idx1], 1);
        Tree[Idx1] = *LeafNode;
        TrackPathLoad (host, DiscEngData, Tree, LeafNode, 0);
        return QPI_SUCCESS;
      } else {
        //
        // Existing path is optimum. Leave the tree intact
        //
        return QPI_SUCCESS;
      } 
    }
  }

  return (QPI_STATUS) QPI_FAILURE;
}

STATIC
QPI_STATUS
TrackPathLoad (
  struct sysHost       *host,
  QPI_DISC_DATA        *DiscEngData,
  TOPOLOGY_TREE_NODE   *Tree,
  TOPOLOGY_TREE_NODE   *LeafNode,
  UINT8                      Adjust
  )
/*++

Routine Description:

  Checks and resolves potential dead locks due to overlapping routes  

Arguments:

  host  - Pointer to the system host (root) structure
  DiscEngData - Pointer to socket specific data 
  Tree        - QPI fabric tree structure
  LeafNode    - QPI fabric tree structure
  Adjust      - 1 = Remove, 0 = Add to linkload

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8               Port0; // Parent Port that connects it to the child
  TOPOLOGY_TREE_NODE  Child, Parent;

  Child = *LeafNode;
  while (Child.Node.Hop != 0) {
    Parent = Tree[Child.Node.ParentIndex];
    Port0 = GetChildLocalPortFromLEP (host, DiscEngData->Cpu, &Parent, &Child);
    QPI_ASSERT ((Port0 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    if (Adjust == 0) {
      ++DiscEngData->LinkLoad[Parent.Node.SocId][Port0];
    } else {
      QPI_ASSERT ((DiscEngData->LinkLoad[Parent.Node.SocId][Port0] != 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      --DiscEngData->LinkLoad[Parent.Node.SocId][Port0];
    }
    Child = Parent;
  }

  return QPI_SUCCESS;
}

STATIC
UINT8
CalculatePathLoad (
  struct sysHost       *host,
  QPI_DISC_DATA        *DiscEngData,
  TOPOLOGY_TREE_NODE   *Tree,
  TOPOLOGY_TREE_NODE   *LeafNode
  )
/*++

Routine Description:

  Checks and resolves potential dead locks due to overlapping routes  

Arguments:

  host  - Pointer to the system host (root) structure
  DiscEngData - Pointer to socket specific data 
  Tree        - QPI fabric tree structure
  LeafNode    - QPI fabric tree structure

Returns:

--*/
{
  UINT8               Port0; // Parent Port that connects it to the child
  UINT8               Usage;
  TOPOLOGY_TREE_NODE  Child, Parent;

  Usage = 0;
  Child = *LeafNode;
  while (Child.Node.Hop != 0) {
    Parent = Tree[Child.Node.ParentIndex];
    Port0 = GetChildLocalPortFromLEP (host, DiscEngData->Cpu, &Parent, &Child);
    QPI_ASSERT ((Port0 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    Usage = Usage + DiscEngData->LinkLoad[Parent.Node.SocId][Port0];
    Child = Parent;
  }

  return Usage;
}

STATIC
QPI_STATUS
DetectDeadlock (
  struct sysHost      *host,
  QPI_DISC_DATA       *DiscEngData
  )
/*++

Routine Description:
  Treat the topology tree as a Directed Graph and check if the Graph is Directed Acylic Graph (DAG)
  or not using "Topological Sort" algorithm. If the graph is acyclic it indicates the routes are deadlock free.
  The Directed Graph consists of finite set of vertices V and Edges E. In QPI terminology, source/sink of
  QPI packets represent Vertices and the link connecting them represent Edges. So V & E for all
  supported topology is finite set as defined below:

      V = {S0...S7}{P0...P2}{VN0/VN1}{TX/RX} = 8 * 3 * 2 * 2 = 96
      E = {S0...S7}{P0...P2}{VN0/VN1} = 8 * 3 * 2 = 48

  The topology tree is converted into an array consisting the list of adjacent nodes of each node. Since
  each node has 3 links and 2 VNs, a node can have maximum 6 outgoing edges; so it can have maximum
  of 6 nodes in its adjacency list.

  Example: Consider the 4S ring topology

       0   0
    S0 ----- S1
    1|       |1
     |       |
     |       |
     |       |
    1|       |1
    S3 ----- S2
       0   0

   The route/topology tree for each socket would be:
    "S0 P0 VN0 TX" -> "S1 P0 VN0 RX";
    "S0 P1 VN0 TX" -> "S3 P1 VN0 RX";
    "S0 P0 VN0 TX" -> "S1 P0 VN0 RX" -> "S1 P1 VN0 TX" -> "S2 P1 VN0 RX";

    "S1 P0 VN0 TX" -> "S0 P0 VN0 RX";
    "S1 P1 VN0 TX" -> "S2 P1 VN0 RX";
    "S1 P0 VN0 TX" -> "S0 P0 VN0 RX" -> "S0 P1 VN0 TX" -> "S3 P1 VN0 RX";

    "S2 P0 VN0 TX" -> "S3 P0 VN0 RX";
    "S2 P1 VN0 TX" -> "S1 P1 VN0 RX";
    "S2 P0 VN0 TX" -> "S3 P0 VN0 RX" -> "S3 P1 VN0 TX" -> "S0 P1 VN0 RX";

    "S3 P0 VN0 TX" -> "S2 P0 VN0 RX";
    "S3 P1 VN0 TX" -> "S0 P1 VN0 RX";
    "S3 P0 VN0 TX" -> "S2 P0 VN0 RX" -> "S2 P1 VN0 TX" -> "S1 P1 VN0 RX";

    The above topology tree can be represented as an array of adjacency list as shown below:
    Node           Adjacency List
    S0 P0 VN0 TX   S1 P0 VN0 RX
    S0 P0 VN0 RX   S0 P1 VN0 TX
    S0 P1 VN0 TX   S3 P1 VN0 RX
    S1 P0 VN0 TX   S0 P0 VN0 RX
    S1 P0 VN0 RX   S1 P1 VN0 TX
    S1 P1 VN0 TX   S2 P1 VN0 RX
    S2 P0 VN0 TX   S3 P0 VN0 RX
    S2 P0 VN0 RX   S2 P1 VN0 TX
    S2 P1 VN0 TX   S1 P1 VN0 RX
    S3 P0 VN0 TX   S2 P0 VN0 RX
    S3 P0 VN0 RX   S3 P1 VN0 TX
    S3 P1 VN0 TX   S0 P1 VN0 RX

    While this particular topology has only one adjacent node for each node, complex topologies such as 8SG will
    have more than one node in the adjacency list.

    By applying the topological sort on the above table, it can be detected if it is DAG or not.

Arguments:
   
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data

Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               Idx1, Idx2, Idx3;
  UINT8               GrandParentIdx, ParentIdx, ChildIdx;
  UINT8               Port1, Port2;
  UINT8               RootVn, Temp1;
  UINT8               Vertices[MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2];
  UINT8               AdjList[MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2][MAX_QPI_PORTS * 2];
  UINT8               Indegree[MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2];
  BOOLEAN             DeadlockFound;
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  *Parent, *Child;

  MemSetLocal ((VOID *) Vertices, 0x0, sizeof (Vertices));
  MemSetLocal ((VOID *) AdjList, 0xFF, sizeof (AdjList));
  MemSetLocal ((VOID *) Indegree, 0x0, sizeof (Indegree));

  //
  // Construct the adjacency list. Note that the pass thru route is split into two separate independent nodes;
  // one containing the RX and the other containing the TX at the intermediate node. The list is contructed
  // by parsing the topology tree of each socket. In other words, the graph is nothing but collection of the
  // topology tree of each node.
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      //
      // VN assignment is at the soure (root node) and they don't change along the path. Identify the
      // root node's VN assignment.
      //
      TopologyTree = DiscEngData->CpuTree[Idx1];
      RootVn = DiscEngData->Cpu[TopologyTree[0].Node.SocId].Vn;

      //  
      // Traverse the topology tree of this socket and mark the Vertices found in its tree.
      // 
     for (Idx2 = 1; TopologyTree[Idx2].Node.Valid != FALSE; ++Idx2) {
        // 
        // Extract the Parent and Child nodes
        // 
        Child = &TopologyTree[Idx2];
        Port2 = (UINT8)Child->Node.ParentPort; // Child's Port that connects it to the parent
        Port1 = (UINT8)DiscEngData->Cpu[Child->Node.SocId].LinkData[Port2].PeerPort; // Parent's Port that connects it to this child
        Parent = &TopologyTree[Child->Node.ParentIndex];

        // 
        // Mark the corresponding vertices of the parent & child as valid.
        // Note that the parent is always at "TX" node and the child is always at "RX" node. 
        // 
        ParentIdx = (UINT8)(Parent->Node.SocId * MAX_QPI_PORTS * 2 * 2) + (Port1 * 2 * 2) + (RootVn * 2) + TX;
        ChildIdx = (UINT8)(Child->Node.SocId * MAX_QPI_PORTS * 2 * 2) + (Port2 * 2 * 2) + (RootVn * 2) + RX;
        Vertices[ParentIdx] = TRUE;
        Vertices[ChildIdx] = TRUE;

        // 
        // Record the child as adjacent node in parent's adjacency list if it is not already found. 
        // 
        for (Idx3 = 0; Idx3 < MAX_QPI_PORTS * 2; ++Idx3) {
          if (AdjList[ParentIdx][Idx3] == ChildIdx) {
            break;
          } else if (AdjList[ParentIdx][Idx3] == 0xFF) {
            AdjList[ParentIdx][Idx3] = ChildIdx;
            break;
          } 
        }

        // 
        // If the parent is not the root node, make sure that the pass thru route is split into separate nodes corresponding
        // to RX & TX and recorded in the adjacency list.
        // 
        if (Parent->Node.Hop != 0) {
          Port1 = (UINT8)Parent->Node.ParentPort; // Port connecting the parent to grand parent
          GrandParentIdx = (UINT8)(Parent->Node.SocId * MAX_QPI_PORTS * 2 * 2) + (Port1 * 2 * 2) + (RootVn * 2) + RX;
          for (Idx3 = 0; Idx3 < MAX_QPI_PORTS * 2; ++Idx3) {
            if (AdjList[GrandParentIdx][Idx3] == ParentIdx) {
              break;
            } else if (AdjList[GrandParentIdx][Idx3] == 0xFF) {
              AdjList[GrandParentIdx][Idx3] = ParentIdx;
              break;
            } 
          }
        }

      }
    }
  }


  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; Adjacency Table"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; ----------------"));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2; ++Idx1) {
    if (Vertices[Idx1] == TRUE) {
      Temp1 = Idx1 % (MAX_QPI_PORTS * 2 * 2);
      Port1 = Temp1 / (2 * 2);
      RootVn = (Temp1 % (2 * 2)) / 2;
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;S%u P%u VN%u %s (%02u) :", Idx1 / (MAX_QPI_PORTS * 2 * 2), Port1, RootVn, (Idx1 & 0x1) ? "RX" : "TX", Idx1));
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS * 2; ++Idx2) {
        if (AdjList[Idx1][Idx2] != 0xFF) {
          Temp1 = AdjList[Idx1][Idx2] % (MAX_QPI_PORTS * 2 * 2);
          Port2 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   S%u P%u VN%u %s (%02u)", AdjList[Idx1][Idx2] / (MAX_QPI_PORTS * 2 * 2), Port2, RootVn,
              (AdjList[Idx1][Idx2] & 0x1) ? "RX" : "TX", AdjList[Idx1][Idx2]));
        }
      }
    }
  }


  //
  // Do topological sort of the graph. A topological sort is an ordering of the vertices in a directed graph
  // such that if there is a path from node i to j, then node j appears after node i in the ordering.
  // If there is a cycle in the graph then such ordering is not possible and it indicates a deadlock in the route.
  // Note that we are not interested in doing the actual sorting. We just knock out the vertices with indegree
  // 0 and repeat the loop for all nodes. At the end if we find any node with non-zero indegree, it implies
  // there is a cycle in the graph. Also note that the list of nodes in the array is nothing but the nodes that
  // constitue the cycle in the graph.
  //

  //
  // Construct a table containing the indgree of each node.
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2; ++Idx1) {
    if (Vertices[Idx1] == TRUE) {
      for (Idx3 = 0; Idx3 < MAX_QPI_PORTS * 2; ++Idx3) {
        if (AdjList[Idx1][Idx3] == 0xFF) {
          break;
        } else {
          Indegree[AdjList[Idx1][Idx3]]++;
        }
      }
    }
  }


  //
  // Remove all nodes with indegree zero and all its outgoing edges by decreasing the indegree of the nodes
  // that are connected by the edges being removed.
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2; ++Idx1) {
    for (Idx2 = 0; Idx2 < MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2; ++Idx2) {
      if (Vertices[Idx2] == TRUE && Indegree[Idx2] == 0) {
        Vertices[Idx2] = FALSE;
        for (Idx3 = 0; Idx3 < MAX_QPI_PORTS * 2; ++Idx3) {
          if (AdjList[Idx2][Idx3] == 0xFF) {
            break;
          } else {
            Indegree[AdjList[Idx2][Idx3]]--;
          }
        }
      }
    }
  }

  //
  // Check for cycle and print the nodes that form the cycle. If we find any valid node in the vertices it indicates a cycle.
  //
  DeadlockFound = FALSE;
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; Checking for Deadlock..."));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS * MAX_QPI_PORTS * 2 * 2; ++Idx1) {
    if (Vertices[Idx1] == TRUE) {
      DeadlockFound = TRUE;
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS * 2; ++Idx2) {
        if (AdjList[Idx1][Idx2] == 0xFF) {
          break;
        } else {
          Temp1 = Idx1 % (MAX_QPI_PORTS * 2 * 2);
          Port1 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;S%u P%u VN%u %s ->  ", Idx1 / (MAX_QPI_PORTS * 2 * 2), Port1, RootVn, (Idx1 & 0x1) ? "RX" : "TX"));
          Temp1 = AdjList[Idx1][Idx2] % (MAX_QPI_PORTS * 2 * 2);
          Port2 = Temp1 / (2 * 2);
          RootVn = (Temp1 % (2 * 2)) / 2;
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "S%u P%u VN%u %s", AdjList[Idx1][Idx2] / (MAX_QPI_PORTS * 2 * 2), Port2, RootVn, (AdjList[Idx1][Idx2] & 0x1) ? "RX" : "TX"));
        }
      }
    }
  }

  if (DeadlockFound == TRUE) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n Dead Lock Found!!! \n Topology Not Supported. System Halted\n"));
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
CopyTopologyTree (
  struct sysHost      *host,
  QPI_SOCKET_DATA      *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA       *DiscEngData
  )
/*++

Routine Description:

Arguments:
   
   host  - Pointer to the system host (root) structure
   SocketData - Pointer to socket specific data
   QpiInternalGlobal - QPIRC internal variables.
   DiscEngData - Pointer to socket specific data

Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               Idx1, Idx2;
  UINT8               TempPort;

  //
  // Copy the Si agnostic toplogy tree created by the discovery engine and apply the port mapping to make Si specific.
  //
  MemCopy ((UINT8 *) SocketData->CpuTree, (UINT8 *) DiscEngData->CpuTree, sizeof (SocketData->CpuTree));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 1; DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE; ++Idx2) { // Skip the root node since it doesn't have parent
        TempPort = (UINT8) DiscEngData->CpuTree[Idx1][Idx2].Node.ParentPort;
        SocketData->CpuTree[Idx1][Idx2].Node.ParentPort = TranslateToSiPort (host, TempPort);
      }
    }
  }

  // Copy the VN assigned
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      SocketData->Cpu[Idx1].Vn = DiscEngData->Cpu[Idx1].Vn;
    }
  }

  // Copy the LEP Info and Topology Tree Info into host structure
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      host->var.qpi.CpuInfo[Idx1].Valid  = SocketData->Cpu[Idx1].Valid;
      host->var.qpi.CpuInfo[Idx1].SocId  = SocketData->Cpu[Idx1].SocId;
      MemCopy ((UINT8 *) host->var.qpi.CpuInfo[Idx1].LepInfo, (UINT8 *) SocketData->Cpu[Idx1].LinkData, sizeof (host->var.qpi.CpuInfo[Idx1].LepInfo));
      MemCopy ((UINT8 *) host->var.qpi.CpuInfo[Idx1].TopologyInfo, (UINT8 *) SocketData->CpuTree[Idx1], sizeof (host->var.qpi.CpuInfo[Idx1].TopologyInfo));
    }
  }

  TopologyInfoDump (host, SocketData->Cpu, SocketData->CpuTree, TRUE);

  return QPI_SUCCESS;
}

#if MAX_CPU_SOCKETS > 4

BOOLEAN
CheckChildNodeInThisRoute (
  UINT8               RootSocket, 
  TOPOLOGY_TREE_NODE  *Parent,
  TOPOLOGY_TREE_NODE  *Child,
  UINT8               (*OemRoute)[4][2]
  )
/*++

Routine Description:

  Check this Child Node is in this route   

Arguments:
   Child  - Pointer to child topology tree node
   DiscEngData - Pointer to socket specific data 
   
Returns:

   TRUE - yes
   FALSE - no

--*/
{
   BOOLEAN       AddNode;
   UINT8         PathNum;

   //
   // If it is 1 Hop away node (i.e immediate neighbor), just add the node
   //
  AddNode = FALSE;
  if (Child->Node.Hop == 1) {
    AddNode = TRUE;
  } else {
    //
    // If it is 2 Hop away node, make sure the template routing indicates this route to be used
    //
    for (PathNum = 0; PathNum < 4; ++PathNum) {
      if (OemRoute[RootSocket][PathNum][0] == Parent->Node.SocId && OemRoute[RootSocket][PathNum][1] == Child->Node.SocId) {
        AddNode = TRUE;
        break;
      }
    }
  }

  return AddNode;
}

BOOLEAN
CheckChildNodeInThisRoute_VN1Disable (
  QPI_DISC_DATA       *DiscEngData,
  UINT8               RootSocket, 
  TOPOLOGY_TREE_NODE  *Parent,
  TOPOLOGY_TREE_NODE  *Child,
  UINT8               (*OemRoute_VN1Disable)[4][3]
  )
/*++

Routine Description:

  Check this Child Node is in this route   

Arguments:
   Child  - Pointer to child topology tree node
   DiscEngData - Pointer to socket specific data 
   
Returns:

   TRUE - yes
   FALSE - no

--*/
{
  BOOLEAN       AddNode;
  UINT8         PathNum;
  TOPOLOGY_TREE_NODE  GrandParent;

  //
  // If it is 1 Hop away node (i.e immediate neighbor), just add the node
  //
  AddNode = FALSE;
  if (Child->Node.Hop == 1) {
    AddNode = TRUE;
  } else if (Child->Node.Hop == 2) {
    //
    // If it is 2 Hop away node, make sure the template routing indicates this route to be used
    //
    for (PathNum = 0; PathNum < 4; ++PathNum) {
      if (OemRoute_VN1Disable[RootSocket][PathNum][0] == Parent->Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][1] == Child->Node.SocId ) {
          AddNode = TRUE;
          break;
      }
    }
  } else {
    //
    // If it is 3 Hop away node, make sure the template routing indicates this route to be used
    //
    GrandParent = DiscEngData->CpuTree[RootSocket][Parent->Node.ParentIndex];
    for (PathNum = 0; PathNum < 4; ++PathNum) {
      if (OemRoute_VN1Disable[RootSocket][PathNum][0] == GrandParent.Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][1] == Parent->Node.SocId &&
          OemRoute_VN1Disable[RootSocket][PathNum][2] == Child->Node.SocId) {
        AddNode = TRUE;
        break;
      }
    }
  }

  return AddNode;
}


STATIC
QPI_STATUS
ComputeTopologyTree8S (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA            *DiscEngData
  )
/*++

Routine Description:

  Computes the QPI CPU topology found in the system   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8         OemRoute_VN1Disable[MAX_CPU_SOCKETS][4][3];      
  UINT8         OemRoute[MAX_CPU_SOCKETS][4][2];
  UINT8         Idx1, Idx2, QpiLink, MaxHop;
  TOPOLOGY_TREE_NODE  Parent, Child;
  QPI_LINK_DATA       ParentLep;
  BOOLEAN       AddNode;

  MemSetLocal ((VOID *) OemRoute, 0xFF, sizeof (OemRoute));
  MemSetLocal ((VOID *) OemRoute_VN1Disable, 0xFF, sizeof (OemRoute_VN1Disable));
  Map8SNodes (host, DiscEngData);
  
  if( QpiInternalGlobal->Vn1En == TRUE) {
  Map8SRoutes (host, DiscEngData, OemRoute);
    MaxHop = 2;
  } else {  
    Map8SRoutes_VN1Disable(host, DiscEngData, OemRoute_VN1Disable);
    MaxHop = 3;
  }


  //
  // Construct the topology tree that uses the template routing
  //
  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE && DiscEngData->CpuTree[Idx1][Idx2].Node.Hop < MaxHop; ++Idx2) {

        Parent = DiscEngData->CpuTree[Idx1][Idx2];
        Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
        Child.Node.ParentIndex = Idx2; // Remember who is the immediate parent of this child

        for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) { // Explore all the links of this parent

          ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[QpiLink];
          if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE) {
            continue;
          }

          //
          // Get the Child Socket Id, Type and port that connects it to the parent form LEP
          //
          Child.Node.SocId      = ParentLep.PeerSocId;
          Child.Node.SocType    = ParentLep.PeerSocType;
          Child.Node.ParentPort = ParentLep.PeerPort;

          //
          // Make sure that this socket is not already found in the tree between hop level 0 and parent's hop level.
          // The rational is that, if we find child socket already at that level, then we have a shorter path to this child.
          //
          if (ShorterPathExists (host, DiscEngData->CpuTree[Idx1], &Child) == TRUE) {
            continue;
          }

          if( QpiInternalGlobal->Vn1En == TRUE) {
             AddNode = CheckChildNodeInThisRoute(Idx1, &Parent, &Child, OemRoute);
          } else {
             AddNode = CheckChildNodeInThisRoute_VN1Disable(DiscEngData, Idx1, &Parent, &Child, OemRoute_VN1Disable);
          }

          if (AddNode == FALSE) {
            continue;
          }

          if (AddNodeToTopologyTree (host, DiscEngData->CpuTree[Idx1], &Child) != QPI_SUCCESS) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                              Child.Node.SocId, Idx1));
            QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          }
        } // for(QpiLink...)
      }
    }
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
Map8SNodes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  )
/*++

Routine Description:

  This routine establishes a mapping between intel's ref platform and this platform. Port number is not relevant.

  Ref Platform:

     +-------------------------+
     |                         |
    S0 ----- S1       S6 ----- S7
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S2 ----- S3       S4 ----- S5
     |                         |
     +-------------------------+


  OEM Platform:

     +-------------------------+
     |                         |
    S1 ----- S2       S3 ----- S6
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S5 ----- S7       S0 ----- S4
     |                         |
     +-------------------------+



  Due to isomorphic nature of the 8SG topology, the OEM Platform can be redrawn as:

     +-------------------------+
     |                         |
    S0 ----- S4       S1 ----- S2
     |       | \     /|        |
     |       |   \ /  |        |
     |       |    \   |        |
     |       |  /   \ |        |
     |       |/      \|        |
    S3 ----- S6       S5 ----- S7
     |                         |
     +-------------------------+


  So the socket mapping would be:

    Intel   OEM
      S0     S0
      S1     S4
      S2     S3
      S3     S6
      S4     S5
      S5     S7
      S6     S1
      S7     S2

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
      
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               Idx1, QpiLink;
  UINT8               RingNum1, RingNum2, RingNum3;
  UINT8               RefSoc;
  QPI_LINK_DATA       ParentLep;
  UINT8               TempSoc1 =0 , TempSoc2=0;
  

  //
  // Begin with S0 of this platform, identify its pair and map it to S0, S2 of ref platform
  //
  RefSoc = 0;
  DiscEngData->SocMap[0] = RefSoc;
  DiscEngData->SocMap[2] = DiscEngData->RingNodePairs[RefSoc];

  // Identify S0's two rings present in this platform 
  RingNum1 = DiscEngData->RingInfo[RefSoc][1];
  RingNum2 = DiscEngData->RingInfo[RefSoc][2];

  // Identify the other 2 nodes of S0's first ring
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (DiscEngData->Rings[RingNum1][Idx1] == RefSoc || DiscEngData->Rings[RingNum1][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum1][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's first ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[QpiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S1 and S3 in intel ref platform
  DiscEngData->SocMap[1] = TempSoc1;
  DiscEngData->SocMap[3] = TempSoc2;


  // Identify the other 2 nodes of S0's second ring
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (DiscEngData->Rings[RingNum2][Idx1] == RefSoc || DiscEngData->Rings[RingNum2][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum2][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's second ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[QpiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S7 and S5 in intel ref platform
  DiscEngData->SocMap[7] = TempSoc1;
  DiscEngData->SocMap[5] = TempSoc2;


  RefSoc = DiscEngData->SocMap[1];

  // Identify RefSoc's ring that doesn't belong to S0
  if (DiscEngData->RingInfo[RefSoc][1] == RingNum1 || DiscEngData->RingInfo[RefSoc][1] == RingNum2) {
    RingNum3 = DiscEngData->RingInfo[RefSoc][2];
  } else {
    RingNum3 = DiscEngData->RingInfo[RefSoc][1];
  }

  // Identify the other 2 nodes ofRefSoc's ring
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (DiscEngData->Rings[RingNum3][Idx1] == RefSoc || DiscEngData->Rings[RingNum3][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    TempSoc1 = DiscEngData->Rings[RingNum3][Idx1];
    break;
  }
  TempSoc2 = DiscEngData->RingNodePairs[TempSoc1];

  //
  // Make sure TempSoc2 is the diagonally opposite in RefSoc's ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[QpiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S4 and S6 in intel ref platform
  DiscEngData->SocMap[4] = TempSoc1;
  DiscEngData->SocMap[6] = TempSoc2;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    Socket Mapping"));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;      %u  -  %u ", Idx1, DiscEngData->SocMap[Idx1]));
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
Map8SRoutes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8         (*OemRoute)[4][2]
  )
/*++

Routine Description:

  Uses the following static routing as template and produces the routing for the given platform
  using the socket mapping.
    
  S0 -> S2 -> S3;   S0 -> S7 -> S6;   S0 -> S1 -> S4;   S0 -> S2 -> S5
  S1 -> S3 -> S2;   S1 -> S3 -> S6;   S1 -> S4 -> S5;   S1 -> S0 -> S7
  S2 -> S0 -> S1;   S2 -> S3 -> S6;   S2 -> S5 -> S4;   S2 -> S0 -> S7
  S3 -> S1 -> S0;   S3 -> S1 -> S4;   S3 -> S2 -> S5;   S3 -> S6 -> S7
  S4 -> S1 -> S0;   S4 -> S5 -> S2;   S4 -> S6 -> S3;   S4 -> S6 -> S7
  S5 -> S7 -> S0;   S5 -> S2 -> S3;   S5 -> S4 -> S1;   S5 -> S7 -> S6
  S6 -> S7 -> S0;   S6 -> S3 -> S2;   S6 -> S4 -> S1;   S6 -> S4 -> S5
  S7 -> S5 -> S2;   S7 -> S6 -> S3;   S7 -> S0 -> S1;   S7 -> S5 -> S4

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   OemRoute - 
      
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8         RefRoute[8][4][2] = {
                {{2, 3}, {7, 6}, {1, 4}, {2, 5}}, // S0
                {{3, 2}, {3, 6}, {4, 5}, {0, 7}}, // S1
                {{0, 1}, {3, 6}, {5, 4}, {0, 7}}, // S2
                {{1, 0}, {1, 4}, {2, 5}, {6, 7}}, // S3
                {{1, 0}, {5, 2}, {6, 3}, {6, 7}}, // S4
                {{7, 0}, {2, 3}, {4, 1}, {7, 6}}, // S5
                {{7, 0}, {3, 2}, {4, 1}, {4, 5}}, // S6
                {{5, 2}, {6, 3}, {0, 1}, {5, 4}}  // S7
             };
  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      for (Idx3 = 0; Idx3 < 2; ++Idx3) {
        OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n"));
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "  S%u -> S%u -> S%u", Idx1, OemRoute[Idx1][Idx2][0], OemRoute[Idx1][Idx2][1]));
      QPI_ASSERT ((OemRoute[Idx1][Idx2][0] != 0xFF && OemRoute[Idx1][Idx2][1] != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    }
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
Map8SRoutes_VN1Disable (
  struct sysHost           *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8                    (*OemRoute)[4][3]
  )
/*++

Routine Description:

  Uses the following static routing as template and produces the routing for the given platform
  using the socket mapping.
    
  S0 -> S7;  S0 -> S2 -> S5; S0 -> S1 -> S4 -> S6; S0 -> S2 -> S3   
  S1 -> S0 -> S7; S1 -> S3 -> S2 -> S5; S1 -> S4 -> S6 
  S2 -> S0 -> S7; S2 -> S5 -> S4; S2 -> S3 -> S6;  S2 -> S0 -> S1
  S3 -> S6 -> S7; S3 -> S2 -> S5; S3 -> S1 -> S4;  S3 -> S1 -> S0 
  S4 -> S5 -> S7; S4 -> S6; S4 -> S1 -> S0; S4 -> S5 -> S2 -> S3
  S5 -> S7 -> S6; S5 -> S7 -> S0; S5 -> S4 -> S1; S5 -> S2 -> S3
  S6 -> S4 -> S5; S6 -> S3 -> S2; S6 -> S7 -> S0; S6 -> S4 -> S1
  S7 -> S5 -> S4; S7 -> S5 -> S2; S7 -> S0 -> S1; S7 -> S6 -> S3

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   OemRoute - 
      
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8     RefRoute[8][4][3] = {
                {{2, 3, 0xFF}, {2, 5, 0xFF}, {1, 4,    6}, {   7, 0xFF, 0xFF}}, // S0
                {{0, 7, 0xFF}, {4, 6, 0xFF}, {3, 2,    5}, {0xFF, 0xFF, 0xFF}}, // S1
                {{0, 1, 0xFF}, {3, 6, 0xFF}, {5, 4, 0xFF}, {   0,    7, 0xFF}}, // S2
                {{1, 0, 0xFF}, {1, 4, 0xFF}, {2, 5, 0xFF}, {   6,    7, 0xFF}}, // S3
                {{1, 0, 0xFF}, {5, 7, 0xFF}, {5, 2,    3}, {   6, 0xFF, 0xFF}}, // S4
                {{7, 0, 0xFF}, {2, 3, 0xFF}, {4, 1, 0xFF}, {   7,    6, 0xFF}}, // S5
                {{7, 0, 0xFF}, {3, 2, 0xFF}, {4, 1, 0xFF}, {   4,    5, 0xFF}}, // S6
                {{5, 2, 0xFF}, {6, 3, 0xFF}, {0, 1, 0xFF}, {   5,    4, 0xFF}}  // S7
             };

  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      for (Idx3 = 0; Idx3 < 3; ++Idx3) {
        if (RefRoute[Idx1][Idx2][Idx3] != 0xFF) {
          OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
        }
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;"));
      for (Idx2 = 0; Idx2 < 4; ++Idx2) {
          if (OemRoute[Idx1][Idx2][0] != 0xFF) {
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "  S%u->S%u", Idx1, OemRoute[Idx1][Idx2][0]));          
            if (OemRoute[Idx1][Idx2][1] != 0xFF) {
              QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "->S%u", OemRoute[Idx1][Idx2][1]));
            }
            if (OemRoute[Idx1][Idx2][2] != 0xFF) {
              QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "->S%u", OemRoute[Idx1][Idx2][2]));
            }
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "; ")); 
          }
      }      
    }
  }

 
  return QPI_SUCCESS;
}


STATIC
QPI_STATUS
ComputeTopologyTree6S (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  )
/*++

Routine Description:

  Computes the QPI CPU topology found in the system   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8         OemRoute[MAX_CPU_SOCKETS][3][3];
  UINT8         Idx1, Idx2, Idx3, QpiLink;
  TOPOLOGY_TREE_NODE  GrandParent, Parent, Child;
  QPI_LINK_DATA       ParentLep;
  BOOLEAN       AddNode;

  MemSetLocal ((VOID *) OemRoute, 0xFF, sizeof (OemRoute));
  Map6SNodes (host, DiscEngData);
  Map6SRoutes (host, DiscEngData, OemRoute);

  //
  // Construct the topology tree that uses the template routing
  //
  Child.RawNode    = 0;
  Child.Node.Valid = TRUE;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0;  DiscEngData->CpuTree[Idx1][Idx2].Node.Valid == TRUE && DiscEngData->CpuTree[Idx1][Idx2].Node.Hop < 3; ++Idx2) {

        Parent = DiscEngData->CpuTree[Idx1][Idx2];
        Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
        Child.Node.ParentIndex = Idx2; // Remember who is the immediate parent of this child

        for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) { // Explore all the links of this parent

          ParentLep = DiscEngData->Cpu[Parent.Node.SocId].LinkData[QpiLink];
          if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE) {
            continue;
          }

          //
          // Get the Child Socket Id, Type and port that connects it to the parent form LEP
          //
          Child.Node.SocId      = ParentLep.PeerSocId;
          Child.Node.SocType    = ParentLep.PeerSocType;
          Child.Node.ParentPort = ParentLep.PeerPort;

          //
          // Make sure that this socket is not already found in the tree between hop level 0 and parent's hop level.
          // The rational is that, if we find child socket already at that level, then we have a shorter path to this child.
          //
          if (ShorterPathExists (host, DiscEngData->CpuTree[Idx1], &Child) == TRUE) {
            continue;
          }

          //
          // If it is 1 Hop away node (i.e immediate neighbor), just add the node
          //
          AddNode = FALSE;
          if (Child.Node.Hop == 1) {
            AddNode = TRUE;
          } else if (Child.Node.Hop == 2) {
            //
            // If it is 2 Hop away node, make sure the template routing indicates this route to be used
            //
            for (Idx3 = 0; Idx3 < 3; ++Idx3) {
              if (OemRoute[Idx1][Idx3][0] == Parent.Node.SocId &&
                  OemRoute[Idx1][Idx3][1] == Child.Node.SocId &&
                  OemRoute[Idx1][Idx3][2] == 0xFF) {
                AddNode = TRUE;
                break;
              }
            }
          } else {
            //
            // If it is 3 Hop away node, make sure the template routing indicates this route to be used
            //
            GrandParent = DiscEngData->CpuTree[Idx1][Parent.Node.ParentIndex];
            for (Idx3 = 0; Idx3 < 3; ++Idx3) {
              if (OemRoute[Idx1][Idx3][0] == GrandParent.Node.SocId &&
                  OemRoute[Idx1][Idx3][1] == Parent.Node.SocId &&
                  OemRoute[Idx1][Idx3][2] == Child.Node.SocId) {
                AddNode = TRUE;
                break;
              }
            }
          }

          if (AddNode == FALSE) {
            continue;
          }

          if (AddNodeToTopologyTree (host, DiscEngData->CpuTree[Idx1], &Child) != QPI_SUCCESS) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Adding CPU%u to Topology Discovery Tree of CPU%u failed.",
                              Child.Node.SocId, Idx1));
            QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          }
        } // for(QpiLink...)
      }
    }
  }

  return QPI_SUCCESS;

}


STATIC
QPI_STATUS
Map6SNodes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData
  )
/*++

Routine Description:

  This routine establishes a mapping between intel's ref platform and this platform. Port number is not relevant.

  Ref Platform:

    S0 ----- S1 ----- S4
     |       |        |
     |       |        |
     |       |        |
     |       |        |
     |       |        |
    S2 ----- S3 ----- S5


  OEM Platform:

    S5 ----- S1 ----- S3
     |       |        |
     |       |        |
     |       |        |
     |       |        |
     |       |        |
    S4 ----- S2 ----- S0



  The socket mapping would be:

    Intel   OEM
      S0     S5
      S1     S1
      S2     S4
      S3     S2
      S4     S3
      S5     S0

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
      
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8               Idx1, QpiLink;
  UINT8               RingNum1, RingNum2;
  UINT8               RefSoc = 0;
  UINT8               TempSoc1, TempSoc2;
  QPI_LINK_DATA       ParentLep;

  //
  // Identify the two nodes that are part of both the rings and map them to S1 & S3 of the ref topology
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->RingNodePairs[Idx1] != 0xFF) {
      RefSoc = Idx1;
      DiscEngData->SocMap[1] = RefSoc;
      DiscEngData->SocMap[3] = DiscEngData->RingNodePairs[RefSoc];
      break;
    }
  }
  QPI_ASSERT ((Idx1 < MAX_CPU_SOCKETS), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // Identify RefSoc's two rings present in this platform 
  RingNum1 = DiscEngData->RingInfo[RefSoc][1];
  RingNum2 = DiscEngData->RingInfo[RefSoc][2];

  // Identify the other 2 nodes of RefSoc's first ring
  TempSoc1 = 0xFF;
  TempSoc2 = 0xFF;
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (DiscEngData->Rings[RingNum1][Idx1] == RefSoc || DiscEngData->Rings[RingNum1][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    if (TempSoc1 == 0xFF) {
      TempSoc1 = DiscEngData->Rings[RingNum1][Idx1];
    } else {
      TempSoc2 = DiscEngData->Rings[RingNum1][Idx1];
    }
  }
  QPI_ASSERT ((TempSoc1 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((TempSoc2 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  //
  // Make sure TempSoc2 is the diagonally opposite in RefSoc's first ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[QpiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S0 and S2 of intel ref platform
  DiscEngData->SocMap[0] = TempSoc1;
  DiscEngData->SocMap[2] = TempSoc2;


  // Identify the other 2 nodes of RefSoc's second ring
  TempSoc1 = 0xFF;
  TempSoc2 = 0xFF;
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (DiscEngData->Rings[RingNum2][Idx1] == RefSoc || DiscEngData->Rings[RingNum2][Idx1] == DiscEngData->RingNodePairs[RefSoc]) {
      continue;
    }
    if (TempSoc1 == 0xFF) {
      TempSoc1 = DiscEngData->Rings[RingNum2][Idx1];
    } else {
      TempSoc2 = DiscEngData->Rings[RingNum2][Idx1];
    }
  }
  QPI_ASSERT ((TempSoc1 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((TempSoc2 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  //
  // Make sure TempSoc2 is the diagonally opposite in S0's second ring; otherwise swap out TempSoc1 & TempSoc2
  //
  for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
    ParentLep = DiscEngData->Cpu[RefSoc].LinkData[QpiLink];
    if (ParentLep.Valid == TRUE) {
      if (ParentLep.PeerSocId == TempSoc2) {
        TempSoc2 = TempSoc1;
        TempSoc1 = (UINT8)ParentLep.PeerSocId;
        break;
      }
    }
  }

  // TempSoc1 and TempSoc2 can be now mapped to S4 and S5 of intel ref platform
  DiscEngData->SocMap[4] = TempSoc1;
  DiscEngData->SocMap[5] = TempSoc2;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    Socket Mapping"));
  for (Idx1 = 0; Idx1 < 6; ++Idx1) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;      %u  -  %u ", Idx1, DiscEngData->SocMap[Idx1]));
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
Map6SRoutes (
  struct sysHost             *host,
  QPI_DISC_DATA            *DiscEngData,
  UINT8         (*OemRoute)[3][3]
  )
/*++

Routine Description:

  Uses the following static routing as tamplate and produces the routing for the given platform
  using the socket mapping.

  S0->S2->S3;  S0->S1->S4;  S0->S2->S3->S5
  S1->S3->S2;  S1->S3->S5
  S2->S0->S1;  S2->S3->S5;  S2->S0->S1->S4
  S3->S1->S0;  S3->S1->S4
  S4->S5->S3;  S4->S1->S0;  S4->S5->S3->S2
  S5->S4->S1;  S5->S3->S2;  S5->S4->S1->S0   

Arguments:
   host  - Pointer to the system host (root) structure
   DiscEngData - Pointer to socket specific data 
   OemRoute -
   
Returns:

   0 - Success
   Other - Failure

--*/
{
  UINT8         RefRoute[6][3][3] = {
                {{2, 3, 0xFF}, {1, 4, 0xFF}, {2, 3, 5}}, // S0
                {{3, 2, 0xFF}, {3, 5, 0xFF}, {0xFF, 0xFF, 0xFF}}, // S1
                {{0, 1, 0xFF}, {3, 5, 0xFF}, {0, 1, 4}}, // S2
                {{1, 0, 0xFF}, {1, 4, 0xFF}, {0xFF, 0xFF, 0xFF}}, // S3
                {{5, 3, 0xFF}, {1, 0, 0xFF}, {5, 3, 2}}, // S4
                {{4, 1, 0xFF}, {3, 2, 0xFF}, {4, 1, 0}}  // S5
             };
  UINT8         Idx1, Idx2, Idx3, OemSoc;

  for (Idx1 = 0; Idx1 < 6; ++Idx1) {
    OemSoc = DiscEngData->SocMap[Idx1];
    for (Idx2 = 0; Idx2 < 3; ++Idx2) {
      for (Idx3 = 0; Idx3 < 3; ++Idx3) {
        if (RefRoute[Idx1][Idx2][Idx3] != 0xFF) {
          OemRoute[OemSoc][Idx2][Idx3] = DiscEngData->SocMap[RefRoute[Idx1][Idx2][Idx3]];
        }
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;"));
      for (Idx2 = 0; Idx2 < 3; ++Idx2) {
        if (OemRoute[Idx1][Idx2][0] != 0xFF) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "  S%u -> S%u -> S%u", Idx1, OemRoute[Idx1][Idx2][0], OemRoute[Idx1][Idx2][1]));
          if (OemRoute[Idx1][Idx2][2] != 0xFF) {
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, " -> S%u", OemRoute[Idx1][Idx2][2]));
          }
        }
      }
    }
  }

  return QPI_SUCCESS;
}

#endif

//
// All the following routines are Si specific and needs to be ported based on Si capability
//

STATIC
QPI_STATUS
CalculateRT (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Calculate routing for socket

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC internal variables.

Returns:

   0 - Success
   Other - Failure

--*/
{
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  Child, Parent;
  UINT8               Idx1, Idx2;
  UINT8               QpiLink;
  QPI_LINK_DATA       ParentLep;
  UINT32              TempRt;

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {

      TopologyTree = SocketData->CpuTree[Idx1];
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; Calculating Route for CPU%u ", TopologyTree[0].Node.SocId));
      QpiCheckPoint ((UINT8) (1 << TopologyTree[0].Node.SocId), (UINT8) TopologyTree[0].Node.SocType, 0, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CALCULATE_ROUTE, host);

      for (Idx2 = 0; TopologyTree[Idx2].Node.Valid != FALSE; ++Idx2) {
        Parent            = TopologyTree[Idx2];
        Child.Node.Valid  = TRUE;
        Child.Node.Hop    = Parent.Node.Hop + 1; // One level down from parent
        if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
          for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {

            ParentLep = SocketData->Cpu[Parent.Node.SocId].LinkData[QpiLink];

            //
            // Skip the Invalid/Duplicate Links
            //
            if (ParentLep.Valid != TRUE || ParentLep.DualLink == TRUE) {
              continue;
            }

            //
            // Get the Child's socket Id, Type and parent port form LEP
            //
            Child.Node.SocId      = ParentLep.PeerSocId;
            Child.Node.SocType    = ParentLep.PeerSocType;
            Child.Node.ParentPort = ParentLep.PeerPort;

            //
            // Check that this Child is found in the topology tree
            //
            if (NodeFoundInTopologyTree (host, TopologyTree, &Child) == TRUE) {
              //
              // Setup the route including snoop fanout route.
              //   
              SetupNonBCRoute (host, SocketData, QpiInternalGlobal, TopologyTree, &TopologyTree[0], &Parent, &Child);
              if (QpiInternalGlobal->SnoopFanoutEn == TRUE) {
                SetupBCRoute(host, SocketData, QpiInternalGlobal, &TopologyTree[0], &Parent, &Child, FALSE);
              }
            //
            // Check if 4S ring and the socket is not Root socket and hop is 1 only (i.e interm socket)
            //    
            } else if ((QpiInternalGlobal->Is4SRing == TRUE) && (TopologyTree[0].Node.SocId != Child.Node.SocId && Parent.Node.Hop == 1)) {
              if (QpiInternalGlobal->SnoopFanoutEn == TRUE) {
                SetupBCRoute(host, SocketData, QpiInternalGlobal, &TopologyTree[0], &Parent, &Child, TRUE);
              }
            }
          } // for(QpiLink...)
        } else {
          //
          // This is a fatal error
          //
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Unknown Socket Type found in CPU%u TopologyTree. Topology not supported.\n ", TopologyTree[0].Node.SocId));
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
        }
      }
    }
  }

  //
  // If it is a 2S 2QPI "twisted" link config, then swap the Odd and Even Cbo route tables in socket 1.
  //
  if (QpiInternalGlobal->TwistedQpiLinks == TRUE) { 
    TempRt = SocketData->Route[1].CboEvenQpiRt;
    SocketData->Route[1].CboEvenQpiRt = SocketData->Route[1].CboOddQpiRt;
    SocketData->Route[1].CboOddQpiRt = TempRt;
  }

  // Disable RBT update for the routes that are different in forward & return directiron (e.g ring)
  CheckAndDisableRbtUpdate (host, SocketData->Cpu, SocketData->CpuTree, SocketData->Route, TRUE);

  // Optmize the HA from sending snoop to the requester in snoop fanout mode.
  if (QpiInternalGlobal->SnoopFanoutEn == TRUE) {
    OptimizeHaFanoutSnoop (host, SocketData);
  }

  RouteTableDump (host, SocketData, QpiInternalGlobal);

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
SetupNonBCRoute (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,  
  TOPOLOGY_TREE_NODE         *TopologyTree,
  TOPOLOGY_TREE_NODE         *Root,
  TOPOLOGY_TREE_NODE         *Interm,
  TOPOLOGY_TREE_NODE         *Dest
  )
/*++

Routine Description:

  Setup route for node

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC internal variables.
  TopologyTree - Pointer to full system QPI fabric tree
  Root  - Pointer to root QPI node
  Interm - Pointer to intermediate QPI node
  Dest - Pointer to destination QPI node

Returns:

  0 - Successful
  1 - Failure

--*/
{
  TOPOLOGY_TREE_NODE  Parent;
  TOPOLOGY_TREE_NODE  Child;
  TOPOLOGY_TREE_NODE  GrandParent;
  UINT8               cLocalPort1;
  UINT8               cLocalPort2;
  UINT8               pLocalPort1;
  UINT8               RootVn;
  QPI_CPU_SOCKET_DATA     *Cpu;

  Cpu = SocketData->Cpu;
  RootVn = Cpu[Root->Node.SocId].Vn;
  //
  // Set the Forward Path from "Root" to "Dest". Backward path from "Dest" to "Root" will be
  // set when this routineis called with "Dest" as "Root" and "Root" as "Dest".
  //
  //
  // 1. Recursively set the Forward Path in:
  //     - Socket "Interm"
  //     - Sockets that exist between "Interm" & "Root"
  //     - Socket "Root"
  // (No need to set the Forward path in Socket "Dest" itself since the Local Routing rules in "Dest" will take care of it)
  //
  Parent  = *Interm;
  Child   = *Dest;
  while (TRUE) {
    if (Parent.Node.SocId == Root->Node.SocId && Parent.Node.SocType == Root->Node.SocType) {
      if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
        //
        // In Root CPU socket, setup the route in Cbo, HA & R2PCIe.
        //
        if (DuplicateLinkExists (host, SocketData->Cpu, &Parent, &Child)) {
          GetDualLinkFromLEP (host, SocketData->Cpu, &Parent, &Child, &cLocalPort1, &cLocalPort2);
          if (cLocalPort1 == 0xFF || cLocalPort2 == 0xFF) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Invalid Dual-Link between CPU%u and CPU%u. ", Parent.Node.SocId, Child.Node.SocId));
            QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          }
          SetRootNodeRoute (host, SocketData, QpiInternalGlobal, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort1, cLocalPort2, TRUE, RootVn);
        } else {
          cLocalPort1 = GetChildLocalPortFromLEP (host, SocketData->Cpu, &Parent, &Child);
          if (cLocalPort1 == 0xFF) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Invalid Link between CPU%u and CPU%u. ", Parent.Node.SocId, Child.Node.SocId));
            QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          }
          SetRootNodeRoute (host, SocketData, QpiInternalGlobal, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort1, cLocalPort1, FALSE, RootVn);
        }
      } else {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Unknown Socket Type found in CPU%u TopologyTree. \n ", Root->Node.SocId));
        QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
      }
      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the path in intermediate CPU socket found in between root & dest
      //
      GetParentSocket (host, TopologyTree, &Parent, &GrandParent);
      pLocalPort1 = (UINT8) Parent.Node.ParentPort;
      cLocalPort1 = GetChildLocalPortFromLEP (host, SocketData->Cpu, &Parent, &Child);
      if (cLocalPort1 == 0xFF) {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u. ", Parent.Node.SocId, Child.Node.SocId));
        QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
      SetPassThruRoute (host, SocketData, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, pLocalPort1, cLocalPort1, RootVn);
    } else {
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found in CPU%u Topology Tree. ", Root->Node.SocId));
      QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }

    //
    // Go up one step towards the root & repeat the loop
    //
    Child = Parent;
    GetParentSocket (host, TopologyTree, &Child, &Parent);
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
SetupBCRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Root,
  TOPOLOGY_TREE_NODE       *Interm,
  TOPOLOGY_TREE_NODE       *Dest,
  BOOLEAN                  PassThruFanout
  )
{
  UINT8                      cLocalPort1, cLocalPort2, pLocalPort1, pLocalPort2;

  cLocalPort1 = cLocalPort2 = pLocalPort1 = pLocalPort2 = 0xFF;

  //
  //  Set the broadcast path to "Dest" in "Interm" socket
  //
  if (Interm->Node.SocId == Root->Node.SocId && Interm->Node.SocType == Root->Node.SocType) {
    if (DuplicateLinkExists(host, SocketData->Cpu, Interm, Dest)) {
      //
      // For dual-link configs, we select the first link.
      // 
      GetDualLinkFromLEP(host, SocketData->Cpu, Interm, Dest, &cLocalPort1, &cLocalPort2);
    } else {
      cLocalPort1 = GetChildLocalPortFromLEP(host, SocketData->Cpu, Interm, Dest);
      pLocalPort1 = (UINT8)Dest->Node.ParentPort;
      if (cLocalPort1 == 0xFF || pLocalPort1 == 0xFF) {
        QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u. ", Interm->Node.SocId, Dest->Node.SocId));
        QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
    }
    SetRootNodeFanoutRoute (SocketData, QpiInternalGlobal, Interm, (UINT8)Root->Node.SocId, cLocalPort1);
  } else if (Interm->Node.SocType == SOCKET_TYPE_CPU) {
    if((QpiInternalGlobal->Is4SRing != TRUE) || ((QpiInternalGlobal->Is4SRing == TRUE) && (PassThruFanout == TRUE))) {  // check if (not 4S ring) or (4S ring with passthru)
    
    //
    // Setup the path in intermediate CPU socket found in between root & dest
    //
    cLocalPort1 = GetChildLocalPortFromLEP(host, SocketData->Cpu, Interm, Dest);
    pLocalPort1 = (UINT8)Interm->Node.ParentPort;
    if (cLocalPort1 == 0xFF || pLocalPort1 == 0xFF) {
      QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Invalid Port connection between CPU %u and CPU %u.", Interm->Node.SocId, Dest->Node.SocId));
      QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
    SetPassThruFanoutRoute (host, SocketData, Interm, (UINT8)Root->Node.SocId, pLocalPort1, cLocalPort1);
    }
  } else {
    QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found in CPU %u Topology Tree.", Root->Node.SocId));
    QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Topology not supported\n"));
    QPI_ASSERT(TRUE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
SetRootNodeRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,  
  UINT8                   SocId,
  UINT8                   DestSocId,
  UINT8                   OutPort1,
  UINT8                   OutPort2,
  BOOLEAN                 DualLink,
  UINT8                   RootVn
  )
/*++

Routine Description:

  Set Root Node Route and its VN

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC internal variables.
  SocId       - Root socket ID
  DestSocId   - endpoint processor socket ID
  OutPort1    - First QPI port of node
  OutPort2    - Second QPI port of node; set to same port as OutPort1 for non dual-link topologies
  DualLink    - TRUE - Dual link exists
                FALSE - No Dual link
  RootVn      - Root socket's VN

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8    PortFieldWidth;
  UINT8    CboPortFieldWidth;
  UINT32   Nid0, Nid1;
  R3QPI_VN_SEL *R3QpiVnSel;

  if ((OutPort1 >= MAX_QPI_PORTS) | (OutPort2 >= MAX_QPI_PORTS)) {
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "Invalid value of QPI port of node"));
    return QPI_FAILURE;
  }

  //
  //  Set the RT in Cbo, HA & R2PCIe. R3QPI & QPIAgent RT needs to set for pass-thru route only.
  //  For all non-dual link topologies, the route setting is straight forward. Cbo, HA and R2PCIe
  //  route tables are configured to use the only link that was calculated in discovery process.
  //  
  //  If dual link exists between root node and immediate neighbor, need to balance load on them.
  //  Dual link can exist in 2S config only. Also the load balancing needs to be done for Cbo/HA only.
  //  It is not needed for R2PCIe since it is set to operate in a mode where it sends
  //  all the responses back on the same port on which the request was received which will balance
  //  the load on the links (R2PGNCTRL.qpi_rt_sel_ndr_drs=0). However R2PCIe requests always use
  //  route table and they can select only one link based on route table so the R2PCIe requests are not
  //  balanced out. (Only Ubox initiated requests; IIO initiated requests such as DMA, P2P are serviced
  //  by Cbo based on address hashing. Cbo will use balanced routing).
  //  
  //  HA is configured to send the responses back on the same link on which the request was received. 
  //  (QPILUT.QpiSrcRoute = 1. QPILUT.QpiRoute field is ignored when QPILUT.QpiSrcRoute is set).
  //  So the HA route table is not enabled/used for 2S topology.
  //  Since the requester (i.e Cbo) is configured to balance load on the links, HA responses also balanced out.
  //  For HA initiated messages such as snoops in Home Snoop mode, HA has to select one of the links.
  //  (If processor doesn't support Home Snoop for 2S config). HA is configured (HaSysDefeature2.CboIdSnoopIntlv = 1)
  //  to use the following interleaving scheme to select the link for snoop messages:
  //  
  //                    Link = ((HAID ^ CboID[0]) ^ HaSysDefeature2.SwitchQPI0vs1)
  //  
  //  HaSysDefeature2.SwitchQPI0vs1 has default 0 which will work for port0<->port0, port1<->port1 config.
  //  For port0<->port1, port1<->port0 config , HaSysDefeature2.SwitchQPI0vs1 needs to be set for one of the
  //  sockets to make sure the responses come back on the same port.
  //  
  //  Cbo is configured to balance the load on the links using the route table. For 2S 2QPI 1HA with
  //  port0<->port0, port1<->port1 mapping case, even Cbos are configured to use port 0 and odd Cbos
  //  are configured to use port1 on both sockets. Note that a Cbo will use the same port to talk all peer agents.
  //  In other words, even though the Cbo route table allows for selecting port at DNID granulairty, they are
  //  configured to be socket granular my programming the same port number for all NIDs of the peer socket.
  //  This will ensure that the responses come back on the same port and take advantage of D2C.
  //  For 2S 2QPI 2HA with port0<->port0, port1<->port1 mapping case, Cbo route tables are set the same way
  //  as in 2S 2QPI 1HA. In addition interleave mode is enabled in the Cbos by programming
  //  CBO_COH_CONFIG.cbotraffictoqpilinksintlv. When this field is set, the Cbo selects the link as follows:
  //
  //       Link = (Link 0 or 1 from Routing Table ^ HAID)
  //
  //  For configs with port0<->port1, port1<->port0 mapping, socket 0 Cbos are configured as above, but socket 1
  //  Cbos are configured such that Even Cbo uses port 1 and odd Cbo uses port 0.
  //  
  //  This along with the HA interleaving config described above will ensure that the responses always come back on
  //  the same port for Cbo initiated requests and HA initiated snoops. This esnures all flavours of 2S 2QPI configs
  //  benifit from D2C.
  //  
  //  Note: We calculate R2PCIe & HA route as well for simplicity to match other configs.
  //
  //  Note: In ExtRTID mode, each socket is assigned additional NID. The Cbo route tables
  //  will be adjusted to comprehend the extra NID before programming it. Other agent's route table 
  //  doesn't require this since they are socket ID granular, not NID granular.
  //
  
  PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2
  CboPortFieldWidth = 4; // Four bit field to encode link 0, 1 & 2

  // Cbo Route
  Nid0 = SOCID_TO_NID(DestSocId, 0);
  Nid1 = SOCID_TO_NID(DestSocId, 1);

  SocketData->Route[SocId].CboEvenQpiRt |= (OutPort1 << (CboPortFieldWidth * DestSocId));
  SocketData->Route[SocId].CboOddQpiRt  |= (OutPort2 << (CboPortFieldWidth * DestSocId));  
 
  // HA Route
  SocketData->Route[SocId].HaQpiRt |= (OutPort1 << (PortFieldWidth * DestSocId));

  // R2PCIe Route
  SocketData->Route[SocId].R2PcieQpiRt |= (OutPort1 << (PortFieldWidth * DestSocId));

  //
  // Calculate the VN setting for the root node. We use a scheme where the VN selection is done only
  // at the root (i.e local) node. At intermediate nodes the VN is not switched; messages are passed on the same
  // VN on which they arrived. The VN selection for the local traffic is obtained thru VN Lookup table (LUT)
  // stored in R3QIGRVNSEL0_R3QPI_CFG_REG / R3QIGRVNSEL1_R3QPI_CFG_REG registers. 
  // Each entry in the table is 1 bit wide that selects VN0 (0) or VN1 (1). 
  // The table is 32-entry deep whose index is calculated as follows:  
  //
  //        {Local_Remote_Sel, VN, DNID[3], DNID[1:0]}
  // 
  //        Bit[15-0]    for Remote, i.e Local_Remote_Sel = 0
  //        Bit[31-16]   for Local,  i.e Local_Remote_Sel = 1
  // 
  // Remote entries are always programmed to map VN0->VN0, VN1->VN1.
  // Local entries are used to select the VN for the traffic that originates in this (root) socket. Even
  // though the local traffic always arrives on VN0, we program the entry corresponding to VN1 as well.
  // Since a root socket uses same VN for all DNIDs that it talks to, all the 16 entries corresponding to
  // "Local" will have same setting. For the same reason, all the ports will have same setting as well.
  // Note that even though a port may not be used to talk to all other sockets or not all the sockets are
  // populated, we calculate the VN setting assumign a port is used to talk to all possible sockets to
  // avoid variations in setting. Also since the h/w does the route lookup separate from VN lookup
  // and route table is set only for the valid sockets, this is ok. 
  // 
  // Note: EX Platform BIOS sighting 4032686 has found an error in the prvious version of this code that had Bit[15-0] and Bit[31-16] accidently swapped.
  //
  R3QpiVnSel =  &(SocketData->Route[SocId].R3QpiVnSel[PORT_TO_R3QPI(OutPort1)]);
  (RootVn == VN0) ? (R3QpiVnSel->IgrVnSel = 0x0000FF00) : (R3QpiVnSel->IgrVnSel = 0xFFFFFF00);

  return QPI_SUCCESS;
}


STATIC
QPI_STATUS
SetPassThruRoute (
  struct sysHost           *host,
  QPI_SOCKET_DATA         *SocketData,
  UINT8                   SocId,
  UINT8                   DestSocId,
  UINT8                   InPort,
  UINT8                   OutPort,
  UINT8                   RootVn
  )
/*++

Routine Description:

  Sets up route-through route on a socket 

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  SocId       - Pass-thru socket ID
  DestSocId   - endpoint processor socket ID
  InPort      - pass-thru port towards the root
  OutPort      - pass-thru port towards the destination
  RootVn      - Root socket's VN

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8  R3QpiNum;
  UINT32 EncodedRoute = 0;

  // set route table in R3QPI  
  R3QpiNum = PORT_TO_R3QPI(InPort);
  ConvertR3QPIRoutingTable(InPort, OutPort, DestSocId, &EncodedRoute); 
  SocketData->Route[SocId].R3QpiRt[R3QpiNum] |= EncodedRoute;

  // 
  // Calculate the VN setting for the pass-thru node. Note that the VN selection for the pass-thru traffic
  // happens at the incoming port, not at the outgoing port. So we need to programming the VN setting 
  // at the R3QPI to which the incoming port belongs to. The pass-thru VN selection table is
  // stored in R3QEGR1VNSEL register. Each entry in the table is 1 bit wide that selects VN0 (0) or VN1 (1).
  // Lower 16 bits of the register are used by port0 and upper 16 by port1. Index is calculated as follows:
  //
  //        {VN, DNID[3], DNID[1:0]}
  // 
  // Since there is no VN switching at pass-thru sockets, all the ports will have same setting.
  // Also, we set the VN selection for all possible DNIDs even though the link might never traffic
  // destined for some DNIDs. Since the pass-thru routes are set only for valid DNID and the route
  // lookup is done is parrall, we can have this simplicity.
  //
  SocketData->Route[SocId].R3QpiVnSel[R3QpiNum].EgrVnSel = 0xFF00FF00;

  // set route table in QPI Agent
  SocketData->Route[SocId].QpiAgentRt[InPort] |= (1 << DestSocId);

  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
SetRootNodeFanoutRoute (
  QPI_SOCKET_DATA          *SocketData,
  QPI_HOST_INTERNAL_GLOBAL *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    OutPort
  )
{
  UINT8   Idx1;
  UINT32  EncodingBit = 0;

  //
  // Request coming from all populated sockets including the local socket should trigger a snoop to OutPort
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ((QpiInternalGlobal->CpuList & (1 << Idx1))) {
      EncodingBit |= ((1 << OutPort) << ((3 * Idx1)+ 5)) ; // program Send2QPIPorts for this RHNID in HA
    }
  }
  SocketData->Route[Parent->Node.SocId].HaQpiFanOut |= EncodingBit;

  return QPI_SUCCESS;
}


STATIC
QPI_STATUS
SetPassThruFanoutRoute (
  struct sysHost          *host,
  QPI_SOCKET_DATA          *SocketData,
  TOPOLOGY_TREE_NODE       *Parent,
  UINT8                    RootBcRti,
  UINT8                    InPort,
  UINT8                    OutPort
  )
{
  UINT8   R3QpiNum;
  UINT32  Data32;
  UINT8  FanoutLookUpTable[SI_MAX_QPI_PORTS][SI_MAX_QPI_PORTS]= { 
        {0xFF, 0x01, 0x02},
        {0x01, 0xFF, 0x02},
        {0x02, 0x04, 0xFF}
    }; 
    
  R3QpiNum = PORT_TO_R3QPI(InPort);            // determine R3QPI agent based on InPort
  Data32 = FanoutLookUpTable[InPort][OutPort];  // Encoded fanout route
  SocketData->Route[Parent->Node.SocId].R3QpiSnpFanOut[R3QpiNum][InPort & 0x1] |= (Data32 <<  (3 * RootBcRti));
 
  return QPI_SUCCESS;
}

STATIC
VOID
CheckAndDisableRbtUpdate (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *Cpu,
  TOPOLOGY_TREE_NODE      (*CpuTree)[MAX_TREE_NODES],
  QPI_CPU_RT              *Route, 
  BOOLEAN                 SiPort
  )
/*++

Routine Description:
   When forward and return routes are different between any 2 sockets, then the RBT update needs to be disabled
   in those two sockets for the transactions sent/received between them. Such routes can not benifit from D2C.
   Allowing such transactions to overwrite the RBT will prevent the transactions between sockets that use same forward
   and return routes benifitting from D2C. Such routes will operate with the RBT entries preloaded by QPIRC (Static RBT).

   For example:
                S0 ---- S1
                |        |
                S3 ---- S2

   If forward route is  0->1->2, return route is 2->3->0, disable RBT update in S0 for transactions targetting S2 and
   in S2 for transactions targetting S0.

Arguments:

  host  - Pointer to the system host (root) structure
  Cpu   - Socket data
  CpuTree      - Topology tree of all the sockets  
  Route     - Route setting
  SiPort      - TRUE if the port number in "Cpu" data structure is Si assigned port number

Returns:
  None

--*/

{
  UINT8               Index, Index1, Index2, Index3, SourceSocId, DestSocId, NumOfStopsOnSource, PortFromSource;
  UINT8               Port;
  UINT8               TempStr[MAX_CPU_SOCKETS * 2];
  UINT8               Path[MAX_CPU_SOCKETS][MAX_CPU_SOCKETS][MAX_CPU_SOCKETS * 2];
  UINT8               NumStops[MAX_CPU_SOCKETS][MAX_CPU_SOCKETS];
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  Parent, Child;

  MemSetLocal ((VOID *) NumStops, 0, sizeof (NumStops));      // initialize the NumStops to 0

  //Update all route paths for all source sockets with all dest sockets
 
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (Cpu[Index].Valid == TRUE) {
      TopologyTree = CpuTree[Index];
      for (Index1 = 1; TopologyTree[Index1].Node.Valid != FALSE; ++Index1) {
        MemSetLocal ((VOID *) TempStr, '$', sizeof (TempStr));
        Index2 = 0;
        Child = TopologyTree[Index1];
        while (Child.Node.Hop != 0) {
          Parent = TopologyTree[Child.Node.ParentIndex];
          Port = GetChildLocalPortFromLEP (host, Cpu, &Parent, &Child);
          TempStr[Index2++] = (UINT8)((Child.Node.SocId  << 4) | Child.Node.ParentPort);
          if (SiPort == TRUE) {
            Port = TranslateToSiPort (host, Port);
            TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
          } else {
            TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
          }
          MemCheckIndex(host, Index2, MAX_CPU_SOCKETS * 2);// Klocwork 1863: Array 'TempStr' of size 8 may use index value(s) 9..255. 
          Child = Parent;
        }

        //update the route between SourceSocketId and DestSocketId
        SourceSocId = TempStr[Index2-1] >> 4;
        DestSocId = TempStr[0] >> 4;
        Index3 = 0;
        NumStops[SourceSocId][DestSocId] = Index2;
        
        if (Index2 > (MAX_CPU_SOCKETS * 2)) {
          QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        } else {
        for (; Index2 != 0; Index2--) {
            Path[SourceSocId][DestSocId][Index3++]= TempStr[Index2-1];
        }             
        }
       
      }
    }
  }

  // compare the sending route and return route for all sockets
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {                   //scan all valid sockets, Index for source
    if (Cpu[Index].Valid == TRUE) { 
       SourceSocId = Cpu[Index].SocId;
       for (Index1 = 0; Index1 < MAX_CPU_SOCKETS; ++Index1) {           //scan all valid sockets, Index1 for DestSocketId
         if( Cpu[Index1].SocId != SourceSocId && Cpu[Index1].Valid == TRUE ){
              DestSocId = Cpu[Index1].SocId;                         // compare the sending route and return route between SourceSocketId and DestSocketId
              NumOfStopsOnSource = NumStops[SourceSocId][DestSocId];
              PortFromSource = Path[SourceSocId][DestSocId][0] & 0x0F;
              if( NumOfStopsOnSource == NumStops[DestSocId][SourceSocId]){              // check if source and dest have the same stops
                for (Index2 = 0; Index2 < NumOfStopsOnSource && Index2 < (2 * MAX_CPU_SOCKETS); ++Index2) {
                  if( Path[SourceSocId][DestSocId][Index2] != Path[DestSocId][SourceSocId][NumOfStopsOnSource - Index2 -1]){   // check if the same path
                      Route[SourceSocId].QpiAgentRt[PortFromSource] |= ((1 << DestSocId) << 8);         // disable RBT Update for DestSocId
                      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; Disable RBT update on SocId %u Port %u for DestSocId %u", SourceSocId, PortFromSource, DestSocId));
                      break;
                  }
                }
              } else {
                  Route[SourceSocId].QpiAgentRt[PortFromSource] |= ((1 << DestSocId) << 8);
              }
         }            
       }  
    }
  }

}

STATIC
VOID
OptimizeHaFanoutSnoop (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  )
/*++

Routine Description:

  When the system is in Snoop Fanout mode, the HA is required to send send one snoop per link. But in some topologies
  it is possible the original requester is the only socket present on any of its link. In such cases, the HA can be optimized
  to no send the snoop over the link to reduce the uncore ring traffic.

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure

Returns:

  VOID

--*/
{
  UINT8               Index1, Index2, PeerSocId, PeerPort, R3QpiNum;
  UINT32              Data32;

  //
  // For each valid sockets, find out if the pass-through traffic is needed on the peer port of peer socket, 
  // If there is no pass-through fanout traffic, then HA can be optimized to no send the snoop over the link to reduce the uncore ring traffic for RHNID = peer socket.
  //
  //
  for (Index1 = 0; Index1 < MAX_CPU_SOCKETS; ++Index1) {
    if (SocketData->Cpu[Index1].Valid == TRUE) {

      //
      // find out if the pass-through traffic is needed on the peer port of peer socket, 
      // 
      //
      for (Index2 = 0; Index2 < MAX_QPI_PORTS; ++Index2) {
        if (SocketData->Cpu[Index1].LinkData[Index2].Valid == TRUE ) {

          //
          //Read R3QPI Fanout value from Peer Port of Peer Socket 
          //If is 0, there is no pass-through fanout needed on Peer Port of Peer Socket
          //
          PeerSocId = (UINT8) SocketData->Cpu[Index1].LinkData[Index2].PeerSocId;
          PeerPort =  (UINT8) SocketData->Cpu[Index1].LinkData[Index2].PeerPort;
          R3QpiNum = PORT_TO_R3QPI(PeerPort);
          Data32 = SocketData->Route[PeerSocId].R3QpiSnpFanOut[R3QpiNum][PeerPort & 0x01];

          if (Data32 == 0){
          //
          // Optimize the HA to not send snoop to the link that peer socket id is equal to RHNID
          //
          Data32 = SocketData->Route[Index1].HaQpiFanOut;
          Data32 = Data32 & (~((1 << Index2) << ((3 * PeerSocId) + 5)));     
          SocketData->Route[Index1].HaQpiFanOut = Data32;
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; CPU%1u HA Snoop fanout optimized for Link%1u target CPU%1u",
                                 Index1, Index2, PeerSocId)); 
          }
        }
      }
    }
  }

  return;
}


