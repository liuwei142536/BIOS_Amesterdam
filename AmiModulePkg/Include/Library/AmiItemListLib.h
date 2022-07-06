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
  AmiItemList library class definition. Array-based list of pointers.

  The library class defines a pointer container based on reallocatable array.
*/

#ifndef __AMI_ITEM_LIST_LIB__H__
#define __AMI_ITEM_LIST_LIB__H__

//Structure to store pointers to data using reallocatable array.
typedef struct {
    UINTN   InitialCount;
    UINTN   ItemCount;
    VOID    **Items;
}AMI_ITEM_LIST;

/**
  Initializes AMI_ITEM_LIST data structure declared as a global variable
  or as a field of the global variable.

  AMI_ITEM_LIST data structure must be initialized using
  AMI_ITEM_LIST_VARIABLE_INITIALIZE()/AMI_ITEM_LIST_VARIABLE_INITIALIZE_EX() macros
  or AmiItemListInitialize()/AmiItemListInitializeEx() functions before
  any other item list function can be used.
**/
#define AMI_ITEM_LIST_VARIABLE_INITIALIZE()  {0, 0, NULL}

/**
  Initializes AMI_ITEM_LIST data structure declared as a global variable
  or as a field of the global variable.

  AMI_ITEM_LIST data structure must be initialized using
  AMI_ITEM_LIST_VARIABLE_INITIALIZE()/AMI_ITEM_LIST_VARIABLE_INITIALIZE_EX() macros
  or AmiItemListInitialize()/AmiItemListInitializeEx() functions before
  any other item list function can be used.

  @param InitialCount   Initial number of elements in the item list array
**/
#define AMI_ITEM_LIST_VARIABLE_INITIALIZE_EX(InitialCount)  {(InitialCount), 0, NULL}

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
EFI_STATUS AmiItemListInitialize(AMI_ITEM_LIST *List);

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
EFI_STATUS AmiItemListInitializeEx(AMI_ITEM_LIST *List, UINTN InitialCount);

/**
    Appends the provided object to List.

    @param List List to be appended to.
    @param Item Object to be appended.

    @retval EFI_OUT_OF_RESOURCES,  if memory allocation fails.
             Otherwise, EFI_SUCCESS.

**/
EFI_STATUS AmiItemListAppend(AMI_ITEM_LIST *List, VOID* Item);

/**
     Inserts the provided item pointed to by pRes into the provided List at index ItemIndex.
    @param List List to be inserted into.
    @param Item Pointer to object to place in list.
    @param ItemIndex    Index in list to place the object pointed to by pRes.

    @retval EFI_INVALID_PARAMETER,  if the ItemIndex value is invalid.
            EFI_OUT_OF_RESOURCES,   if there is not enough memory for the new object.
            Otherwise, EFI_SUCCESS.

**/
EFI_STATUS AmiItemListInsert(AMI_ITEM_LIST *List, VOID* Item, UINTN ItemIndex);

/**
    Deletes the object stored at the provided ItemIndex in List.
    Frees the memory associated with the object pointer if FreeData is TRUE.

    @param List List to be deleted from.
    @param ItemIndex    Index inside List->Items to be deleted.
    @param FreeData TRUE will free the memory associated with the deleted object.

    @retval EFI_INVALID_PARAMETER,  if the provided ItemIndex is invalid.
            Otherwise, EFI_SUCCESS.

**/
EFI_STATUS AmiItemListDelete(AMI_ITEM_LIST *List, UINTN ItemIndex, BOOLEAN FreeData);

/**
    Clears all the items of List.  If FreeData is TRUE, frees all the memory associated
    with the stored objects.

    @param List         List to be cleared.
    @param FreeData     If TRUE, all objects are freed.

    @retval VOID
**/
VOID AmiItemListClear(AMI_ITEM_LIST *List, BOOLEAN FreeData);

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
);

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
);

#endif //__AMI_ITEM_LIST_LIB__H__
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
