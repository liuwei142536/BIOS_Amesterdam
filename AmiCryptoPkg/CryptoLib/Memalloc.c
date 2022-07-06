//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file 
    Memalloc.c 
    Light-weight memory manager for PEI and DXE crypto library
    Implements memory management routines malloc(), free() 
    and realloc() for PEI and DXE Crypto library
    Mmngr initialization routines:  RestCRmm(), InitCRmm()
**/

#include "includes.h"

// RestCRmm
// InitCRmm
// malloc
// realloc
// free

//#pragma pack(push, 4)
typedef struct {
    UINT8      *Addr;
    UINT8       Pages;      // Num of pages, Page = 256Byte
    UINT8       Attrib;     // 0-Free, 1-Used
    UINT16      Reserved;   // Align to 2 DWords
} CR_MEM_DESC;

typedef struct {
    UINT8    *gMemHeap;
    UINTN    gMaxHeapSize;
    CR_MEM_DESC  *gCurDesc;
    CR_MEM_DESC  *gFirstDesc;
    //dbg
    UINT8    *gMaxAddrReached;
//    UINT8   Reserved[4];   // Align to 8 bytes
} CR_MEM_COMM;
//#pragma pack(pop)

//#define CR_PAGE_SIZE    256
#define CR_PAGE_SIZE    128

//
// Attrib Flag Definitions
//
#define CR_BLOCK_FREE               0x00
#define CR_BLOCK_USED               0x01

void *GetCRmmPtr();
void ResetCRmm();

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
/*
Optimization-combining free blocks:
    If found free descriptor with smaller page size, check if adjacent block(s) are free too:
     - Last free descriptor: Clear Page value, leave address as it's no longer matter
     - Next free descriptor with available combined Page size(can be more then 1):
        Combine Page numbers with Prev one.
        Set Attrib = USED
        Use recursive invocation
*/
/*
void CR_squeze(int **i, CR_MEM_DESC* pDesc, int *Pages )
{
// already found at least one free block pDesc[i]
// Pages reflect remaining number
    if(i>=0 && pDesc[i].Attrib==FREE)
        if(pDesc[i].Pages <= Pages)
    {
// upd Pages in adjacent blocks
        pDesc[i].Pages = 0
        pDesc[i-1].Pages+=pDesc[i].Pages;
        Pages-=pDesc[i].Pages;
        pDesc[i-1].Attrib = USED;
//if Pages != 0 -> call recursively
        if(Pages)
            CE_squeze(--i, pDesc, Pages);
    }
}
*/

void *amicrypt_malloc(size_t Size);

/**
   Allocate Pool. Theory of operation:
    -Returns first available address on the Crypto Heap. 
    -Traverses MemDesc indexes from last used to 0 for ones with Free attribute 
      and enough Page space. If free block was not found - returns next available 
      address and updates next available MemDesc and MaxDescIndex
    -Updates the pointer within descriptors (index) for next mem chunk
    -Set mem descriptor with new allocated address within CrHeap, Page size aligned
   __________________________________________________
  |  |                                               |
  |  |<-gMemHeap                <-gCurDesc->     gFirstDesc
  |  |                              ^                ^
  |  |<--------- mem heap --------->|<-- mem_desc -->|
  |__|______________________________|________________|
   ^
 pCR_Comm
    alternative implementation for optimization-combining free blocks:
    If free descriptor with smaller page size is found, check if adjacent block(s) are free too:
     - Last free descriptor: update the Addr to next available Page
     - Next free descriptor with available combined Page size(can be more then 1:
        increment Page num to match requested size

  @param[in]   Size

  @retval      Ptr
**/
void *amicrypt_malloc(size_t Size)
{
    CR_MEM_DESC *pDesc;
    UINT8       *NewAddr, *LastHeapAddr;
    UINT32       Pages;
    int          i;
    CR_MEM_COMM *pCR_Comm = (CR_MEM_COMM*)GetCRmmPtr();

//    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Alloc  sz %X (Heap %X, CurrDesc %X)\n", Size,  pCR_Comm, (pCR_Comm==NULL)?0:pCR_Comm->gCurDesc));
    if(pCR_Comm == NULL)
        return NULL;

    if(pCR_Comm->gCurDesc == NULL) 
        ResetCRmm();

    pDesc = pCR_Comm->gCurDesc;

//  Potential heap overflow condition check - 
//  max allocation mem size must be less then the max number 
//  of Pages can be stored within UINT8 = 256*256=64kb
    Pages = (UINT32)(Size/CR_PAGE_SIZE) + (Size%CR_PAGE_SIZE == 0?0:1); // Align to the page size
    if(Pages > 0xff) {
//        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Pages  %X (pg size %X)\n", Pages, CR_PAGE_SIZE));
        return NULL;
    }

// locate available desc backwards
// Test cases
// 3. Old Pages>=Pages  -? Override Pages : Desc use=47; Heap Use: Max
// 2. Old Pages>=Pages  -? Leave Old Pages: Desc use=25; Heap use: Min
// 1. Old Pages==Pages                      Desc use=28; Heap Use: Mid
    i = 0;
    while(&pDesc[i] <= pCR_Comm->gFirstDesc)
    {
        if(pDesc[i].Attrib==CR_BLOCK_FREE && 
            (pDesc[i].Pages >= (UINT8)Pages))
//            (pDesc[i].Pages == Pages)) // Mode 1
        {
            pDesc[i].Attrib = CR_BLOCK_USED;
////            pDesc[i].Pages = Pages; //(Mode1, 3)
//            DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Alloc1 %08X (%4X->%4X)\n", pDesc[i].Addr, Size, pDesc[i].Pages*CR_PAGE_SIZE));
            return (void*)(pDesc[i].Addr);
        }
        i++;
    }
// allocate new one
    if(pCR_Comm->gCurDesc > pCR_Comm->gFirstDesc) {
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"====\nMMGR:Descriptor mem overflow %08X>%08X\n====", pCR_Comm->gCurDesc, pCR_Comm->gFirstDesc));
        return NULL;// out of Desc space
    }
    NewAddr = pDesc->Addr+(pDesc->Pages*CR_PAGE_SIZE);
