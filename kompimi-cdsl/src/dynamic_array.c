/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <malloc.h>
#include <string.h>

#include "dynamic_array.h"

/*! \brief Verifies the current list data structure is valid and
    satisfies all algorithmic invariants. */
static void check_dynamic_array_invariants(dynamic_array* array);

dynamic_array dynamic_array_create_func(int element_size, int initial_size)
{
    dynamic_array result;
    result.size = result.capacity = initial_size;
    result.data = malloc(element_size * initial_size);
    check_dynamic_array_invariants(&result);
    return result;
}

void dynamic_array_destroy(dynamic_array* array)
{
    array->size = array->capacity = -1;
    free(array->data);
    array->data = NULL;
}

int dynamic_array_resize_func(
    dynamic_array* array, int element_size, int size)
{
    if (size < (int)(array->capacity * DYNAMIC_ARRAY_MINIMUM_LOAD) ||
        size > array->capacity)
    {
        int new_capacity = 1 + (int)(size /
            ((size > array->capacity) ? DYNAMIC_ARRAY_LOAD_AFTER_EXPANSION
                                      : DYNAMIC_ARRAY_LOAD_AFTER_SHRINKING));
        void* new_data = realloc(array->data, element_size * new_capacity);
        if (new_data == NULL)
        {
            return 0;
        }
        array->data = new_data;
        array->capacity = new_capacity;
    }
    array->size = size;
    check_dynamic_array_invariants(array);
    return 1;
}

int dynamic_array_insert_range_func(dynamic_array* array, int element_size, int index_start, int length)
{
    if (!dynamic_array_resize_func(
            array, element_size, array->size + length))
    {
        return 0;
    }
    memmove((char *)array->data + element_size*(index_start + length),
            (char *)array->data + element_size*index_start,
            element_size * (array->size - index_start));
    check_dynamic_array_invariants(array);
    return 1;
}

int dynamic_array_insert_end_func(dynamic_array* array, int element_size, void* new_value)
{
    if (!dynamic_array_resize_func(array, element_size, array->size + 1))
    {
        return 0;
    }
    memmove((char *)array->data + element_size*(array->size - 1),
            (char *)new_value, element_size);
    check_dynamic_array_invariants(array);
    return 1;
}

int dynamic_array_insert_at_func(dynamic_array* array, int element_size, int index, void* new_value)
{
    if (!dynamic_array_resize_func(array, element_size, array->size + 1))
    {
        return 0;
    }
    memmove((char *)array->data + element_size*(index + 1),
            (char *)array->data + element_size*index,
            element_size * (array->size - (index + 1)));
    memmove((char *)array->data + element_size*index,
            (char *)new_value, element_size);
    check_dynamic_array_invariants(array);
    return 1;
}

void dynamic_array_remove_end_func(dynamic_array* array, int element_size)
{
    dynamic_array_resize_func(array, element_size, array->size - 1);
}

void dynamic_array_remove_range_func(dynamic_array* array, int element_size, int index_start, int length)
{
    if (index_start + length < array->size)
    {
        memmove((char *)array->data + element_size*index_start,
                (char *)array->data + element_size*(index_start + length),
                element_size * (array->size - (index_start + length)));
    }
    dynamic_array_resize_func(array, element_size, array->size - length);
    check_dynamic_array_invariants(array);
}

int dynamic_array_reserve_func(dynamic_array* array, int element_size, int capacity)
{
    if (capacity > array->capacity)
    {
        int new_capacity = capacity;
        void* new_data = realloc(array->data, element_size * new_capacity);
        if (new_data == NULL)
        {
            return 0;
        }
        array->data = new_data;
        array->capacity = new_capacity;
    }
    check_dynamic_array_invariants(array);
    return 1;
}

void dynamic_array_swap_func(dynamic_array* array1, dynamic_array* array2, int element_size)
{
    /* Just use memberwise struct copy */
    dynamic_array temp;
    temp = *array1;
    *array1 = *array2;
    *array2 = temp;
    check_dynamic_array_invariants(array1);
    check_dynamic_array_invariants(array2);
}

static void check_dynamic_array_invariants(dynamic_array* array)
{
#ifndef NDEBUG
    assert (array->size >= 0);
    assert (array->size <= array->capacity);
    assert (array->data != NULL);
#endif
}
