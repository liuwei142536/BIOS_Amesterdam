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
    Implementation of AMI_ITEM_LIST manipulation functions
**/

#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiItemListLib.h>
#include <Library/BaseMemoryLib.h>

#define LST_INIT_COUNT  10 //initial value

//----------------------------------------------------------------------
// Methods Implementation for AMI_ITEM_LIST object
//----------------------------------------------------------------------


/** @internal
     Is an internal worker function which reallocates space for a AMI_ITEM_LIST.Items[] array.

    @param List List which needs its Items array reallocated.

    @retval EFI_OUT_OF_RESOURCES,  if allocation failed.
            Otherwise, EFI_SUCCESS.

**/
EFI_STATUS InternalReallocItemLst(AMI_ITEM_LIST *List)
{
    UINTN NewInitialCount;
    VOID *NewItems;

    if (List->ItemCount < List->InitialCount){
        if (List->Items == NULL){
            List->Items = AllocateZeroPool(List->InitialCount * sizeof(VOID*));
            if (List->Items == NULL) return EFI_OUT_OF_RESOURCES;
        }
        return EFI_SUCCESS;
    }
    NewInitialCount = List->InitialCount + LST_INIT_COUNT;
    NewItems = ReallocatePool(
        List->ItemCount * sizeof(VOID*),
        NewInitialCount * sizeof(VOID*),
        List->Items
    );
    if (NewItems == NULL) return EFI_OUT_OF_RESOURCES;
    List->Items = NewItems;
    List->InitialCount = NewInitialCount;

    return EFI_SUCCESS;
}

/**
  Initializes AMI_ITEM_LIST data structure declared as a global variable
  or as a field of the global variable.

  AMI_ITEM_LIST data structure must be initialized using
  AMI_ITEM_LIST_VARIABLE_INITIALIZE()/AMI_ITEM_LIST_VARIABLE_INITIALIZE_EX() macros
  or AmiItemListInitialize()/AmiItemListInitializeEx() functions before
  any other item list function can be used.

  @param List   Item list to initialize

  @return Status of the operation
**/
EFI_STATUS AmiItemListInitialize(AMI_ITEM_LIST *List){
    List->InitialCount = 0;
    List->ItemCount = 0;
    List->Items = NULL;

    return EFI_SUCCESS;
}

/**
  Initializes AMI_ITEM_LIST data structure declared as a global variable
  or as a field of the global variable.

  AMI_ITEM_LIST data structure must be initialized using
  AMI_ITEM_LIST_VARIABLE_INITIALIZE()/AMI_ITEM_LIST_VARIABLE_INITIALIZE_EX() macros
  or AmiItemListInitialize()/AmiItemListInitializeEx() functions before
  any other item list function can be used.

  @param List           Item list to initialize
  @param InitialCount   Initial number of elements in the item list array

  @return Status of the operation
**/
EFI_STATUS AmiItemListInitializeEx(AMI_ITEM_LIST *List, UINTN InitialCount){
    List->InitialCount = InitialCount;
    List->ItemCount = 0;
    List->Items = NULL;

    return EFI_SUCCESS;
}

/**
     Inserts the provided item pointed to by Item into the provided List at index ItemIndex.
    @param List List to be inserted into.
    @param Item Pointer to object to place in list.
    @param ItemIndex    Index in list to place the object pointed to by Item.

    @retval EFI_INVALID_PARAMETER,  if the ItemIndex value is invalid.
            EFI_OUT_OF_RESOURCES,   if there is not enough memory for the new object.
            Otherwise, EFI_SUCCESS.

**/

EFI_STATUS AmiItemListInsert(AMI_ITEM_LIST *List, VOID* Item, UINTN ItemIndex)
{
    INTN        i;

//----------------
    if (!List->ItemCount && List->ItemCount<ItemIndex) return EFI_INVALID_PARAMETER;

    //Check if Items[] array can accomodate a new child...
    if (EFI_ERROR(InternalReallocItemLst(List)))return EFI_OUT_OF_RESOURCES;

    //Shift items after Index forward
    for (i=(INTN)List->ItemCount-1; i>=(INTN)ItemIndex; i--)  List->Items[i+1]=List->Items[i];

    //fill Item address we are inserting
    List->Items[ItemIndex]=Item;
    //Adjust Item Count
    List->ItemCount++;
    return EFI_SUCCESS;
}


/**
    Deletes the object stored at the provided ItemIndex in List.
    Frees the memory associated with the object pointer if FreeData is TRUE.

    @param List List to be deleted from.
    @param ItemIndex    Index inside List->Items to be deleted.
    @param FreeData TRUE will free the memory associated with the deleted object.

    @retval EFI_INVALID_PARAMETER,  if the provided ItemIndex is invalid.
            Otherwise, EFI_SUCCESS.

**/

