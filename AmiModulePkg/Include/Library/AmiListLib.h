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
  AmiList library class definition. Doubly Linked List Library.

  The library class defines doubly linked list API that can be used as an alternative
  to the EDKII doubly linked list API defined by BaseLib.h.
  If you unsure which one to use, use the BaseLib.
  AmiListLib is mainly intended to simplify porting of the legacy Aptio 4 code.
  That is is why the library in certain cases does not adhere
  to the Aptio V naming conventions.

  The key differences between BaseLib and AmiListLib lists are:
  - BaseLib defines a circular list; AmiListLib defines a linear list.
  - BaseLib uses the same LIST_ENTRY data structure to refer to the a list
    and to an element of the list; AmiListLib uses separate
    AMI_LIST and AMI_LINK structures.
  - AmiListLib tracks list length.
*/
#ifndef __AMI_LIST_LIB__H__
#define __AMI_LIST_LIB__H__

struct _AMI_LINK;
typedef struct _AMI_LINK AMI_LINK;
struct _AMI_LINK{
    AMI_LINK *pNext, *pPrev;
};

typedef struct _AMI_LIST{
    UINTN Size;
    AMI_LINK *pHead, *pTail;
} AMI_LIST;

/**
    Initializes linked list structure.
    A list must be initialized before using other list manipulating functions.

    @param List A pointer to the doubly linked list structure.
**/
VOID AmiListInit(IN AMI_LIST* List);

/**
    Checks if list is empty

    @param List A pointer to the doubly linked list structure.

    @retval TRUE    That list is empty
    @retval FALSE   That list is not empty
*/
BOOLEAN AmiListEmpty(IN AMI_LIST* List);

/**
    Adds new element to the list.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be added at the end of the doubly linked list.
**/
VOID AmiListAdd(IN AMI_LIST* List, IN AMI_LINK* Element);

/**
    Removes element from the list.

    Removes the node Element from a doubly linked list.  It is up to the caller
    to release the memory used by the Element if required.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be deleted from the list.

**/
VOID AmiListDelete(IN AMI_LIST* List, IN AMI_LINK* Element);

/**
    Inserts Element into the list at the defined position.

    The function inserts new element into the list immediately after
    an element specified by the third parameter After.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be added to the doubly linked list.
    @param After A pointer to a node after which Element is to be added.
           If After is NULL, the Element is added at the head of the list.
**/
VOID AmiListInsert(IN AMI_LIST* List, IN AMI_LINK* Element, IN AMI_LINK* After OPTIONAL);

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
