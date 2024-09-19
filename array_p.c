


#include "array_p.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

GArray *
GArrayCreateP(
    uint32_t base_allocate
    )
{
    const uint32_t item_size = sizeof(char *);
    GArray *ret = malloc(sizeof(GArray));

    if(ret)
    {
        uint8_t status = GArrayCreateFilled8(ret, item_size, base_allocate);
        if(status == EXIT_FAILURE)
        {
            GArrayWipe8(ret);
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

int
GArrayCreateFilledP(
    GArray *array_return,
    uint32_t base_allocate
    )
{
    if(!array_return || !item_size)
    {   return EXIT_FAILURE;
    }
    array_return->data = NULL;
    array_return->data_len = 0;
    array_return->data_len_real = 0;
    GArrayResize8(array_return, base_allocate);
    return EXIT_SUCCESS;
}

void
GArrayWipeP(
    GArray *array
    )
{
    if(!array)
    {   return;
    }
    free(array->data);
}

int
GArrayResizeP(
    GArray *array,
    uint32_t item_len
    )
{
    if(!array)
    {   return EXIT_FAILURE;
    }

    if(array->data_len == item_len)
    {   return EXIT_SUCCESS;
    }

    const uint32_t item_size = sizeof(char *);
    const float MIN_GROWTH = 1.25f;
    
    if(item_len == 0)
    {   
        free(array->data);
        array->data = NULL;
    }
    else if(!array->data)
    {   
        uint64_t size = item_len * MIN_GROWTH;
        array->data = malloc(item_size * size);
        if(!array->data)
        {   return EXIT_FAILURE;
        }
        array->data_len_real = size;
    }
    else
    {
        const uint8_t toosmall = array->data_len_real < item_len;
        uint64_t size;
        if(toosmall)
        {   size = item_len + (item_len / (array->data_len_real + !array->data_len_real));
        }
        else
        {   size = array->data_len_real - (array->data_len_real - item_len);
        }
        
        void *rec = realloc(array->data, item_size * size);
        if(!rec )
        {   return EXIT_FAILURE;
        }
        array->data = rec;
    }

    array->data_len = item_len;

    return EXIT_SUCCESS;
}
int
GArrayPushBackP(
    GArray *array,
    void *item_cpy
    )
{
    if(!array || !item_cpy)
    {   return EXIT_FAILURE;
    }

    const uint32_t item_size = sizeof(char *);

    uint8_t status = GArrayResize8(array, array->data_len + 1);
    if(status == EXIT_SUCCESS)
    {   
        uint8_t *data = array->data;
        uint8_t *dest = data + (array->data_len - 1) * item_size;
        uint8_t *src = item_cpy;
        uint32_t size = item_size;

        memmove(dest, src, size);
    }
    return status;
}

int
GArrayPopBackP(
    GArray *array
    )
{
    if(!array)
    {   return EXIT_FAILURE;
    }

    if(array->data)
    {   
        /* make sure no underflow */
        if(array->item_len)
        {   GArrayResize8(array, array->item_len - 1);
        }
    }
    return EXIT_SUCCESS;
}

int
GArrayReplaceP(
    GArray *array,
    void *item_cpy,
    uint32_t index
    )
{
    if(!array)
    {   return EXIT_FAILURE;
    }
    if(index > array->data_len)
    {   return EXIT_FAILURE;
    }

    const uint32_t item_size = sizeof(char *);
    uint8_t *data = array->data;
    uint8_t *dest = data + index * item_size;
    uint8_t *src = item_cpy;

    if(item_cpy)
    {   memmove(dest, src, item_size);
    }
    else
    {   memset(dest, 0, item_size);
    }
    return EXIT_SUCCESS;
}

int
GArrayInsertP(
    GArray *array,
    void *item_cpy,
    uint32_t index
    )
{
    if(!array)
    {   return EXIT_FAILURE;
    }
    if(index >= array->data_len)
    {   return EXIT_FAILURE;
    }

    uint8_t status = GArrayResize8(array, array->data_len + 1);

    if(status == EXIT_SUCCESS)
    {   
        const uint32_t item_size = sizeof(char *);

        uint8_t *data = array->data;
        uint8_t *dest = data + (index + 1) * item_size;
        uint8_t *src = data + index * item_size;

        uint32_t move_size = (array->data_len - index - 1) * item_size;

        memmove(dest, src, move_size);
        if(item_cpy)
        {   memmove(src, item_cpy, item_size);
        }
        else
        {   memset(src, 0, item_size);
        }
    }

    return status;
}

int
GArrayDeleteP(
    GArray *array,
    uint32_t index
    )
{
    if(!array)
    {   return EXIT_FAILURE;
    }
    if(index >= array->data_len)
    {   return EXIT_FAILURE;
    }

    const uint32_t item_size = sizeof(char *);
    const uint32_t BYTES_MOVE = (array->data_len - index - 1) * item_size;

    uint8_t *data = array->data;
    uint8_t *src = data + (item_size * (index + 1));
    uint8_t *dest = data + (item_size * index);

    /* Check if last so no invalid memove */
    if(index < array->data_len - 1)
    {   memmove(dest, src, BYTES_MOVE);
    }

    GArrayResize8(array, array->data_len - 1);
    return EXIT_SUCCESS;
}


void *
GArrayAtP(
        GArray *array,
        uint32_t index
        )
{
    if(array->data_len < index)
    {   return NULL;
    }
    const uint32_t item_size = sizeof(char *);
    return (uint8_t *)array->data + (index * item_size);
}

int
GArrayAtSafeP(
        GArray *array,
        uint32_t index,
        void *fill_return
        )
{
    const uint32_t item_size = sizeof(char *);

    void *data = GArrayAt8(array, index);
    int ret = EXIT_FAILURE;
    if(fill_return)
    {
        if(data)
        {
            memmove(fill_return, data, item_size);
            ret = EXIT_SUCCESS;
        }
    }
    return ret;
}


uint32_t
GArrayEndP(
        GArray *array
        )
{
    if(array)
    {   return array->data_len;
    }
    return 0;
}

uint32_t 
GArrayStartP(
        GArray *array
        )
{   return (const unsigned int) 0;
}