//    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Alloc2 %08X (%4X->%4X)\n", NewAddr,Size,Pages*CR_PAGE_SIZE));
    LastHeapAddr = NewAddr+(Pages*CR_PAGE_SIZE);
// reuse the descriptor if it has matching address
    if(NewAddr != pDesc->Addr) pDesc--;
// dbg
    if(pCR_Comm->gMaxAddrReached < LastHeapAddr) {
        pCR_Comm->gMaxAddrReached = LastHeapAddr;
    }
// end dbg
    if(LastHeapAddr < (UINT8*)pDesc)
    {
        pCR_Comm->gCurDesc = pDesc;
        pDesc->Addr = NewAddr;
        pDesc->Pages = (UINT8)Pages;
        pDesc->Attrib = CR_BLOCK_USED;
        return (VOID*)(NewAddr);
    }
    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"====>\nMMGR:Heap and Descriptor mem overlap %08X>%08X\n<====", LastHeapAddr, (UINT8*)pDesc));
    return NULL; // out of Heap space
}

#ifndef os_zalloc
void *os_zalloc(size_t Size)
{
    return(amicrypt_malloc(Size));
}
#endif

/**
  Crypto Memory Manager->Free
  Traverses MemDescriptor indexes from last used to 0 to locate Address to be freed. 
  Set Attrib to Free. May also Clear the stale memory

  @param[in]   Ptr

  @retval      none
**/
void amicrypt_free(void *ptr)
{
    int    i = 0;
    CR_MEM_DESC *pDesc;
    CR_MEM_COMM *pCR_Comm = (CR_MEM_COMM*)GetCRmmPtr();

    if(pCR_Comm==NULL)
    {
        return;
    }

//    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Free   %08X", ptr));
    if(pCR_Comm->gCurDesc == NULL) 
        ResetCRmm();

    pDesc = pCR_Comm->gCurDesc;
    // locate any available backwards
    while(&pDesc[i] <= pCR_Comm->gFirstDesc)
    {
        if(pDesc[i].Attrib==CR_BLOCK_USED && pDesc[i].Addr == ptr)
        {
            pDesc[i].Attrib = CR_BLOCK_FREE;
            // clear unused memory
//            DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Free   %08X (%4X)\n", ptr, pDesc[i].Pages*CR_PAGE_SIZE));
            memset(pDesc[i].Addr, 0x0, pDesc[i].Pages*CR_PAGE_SIZE);
            ptr=NULL;
            return;
        }
        i++;
    }
//    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"(free) FAIL!!!\n"));
}

/**
  Crypto Memory Manager->realloc
  Traverses memory descriptors back from last to 0 for matching or smaller free contiguous space
  Alloc new desc and free the old one. Copy mem from old one to new one. Empty old memory

  @param[in]   OldPtr
  @param[in]   NewSize

  @retval      NewPtr
**/
void *amicrypt_realloc(void *OldPtr, size_t NewSize)
{
    void * NewPtr;

//    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"ReAlloc %08X (%4X)\n", (OldPtr), NewSize));
    if ( !OldPtr ) {
        /*
         *  NULL pointer means just do malloc
        */
        return amicrypt_malloc( NewSize );
    } else if ( NewSize == 0 ) {
        /*
         *  Non-NULL pointer and zero size means just do free
        */
        amicrypt_free( OldPtr );
        } else {
            NewPtr = amicrypt_malloc(NewSize);
            if(NewPtr != NULL){
                memcpy(NewPtr, OldPtr, NewSize);
                amicrypt_free( OldPtr );
                return NewPtr;
            }
        }
    return NULL;
}

