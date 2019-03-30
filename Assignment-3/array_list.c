#include "array_list.h"

#include <stdlib.h>
#include <string.h>

static int array_list_resize (struct array_list_t *list, int size)
{
    list->size = size;
    
    if ((list->memory = realloc(list->memory, (sizeof(void *) * size))) == NULL) {
        return 1;
    }
    
    return 0;
}


int array_list_create (struct array_list_t *list, int initial_size)
{
    list->length = 0;
    list->memory = NULL;
    
    // Initial size must be at least 2 or the array will not grow
    return array_list_resize(list, (initial_size < 2) ? 2 : initial_size);
}

void *array_list_get (const struct array_list_t *list, int index)
{
    return list->memory[index];
}

int array_list_append (struct array_list_t *list, void *item)
{
    return array_list_add(list, list->length, item);
}

int array_list_add (struct array_list_t *list, int index, void *item)
{
    if (list->size <= list->length) {
        // Nead to resize array
        if (array_list_resize(list, (int)(list->size * 1.5))) {
            return 1;
        }
    }
    
    if (index != list->length) {
        // Need to shift values
        memmove(list + index, list + index + 1, list->length - index);
    }
    
    list->memory[index] = item;
    list->length++;
    
    return 0;
}

void *array_list_pop (struct array_list_t *list, const int index)
{
    void *tmp = list->memory[index];
    
    if (index != list->length) {
        // Need to shift values
        memmove(list + index + 1, list + index, list->length - index - 1);
    }
    
    list->length --;
    
    return tmp;
}

void array_list_remove (struct array_list_t *list, const int index)
{
    free(array_list_pop(list, index));
}

int array_list_array (const struct array_list_t *list, void **buffer,
                      size_t size)
{
    int len = (size < list->length) ? size: list->length;
    memcpy(buffer, list->memory, len);
    return len;
}

void array_list_free (struct array_list_t *list)
{
    for (int i = 0; i < list->length; i++) {
        free(list->memory[i]);
    }
}
