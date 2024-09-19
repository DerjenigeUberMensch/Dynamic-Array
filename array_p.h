#ifndef __ARRAY__P__H__
#define __ARRAY__P__H__


#include "dynamic_array.h"

typedef struct __GArray__ GArrayP;


GArray *
GArrayCreateP(
    uint32_t base_allocate
    );

int
GArrayCreateFilledP(
    GArray *array_return,
    uint32_t base_allocate
    );

void
GArrayWipeP(
    GArray *array
    );

int
GArrayResizeP(
    GArray *array,
    uint32_t item_len
    );

int
GArrayPopBackP(
    GArray *array
    );

int
GArrayReplaceP(
    GArray *array,
    void *item_cpy,
    uint32_t index
    );

int
GArrayInsertP(
    GArray *array,
    void *item_cpy,
    uint32_t index
    );

int
GArrayDeleteP(
    GArray *array,
    uint32_t index
    );


void *
GArrayAtP(
        GArray *array,
        uint32_t index
        );

int
GArrayAtSafeP(
        GArray *array,
        uint32_t index,
        void *fill_return
        );

uint32_t
GArrayEndP(
        GArray *array
        );

uint32_t 
GArrayStartP(
        GArray *array
        );


#endif
