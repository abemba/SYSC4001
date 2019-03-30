#ifndef array_list_h
#define array_list_h

#include <stdlib.h>

struct array_list_t {
    void **memory;
    int size;
    int length;
};

/**
 *  Create a new array list.
 *
 *  @param list Pointer to the array list descriptor
 *  @param initial_size The initial ammount of memory to allocate
 *
 *  @return 0 if successfull
 */
extern int array_list_create (struct array_list_t *list, int initial_size);

/**
 *  Gets an item from a list.
 *  @param list The list from witch to get the item.
 *  @param index The index of the item to get.
 *
 *  @return The item at the specified index.
 */
extern void *array_list_get (const struct array_list_t *list, int index);

/**
 *  Append an item to a list
 *  @param list The list to which the item should be appended.
 *  @param item The item to be added to the list
 *
 *  @return 0 if successfull
 */
extern int array_list_append (struct array_list_t *list, void *item);

/**
 *  Add an item to a list at a specified index
 *  @param list The list to which the item should be added.
 *  @param index The index at which the item should be added.
 *  @param item The item to be added to the list
 *
 *  @return 0 if successfull
 */
extern int array_list_add (struct array_list_t *list, int index, void *item);

/**
 *  Pop an item from a list.
 *  @note It is the responsability of the calling function to free the returned pointer. It will no longer be managed by the list.
 *  @param list The list from which the item should be poped.
 *  @param index The index which should be poped from the list.
 *
 *  @return The value which was poped from the list
 */
extern void *array_list_pop (struct array_list_t *list, const int index);

/**
 *  Remove an item from a list (and free it).
 *  @param list The list from which the item should be removed.
 *  @param index The index which should be removed from the list.
 */
extern void array_list_remove (struct array_list_t *list, const int index);

/**
 *  Generate an array with the contents of a list.
 *  @param list The list from which the array should be created.
 *  @param buffer The array which should contain the items in the list.
 *  @param size The maximum number of items to add to the buffer.
 *
 *  @return The number of items which where added to the array,
 */
extern int array_list_array (const struct array_list_t *list, void **buffer,
                             size_t size);

/**
 *  Frees all nodes of a list
 *  @param list The list for which all nodes should be freed
 */
extern void array_list_free (struct array_list_t *list);



#endif /* array_list_h */
