//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Tree.c
    USB tree manipulation routines

**/


#include "Uhcd.h"

#include "Tree.h"

/**
    initializes TREENODE_T structure

    @param  Node  Pointer to TREENODE_T structure
    @param  Data  Pointer to data structure specific to
                  the type of tree node

    @retval Pointer to TREENODE_T structure that was passed in as parrameter

**/

TREENODE_T*
TreeCreate (
    TREENODE_T  *Node,
    VOID        *Data
)
{
    Node->data = Data;
    Node->child = 0;
    Node->right = 0;
    Node->left = 0;
    Node->parent = 0;
    return Node;
}


/**
    add a child node to the TREENODE_T structure

    @param    Parent  Pointer to parent TREENODE_T structure
    @param    Child   Pointer to child TREENODE_T structure

    @retval   None

**/

VOID
TreeAddChild (
    TREENODE_T  *Parent,
    TREENODE_T  *Child
)
{
    TREENODE_T* Node = Parent->child;
    Parent->child = Child;
    Child->right = Node;
    if(Node != 0)
        Node->left = Child;
    Child->left = 0;
    Child->parent = Parent;
}


/**
    removes a node from the tree

    @param    Node  Pointer to TREENODE_T structure

    @retval   None
**/

VOID
TreeRemove (
    TREENODE_T    *Node
)
{
    if(  Node->right != 0){
        Node->right->left = Node->left;
    }
    if(  Node->left != 0){
        Node->left->right = Node->right;
    }
    if( Node->parent && Node->parent->child == Node )
        Node->parent->child = Node->right;
    Node->left = 0;
    Node->right = 0;
    Node->parent = 0;
}


/**
    Enumerates nodes of the tree which are direct children of
    the same parent

    @param    Node        Pointer to TREENODE_T structure
    @param    Predicate   Predicate function that is called for each node
                          and controll whether enumeration should continue
                          once predicate returns TRUE the enumeration will
    @param    Data        Pointer that is passed to predicate to maintain
                          the context of the enumeration

    @retval the node that cause enumeration to stop
**/

TREENODE_T*
TreeSearchSibling (
    TREENODE_T         *Node,
    TREE_PREDICATE1_T  Predicate,
    VOID               *Data
)
{
    TREENODE_T *Right;
    for(; Node; Node = Right){
        Right = Node->right;
        if(Predicate(Node->data, Data))return Node;
    }
    return NULL;
}


/**
    Enumerates nodes of the tree which are direct and indirect
    children of the same parent

    @param    Node       Pointer to TREENODE_T structure
    @param    Predicate  Predicate function that is called for each node;
                         controlls whether enumeration should continue
                         once predicate returns TRUE the enumeration will
    @param    Data       Pointer that is passed to predicate to maintain
                         the context of the enumeration

    @retval the node that cause enumeration to stop
**/

TREENODE_T*
TreeSearchDeep (
    TREENODE_T         *Node,
    TREE_PREDICATE1_T  Predicate,
    VOID               *Data
)
{
    TREENODE_T *Right;
    TREENODE_T *Child;
    for(; Node; Node=Right){
        Right = Node->right;
        Child = Node->child;
        if(Predicate(Node->data, Data))return Node;
        if(Child){
            TREENODE_T* Child1 = TreeSearchDeep(Node->child, Predicate, Data);
            if(Child1)
                return Child1;
        }
    }
    return 0;
}


/**
    Enumerates nodes of the tree which are direct children of
    the same parent; In contrust to TreeSearchSibling this
    function ignores the result returned from call-back routine
    and always enumerates all sibling nodes

    @param    Node       Pointer to TREENODE_T structure
    @param    CallBack   Call-back function that is called for each node
    @param    Data       Pointer that is passed to call-back to maintain
                         the context of the enumeration

    @retval   None

**/

VOID
TreeForEachSibling (
    TREENODE_T       *Node,
    TREE_CALLBACK_T  CallBack,
    VOID             *Data
)
{
    for(; Node; Node = Node->right)
        CallBack(Node->data, Data);
}


/**
    retrieves data stored at the tail of the queue and
    removes the tail item

    @param    Queue  Pointer to QUEUE_T structure

    @retval   Data   Pointer to QUEUE_T data

**/

VOID*
QueueGet (
    QUEUE_T  *Queue
)
{
    VOID* Data;
    if( Queue->tail == Queue->head ) return NULL;
    Data = Queue->data[Queue->tail++];
    if( Queue->tail == Queue->maxsize ) Queue->tail -= Queue->maxsize;
    return Data;
}


/**
    retrieves number of items stored in the queue

    @param    Queue  Pointer to QUEUE_T structure

    @retval   Size   Queue Size

**/

int
QueueSize (
    QUEUE_T  *Queue
)
{
    return (Queue->head >= Queue->tail)? Queue->head - Queue->tail:
            Queue->head  + Queue->maxsize - Queue->tail;
}


/**
    add a new item in front of the head of the queue

    @param    Queue  Pointer to QUEUE_T structure
    @param    Data   Pointer to QUEUE_T data

    @retval   None

**/

VOID
QueuePut (
    QUEUE_T  *Queue,
    VOID     *Data
)
{
    ASSERT(QueueSize(Queue) < Queue->maxsize );
    Queue->data[Queue->head++] = Data;
    if(Queue->head == Queue->maxsize) Queue->head -= Queue->maxsize;
    if(Queue->head == Queue->tail){
        //Drop data from queue
        Queue->tail++;
        if( Queue->tail == Queue->maxsize ) Queue->tail -= Queue->maxsize;
    }
}


/**
    add a variable size item to the queue

    @param    Queue  Pointer to QUEUE_T structure
    @param    Data   Pointer to data
    @param    Size   Number of dwords to add to the queue

    @retval   None

**/

VOID
QueuePutMsg (
    QUEUE_T  *Queue,
    VOID     *Data,
    int      Size
)
{
    ASSERT(QueueSize(Queue) < Queue->maxsize);
    ASSERT(Size < Queue->maxsize);
    if(Queue->head + Size > Queue->maxsize)
        Queue->head = 0;
    CopyMem( (char*)Queue->data + Queue->head, Data, Size );
    Queue->head += Size;
    if(Queue->head == Queue->maxsize) Queue->head = 0;
    if(Queue->head == Queue->tail){
        //Drop data from queue
        Queue->tail += Size;
        if( Queue->tail >= Queue->maxsize ) Queue->tail = 0;
    }
}

/**
    retrieves a variable size item from the queue

    @param    Queue  Pointer to QUEUE_T structure
    @param    Size   Number of dwords to remove from the queue

    @retval   Data   Pointer to data

**/

VOID*
QueueRemoveMsg (
    QUEUE_T  *Queue,
    int      Size
)
{
    VOID* Data;
    if( Queue->tail == Queue->head ) return NULL;
    Data = (char*)Queue->data + Queue->tail;
    Queue->tail += Size;
    if( Queue->tail > Queue->maxsize ){
        Data = Queue->data;
        Queue->tail = Size;
    } else if(Queue->tail == Queue->maxsize ){
        Queue->tail = 0;
    }
    return Data;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