/**
  Resets Crypto Memory Manager. Clears mem heap and descriptors

  @param[in]   none

  @retval     none
**/
void ResetCRmm()
{
    CR_MEM_COMM *pCR_Comm = (CR_MEM_COMM*)GetCRmmPtr();

    if(pCR_Comm==NULL)
    {
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"\n===>\n!!! Heap Addr %X !!!\n<===\n", (UINTN)pCR_Comm));
        return;
    }
    DEBUG_CODE (
    if(pCR_Comm->gMaxAddrReached > pCR_Comm->gMemHeap) {
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"\n===>\nMem Mgr Heap usage Statistics:\nHeap Size = %X\n",
                (UINTN)pCR_Comm->gFirstDesc-(UINTN)pCR_Comm->gMemHeap));
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Heap Start= %08X\nHeap Used = %08X (sz %X)\n", 
                pCR_Comm->gMemHeap, pCR_Comm->gMaxAddrReached, (pCR_Comm->gMaxAddrReached==NULL)?0:((UINTN)pCR_Comm->gMaxAddrReached-(UINTN)pCR_Comm->gMemHeap)));
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Heap Free = %X\n\n",
                ((pCR_Comm->gCurDesc==NULL)?pCR_Comm->gMaxHeapSize-sizeof(CR_MEM_DESC):(UINTN)pCR_Comm->gCurDesc-(UINTN)pCR_Comm->gMaxAddrReached) ));
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Desc Start= %08X\nDesc Used = %08X (sz %X)\n<===\n", 
                pCR_Comm->gFirstDesc, pCR_Comm->gCurDesc, (UINTN)pCR_Comm->gFirstDesc - (UINTN)pCR_Comm->gCurDesc));
    }
    );
    // 1. clear all heap but CR_MEM_COMM Descriptor
    memset( (void*)pCR_Comm->gMemHeap, 0, pCR_Comm->gMaxHeapSize);

    // 2. Init 1st CR_MEM_DESC descriptor
    pCR_Comm->gCurDesc = pCR_Comm->gFirstDesc;
    pCR_Comm->gCurDesc->Addr = pCR_Comm->gMemHeap;
    pCR_Comm->gCurDesc->Pages = 0;
    pCR_Comm->gCurDesc->Attrib = CR_BLOCK_FREE;
    
// dbg
    pCR_Comm->gMaxAddrReached = pCR_Comm->gMemHeap;
// dbg
}

//----------------------------------------------------------------------
//   __________________________________________________
//  |  |                              |                |
//  |  |<-gMemHeap                <-gCurDesc->     gFirstDesc
//  |  |                              ^                ^
//  |  |<--------- mem heap --------->|<-- mem_desc -->|
//  |__|______________________________|________________|
//   ^
// CR_Comm
//----------------------------------------------------------------------
/**
  Initializes Crypto Memory Manager.

  @param[in]   pHeap ptr to CRmem Descriptor table
  @param[in]   HeapSize Heap size to be reserved for crypto Mem manager

  @retval     none
**/
void InitCRmm(void* pHeap, UINTN HeapSize)
{
    CR_MEM_COMM *pCR_Comm = (CR_MEM_COMM*)(UINTN)pHeap; //(CR_MEM_COMM*)GetCRmmPtr();

    if(pCR_Comm==NULL)
    {
        DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"\n===>\n!!! Heap Addr %X !!!\n<===\n", (UINTN)pCR_Comm));
        return;
    }
    pCR_Comm->gMemHeap = (UINT8*)((UINTN)pCR_Comm + sizeof(CR_MEM_COMM));
    pCR_Comm->gMaxHeapSize = HeapSize - sizeof(CR_MEM_COMM);
    pCR_Comm->gMaxAddrReached = pCR_Comm->gMemHeap;
// Mem descriptors start from last addr of Mem Heap - CR_MEM_DESC
    pCR_Comm->gFirstDesc = (CR_MEM_DESC*)((UINTN)pCR_Comm->gMemHeap + pCR_Comm->gMaxHeapSize);
    pCR_Comm->gFirstDesc--;

    pCR_Comm->gCurDesc = NULL;
//    ResetCRmm();

    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"\n===>\nMem Mgr Init:\nHeap Start= %08X(sz %X)\n", (pCR_Comm->gMemHeap), pCR_Comm->gMaxHeapSize-sizeof(CR_MEM_DESC)));
    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Heap End  = %08X\n", (UINTN)(pCR_Comm->gFirstDesc) ));
    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL,"Desc Start= %08X(sz %X)\n<===\n", (pCR_Comm->gFirstDesc), sizeof(CR_MEM_DESC) ));
}
