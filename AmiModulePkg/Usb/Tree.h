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

/** @file Tree.h
    Tree routines header

**/

#ifndef __TREE_H__
#define __TREE_H__

typedef struct _TREENODE_T TREENODE_T;
struct _TREENODE_T {
    VOID        *data;
    TREENODE_T  *right;
    TREENODE_T  *left;
    TREENODE_T  *child;
    TREENODE_T  *parent;
};



typedef int (*TREE_PREDICATE1_T)(VOID *Node, VOID *Context);
typedef VOID (*TREE_CALLBACK_T)(VOID *Node, VOID *Context);

TREENODE_T* TreeCreate (TREENODE_T *Node, VOID *Data);
VOID        TreeAddChild (TREENODE_T *Parent, TREENODE_T *Child);
VOID        TreeRemove (TREENODE_T *Node);
TREENODE_T* TreeSearchSibling (TREENODE_T *Node, TREE_PREDICATE1_T Predicate, VOID *Data);
TREENODE_T* TreeSearchDeep (TREENODE_T *Node, TREE_PREDICATE1_T Predicate, VOID *Data);
VOID        TreeForEachSibling (TREENODE_T *Node, TREE_CALLBACK_T Predicate, VOID *Data);

VOID* QueueGet (QUEUE_T *Queue);
int   QueueSize (QUEUE_T *Queue);
VOID  QueuePut (QUEUE_T *Queue, VOID *Data);
VOID* QueueRemoveMsg (QUEUE_T *Queue, int Size);
VOID  QueuePutMsg (QUEUE_T *Queue, VOID *Data, int Size);

#endif //__TREE_H__

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
