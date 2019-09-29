#pragma once
#include <stdlib.h>
#include "fslist.h"

struct timer_logic
{
    struct fslist nodes;
    size_t idGen;
};

struct timer_logic_info
{ 
    size_t timerId;
    size_t triggerTime;
    void ( *callback )( void* );
    void* callbackObj;
};

struct timer_handle
{
    struct fslist_node* n;
    size_t timerId;
};

typedef struct timer_logic timer_logic_t;
typedef struct timer_handle timer_handle_t;
typedef struct timer_logic_info timer_info_t;

//! \brief      Initiate new timer. This function initializes internal fslist.
//! \returns    Number of maximum timers.
size_t timer_init( timer_logic_t* s, void* buff, size_t buffSize );

//! \brief      Allocates new timer. 
timer_handle_t timer_add( timer_logic_t* s, size_t whenToTrigger, void ( *callback )( void* ), void* callbackObj );

//! \brief      Update timer based on given time parameter.
void timer_update( timer_logic_t* s, size_t curTime );

//! \brief      Get closest timer's trigger time
static inline
size_t timer_nextTrigger( timer_logic_t* s )
{
    if ( s->nodes.size == 0 )
    {
        return ( size_t) -1;
    }

    return ( ( timer_info_t*) fslist_data( &s->nodes, s->nodes.get + s->nodes.head ) )->triggerTime;
}

//! \brief      Browse timer handle
static inline
timer_info_t const* timer_browse( timer_logic_t* s, timer_handle_t h )
{
    if ( h.n->isValid )
    {
        timer_info_t* info = ( timer_info_t*) fslist_data( &s->nodes, h.n );
        if ( info->timerId == h.timerId )
            return info;
    }
    return NULL;
}

//! \brief      Check if given timer is active
static inline
bool timer_isActive( timer_logic_t* s, timer_handle_t h )
{
    return timer_browse( s, h ) != NULL;
}

//! \brief      Remove allcoated timer.
static inline
void timer_erase( timer_logic_t* s, timer_handle_t h )
{
    if ( timer_isActive( s, h ) )
    {
        fslist_erase( &s->nodes, h.n );
    }
}