EFI_STATUS AmiItemListDelete(AMI_ITEM_LIST *List, UINTN ItemIndex, BOOLEAN FreeData)
{
    UINTN       i;

//----------------
    if (!List->ItemCount || List->ItemCount<=ItemIndex) return EFI_INVALID_PARAMETER;

    //Free Data associated with Iterm if requested;
    if (FreeData)FreePool(List->Items[ItemIndex]);

    //Shift items after ItemIndex backwards
    for (i=ItemIndex; i<List->ItemCount-1; i++) List->Items[i]=List->Items[i+1];

    //Adjust Item Count
    List->ItemCount--;
    return EFI_SUCCESS;
}

/**
    Appends the provided object to List.

    @param List List to be appended to.
    @param Item Object to be appended.

    @retval EFI_OUT_OF_RESOURCES,  if memory allocation fails.
             Otherwise, EFI_SUCCESS.

**/

EFI_STATUS AmiItemListAppend(AMI_ITEM_LIST *List, VOID* Item )
{
    //Check if Items[] array can accommodate a new child...
    if (EFI_ERROR(InternalReallocItemLst(List)))return EFI_OUT_OF_RESOURCES;

    List->Items[List->ItemCount]=Item;
    List->ItemCount++;

    return EFI_SUCCESS;
}

/**
    Clears all the items of List.  If FreeData is TRUE, frees all the memory associated
    with the stored objects.

    @param List         List to be cleared.
    @param FreeData     If TRUE, all objects are freed.

    @retval VOID
**/

VOID AmiItemListClear(AMI_ITEM_LIST *List, BOOLEAN FreeData)
{
    UINTN       i;

//-----------------------------------------
    if (FreeData)for (i=0; i<List->ItemCount; i++)
        {
            if (List->Items[i])FreePool(List->Items[i]);
        }

    if (List->Items) FreePool(List->Items);

    List->Items=NULL;
    List->ItemCount=0;
    List->InitialCount=0;
}

/**

    Creates a copy of each element in the List and stores them in the NewList.

    @param List     List to copy items from
    @param NewList  Address to return pointer to the newly allocated list with the copied items
    @param ItemSize Size of items referred by pointers in the item list

    @retval EFI_OUT_OF_RESOURCES,  if memory allocation fails.
            Otherwise, EFI_SUCCESS.
**/
EFI_STATUS AmiItemListCopy(
    IN AMI_ITEM_LIST *List, IN OUT AMI_ITEM_LIST **NewList, IN UINTN ItemSize
){
    UINTN i = 0;
    AMI_ITEM_LIST *TmpList;

    *NewList = AllocateZeroPool(sizeof(AMI_ITEM_LIST));

    if (!*NewList)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    TmpList = *NewList;
    TmpList->InitialCount = List->InitialCount;
    TmpList->ItemCount = List->ItemCount;

    if (List->ItemCount == 0)
    {
        return EFI_SUCCESS;
    }

    TmpList->Items = AllocateZeroPool( List->ItemCount * sizeof(VOID*) );

    if (!TmpList->Items)
    {
        return EFI_OUT_OF_RESOURCES;
    }
    if (ItemSize==0)
    {
        CopyMem(TmpList->Items, List->Items, List->ItemCount*sizeof(VOID*));
        return EFI_SUCCESS;
    }
    for (i = 0; i < List->ItemCount; i++)
    {
        TmpList->Items[i] = NULL;

        if (List->Items[i])
        {
            TmpList->Items[i] = AllocateZeroPool(ItemSize);

            if (!TmpList->Items[i])
            {
                return EFI_OUT_OF_RESOURCES;
            }

            CopyMem(TmpList->Items[i], List->Items[i], ItemSize);
        }
    }

    return EFI_SUCCESS;
}

/**
    Find the item in the List

    @param List        List to find the item in
    @param Item        Item to search for
    @param ItemIndex   On success, if ItemIndex is not NULL,
                       it is updated with the index of the found item.

    @retval TRUE    Item has been found in the List. ItemIndex is updated with the item index.
    @retval TRUE    Item has not been found in the List
**/
BOOLEAN AmiItemListFind(
    IN AMI_ITEM_LIST *List, IN VOID* Item, OUT UINTN *ItemIndex OPTIONAL
){
    UINTN i;

    for(i=0; i < List->ItemCount; i++){
        if(List->Items[i]==Item) {
            if (ItemIndex!=NULL) {
                *ItemIndex=i;
            }
            return TRUE;
        }
    }

    return FALSE;
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
