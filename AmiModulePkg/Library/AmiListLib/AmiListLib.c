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
    AmiLibstLib library instance. Linear doubly linked list implemenration.
**/
#include <Library/AmiListLib.h>

/**
    Initializes linked list structure.
    A list must be initialized before using other list manipulating functions.

    @param List A pointer to the doubly linked list structure.
**/
VOID AmiListInit(IN AMI_LIST* List){
    List->pHead = NULL;
    List->pTail = NULL;
    List->Size = 0;
}

/**
    Checks if list is empty

    @param List A pointer to the doubly linked list structure.

    @retval TRUE    That list is empty
    @retval FALSE   That list is not empty
*/
BOOLEAN AmiListEmpty(IN AMI_LIST* List){
    return !List->pHead;
}

/**
    Adds new element to the list.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be added at the end of the doubly linked list.
**/
VOID AmiListAdd(IN AMI_LIST* List, IN AMI_LINK* Element){
    Element->pNext = NULL;
    Element->pPrev = List->pTail;
    if (!List->pHead) List->pHead = Element;
    else List->pTail->pNext = Element;
    List->pTail = Element;
    List->Size++;
}

/**
    Removes element from the list.

    Removes the node Element from a doubly linked list.  It is up to the caller
    to release the memory used by the Element if required.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be deleted from the list.

**/
VOID AmiListDelete(IN AMI_LIST* List, IN AMI_LINK* Element){
    if (List->pTail == Element) List->pTail = Element->pPrev;
    else Element->pNext->pPrev = Element->pPrev;
    if (List->pHead == Element) List->pHead = Element->pNext;
    else Element->pPrev->pNext = Element->pNext;
    List->Size--;
}

/**
    Inserts Element into the list at the defined poisition.

    The function inserts new element into the list immediately after
    an element specified by the third parameter After.

    @param List A pointer to the doubly linked list structure.
    @param Element A pointer to a node that is to be added to the doubly linked list.
    @param After A pointer to a node after which Element is to be added.
           If After is NULL, the Element is added at the head of the list.
**/
VOID AmiListInsert(IN AMI_LIST* List, IN AMI_LINK* Element, IN AMI_LINK* After OPTIONAL){
    Element->pPrev = After;
    if (After)
    {
        Element->pNext = After->pNext;
        if (After==List->pTail) List->pTail=Element;
        else After->pNext->pPrev = Element;
        After->pNext = Element;
    }
    else
    {
        Element->pNext = List->pHead;
        if (List->pHead) List->pHead->pPrev = Element;
        else List->pTail = Element;
        List->pHead = Element;
    }
    List->Size++;
}

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
