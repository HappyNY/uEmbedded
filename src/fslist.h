#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "assert.h"

//! \brief      16bit unsigned integer wil be used to indicate list node index. Therefore only 65535 nodes can be allocated for single list.
typedef uint16_t fslist_idx_t;

//! \breif      Constant value that indicates invaid node index
enum { FSLIST_NODEIDX_NONE = ( fslist_idx_t) -1; };

//! \brief      Free space list.
struct fslist
{
    //! \brief      First reference of active node.
    fslist_idx_t head;

    //! \breif      Last reference of active nodes.
    fslist_idx_t tail;

    //! \brief      First reference of inactive(=available) node.
    fslist_idx_t nodeInactive;

    //! \brief      Number of maximum nodes.
    fslist_idx_t capacity;

    //! \brief      Active node count. Corresponds stl container's size() method.
    fslist_idx_t size;

    //! \brief      Size of single element. Can be any size.
    size_t elemSize;

    //! \brief      Allocated memory for list. This must not be deallocated during list is active.
    char* buff;

    //! \brief      Easy accessor for referencing nodes
    struct fslist_node* get;

    //! \brief      Easy accessor for referencing element data. This member is for internal use !
    char* data;
};
 
//! \brief      List node struct.
struct fslist_node
{
    fslist_idx_t prev;
    fslist_idx_t next;
};

/*! \brief      Initiate node struct 
    \param      buff Buffer to be used internally. This memory chunk must be valid during usage. After the deallocation of fslist, the finalization of this memory chunk is up to the programmer. 
    \param      buffSize Size of passed buffer.
    \returns    Number of actual available nodes. */
size_t fslist_init( struct fslist* s, void* buff, size_t buffSize );

/*! brief       Checks if given node is the node of given list s */
static inline
bool fslist_node_in_range( struct fslist* s, struct fslist_node* n )
{
    return n < s->get + s->capacity && s->get < n;
}

/*! \brief      Get index of list node */
static inline
fslist_idx_t fslist_idx( struct fslist const* s, struct fslist_node const* n )
{
    if ( !fslist_node_in_range( s, n ) )
        return FSLIST_NODEIDX_NONE;
    else
        return n - s->get;
}

/*! \brief      Jump to next node */
static inline
struct fslist_node* fslist_next( struct fslist* s, struct fslist_node* n )
{
    return n->next != FSLIST_NODEIDX_NONE ? s->get + n->next : NULL;
}

/*! \brief      Jump to previous node. */
static inline
struct fslist_node* fslist_prev( struct fslist* s, struct fslist_node* n )
{
    return n->prev != FSLIST_NODEIDX_NONE ? s->get + n->prev : NULL;
}

/*! \brief      Get data from node */
static inline
void* fslist_data( struct fslist* s, struct fslist_node* n )
{
    return ( void*) ( s->data + ( fslist_idx( s, n ) * s->elemSize ) );
}

/*! \brief      Apply same process to all active nodes iteratively. */
static inline
void fslist_forEach( struct fslist* s, void( *callback )( void* ) )
{
    struct fslist_node* n;
    
    if ( s->size == 0 ) 
        return;

    for ( n = s->get + s->head; n; n = fslist_next( s, n ) )
        callback( fslist_data( s, n ) );
}

/*! \brief      Insert new node previous given node. Pass nullptr to push back. */
struct fslist_node* fslist_insert( struct fslist* s, struct fslist_node* n );

/*! \brief      Remove given node from list. */
struct fslist_node* fslist_erase( struct fslist* s, struct fslist_node* n );

/*! \brief      Append new node at last. */
void fslist_pushBack( struct fslist* s );

/*! \brief       Append new node at first */
static inline
void fslist_pushFront( struct fslist* s );

/*! \brief      Pop node at last */
static inline
void fslist_popBack( struct fslist* s );

/*! \brief      Pop node at first */
static inline
void fslist_popFront( struct fslist* s );