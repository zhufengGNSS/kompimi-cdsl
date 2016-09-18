/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

/** @defgroup dynamic_array dynamic_array module
    Structures, macros, and methods supporting the dynamic_array data structure.

   dynamic_array is a dynamic or growable array, which grows and
   shrinks its underlying storage as elements are added or removed,
   similar to std::vector in C++, ArrayList in Java, or List in
   C#. Elements can be added or removed at the end of the array in
   amortized constant time, which makes it efficient for building up
   lists.

   See tests/dynamic_array_test.c for example code.

    @{
*/

#ifndef _DYNAMIC_ARRAY_

#include <assert.h>

#include "config.h"

/*! \brief A dynamic array, an array that grows as elements are added.

   The structure is intended to be stack-allocated or embedded in
   other data structures; it uses external storage.
*/
typedef struct
{
    /*! \brief The current logical number of elements in the array.
        Read-only, use dynamic_array_resize() to modify the size. */
    int size;
    /*! \brief The number of elements that storage has been allocated for.
        Read-only, use dynamic_array_reserve() to reserve storage. */
    int capacity;
    /*! \brief (Internal) Pointer to array data.
        Clients should access data through the IDX() and SET_IDX() macros. */
    void* data;
} dynamic_array;

/*! \brief Gets the value at a given index in a dynamic array.

   Bounds checking is performed only if DYNAMIC_ARRAY_BOUNDS_CHECKING
   is defined in config.c. You can invoke SET_IDX_BOUNDS or
   SET_IDX_NOBOUNDS to override this behavior in specific cases.

   \param array The dynamic array to index into.
   \param type The element type of the dynamic array, as specified at its creation.
   \param idx The index of the value to retrieve.
   \return The value of the dynamic array at the given index.
*/
#if DYNAMIC_ARRAY_BOUNDS_CHECKING
#define IDX(array, type, idx)   IDX_BOUNDS(array, type, idx)
#else
#define IDX(array, type, idx)   IDX_NOBOUNDS(array, type, idx)
#endif

/*! \brief Sets the value at a given index in a dynamic array.

   Bounds checking is performed only if DYNAMIC_ARRAY_BOUNDS_CHECKING
   is defined in config.c. You can invoke SET_IDX_BOUNDS or
   SET_IDX_NOBOUNDS to override this behavior in specific cases.

   \param array The dynamic array to index into.
   \param type The element type of the dynamic array, as specified at its creation.
   \param idx The index of the value to set.
   \param value A value of the specified type to set the array element to.
*/
#if DYNAMIC_ARRAY_BOUNDS_CHECKING
#define SET_IDX(array, type, idx, value)   SET_IDX_BOUNDS(array, type, idx, value)
#else
#define SET_IDX(array, type, idx, value)   SET_IDX_NOBOUNDS(array, type, idx, value)
#endif

/*! \brief Like IDX(), but never uses bounds checking. */
#define IDX_NOBOUNDS(array, type, idx)   ((type *)(array).data)[(idx)]
/*! \brief Like SET_IDX(), but never uses bounds checking. */
#define SET_IDX_NOBOUNDS(array, type, idx, value) \
    (((type *)(array).data)[(idx)] = (value))

/*! \brief Like IDX(), but always uses bounds checking. */
#define IDX_BOUNDS(array, type, idx) \
    (assert((idx) >= 0 && (idx) < (array).size), \
      IDX_NOBOUNDS(array, type, idx))

/*! \brief Like SET_IDX(), but always uses bounds checking. */
#define SET_IDX_BOUNDS(array, type, idx, value) \
    (assert((idx) >= 0 && (idx) < (array).size), \
     (((type *)(array).data)[(idx)] = (value)))

/*! \brief Creates a new dynamic array.
    \param type The type of element the array will contain.
    \param initial_size The number of logical elements initially in the array.
*/
#define dynamic_array_create(type, initial_size) \
    dynamic_array_create_func(sizeof(type), (initial_size))

/*! \brief Destroys a dynamic array.
    Must be called on a dynamic array before it goes out of scope.
    \param array The dynamic array to destroy.
*/
void dynamic_array_destroy(dynamic_array* array);

/*! \brief Reserves underlying storage for expanding the array.

    \param array The dynamic array to reserve space for.
    \param type The type of elements stored in the array.
    \param capacity The desired maximum capacity to support without reallocation.
*/
#define dynamic_array_reserve(array, type, capacity) \
    dynamic_array_reserve_func((array), sizeof(type) (capacity))

/*! \brief Resizes a dynamic array, either extending it or truncating it to
   the given size.

   New elements are uninitialized and may be invalid.

   \return Zero on out of memory, else returns nonzero.
*/
#define dynamic_array_resize(array, type, new_size) \
    dynamic_array_resize_func((array), sizeof(type), (new_size))

/*! \brief Inserts a range of uninitialized values into a dynamic array.

   This function creates space for the new elements but does not
   initialize them. After invoking this function, the values can be
   filled in with SET_IDX(). Requires linear (O(n)) time.

   \param array The dynamic array to insert into.
   \param type The type of elements stored in this array.
   \param index_start The starting index where the elements are inserted.
   \param length The number of new elements to insert.

   \return Zero on out of memory, else returns nonzero.
*/
#define dynamic_array_insert_range(array, type, index_start, length) \
    dynamic_array_insert_range_func(array, sizeof(type), index_start, length)

/*! \brief Inserts a value into a dynamic array at an arbitrary position.

   This function creates space for the new elements but does not
   initialize them. After invoking this function, the values can be
   filled in with SET_IDX(). Requires linear (O(n)) time.

   \param array The dynamic array to insert into.
   \param type The type of elements stored in this array.
   \param index The index where the elements are inserted.
   \param value A pointer to a value of the given type to insert.

   \return Zero on out of memory, else returns nonzero.
*/
#define dynamic_array_insert_at(array, type, index, value) \
    dynamic_array_insert_at_func(array, sizeof(type), index, (void *)(type *)(value))

/*! \brief Inserts an element at the end of the dynamic array.
 
   Amortized constant (O(1)) time.

   \param array The dynamic array to insert into.
   \param type The type of elements stored in this array.
   \param value A pointer to a value of the given type to insert.

   \return Zero on out of memory, else nonzero.
*/
#define dynamic_array_insert_end(array, type, value) \
    dynamic_array_insert_end_func(array, sizeof(type), (void *)(type *)(value))

/*! \brief Removes a range of elements from a dynamic array.

   Removes a range of element indexes from an array, shifting down all
   following elements. Requires linear (O(n)) time.

   \param array The dynamic array to insert into.
   \param type The type of elements stored in this array.
   \param index_start The starting index where the elements are removed from.
   \param length The number of elements to remove.
*/
#define dynamic_array_remove_range(array, type, index_start, length) \
    dynamic_array_remove_range_func(array, sizeof(type), index_start, length)

/*! \brief Removes an element from a dynamic array.

   Removes an element from an arbitrary index of a dynamic
   array. Requires linear (O(n)) time.

   \param array The dynamic array to insert into.
   \param type The type of elements stored in this array.
   \param index The index where the element is removed from.
*/
#define dynamic_array_remove_at(array, type, index) \
    dynamic_array_remove_range_func(array, sizeof(type), (index), 1)

/*! \brief Removes an element from the end of a dynamic array.

   Amortized constant (O(1)) time.

   \param array The nonempty dynamic array to remove from.
   \param type The type of elements stored in this array.
*/
#define dynamic_array_remove_end(array, type) \
    dynamic_array_remove_end_func(array, sizeof(type))

/*! \brief A macro used to help iterate through a dynamic array easily.

   Begins the loop that iterates through the dynamic array. The loop
   is ended with the matching DYNAMIC_ARRAY_ITERATE_END() macro.

   \param array The dynamic array to iterate over.
   \param type The type of elements stored in this array.
   \param iterator The name to use for the iterator which will point successively to each value in the array.
*/
#define DYNAMIC_ARRAY_ITERATE(array, type, iterator) \
    { \
        type * iterator; \
        for ((iterator) = (type *)((array).data); (iterator) != ((type *)(array).data) + (array).size; (iterator)++) \
        {

/*! \brief Closes an array iteration loop opened by DYNAMIC_ARRAY_ITERATE(). */
#define DYNAMIC_ARRAY_ITERATE_END() \
        } \
    }

/*! \brief Cheaply swaps one dynamic array's contents with another's. */
#define dynamic_array_swap(array1, array2, type) \
    dynamic_array_swap_func(array1, array2, sizeof(type))

/*! @cond INCLUDE_HELPERS */

/*! \brief Helper function for dynamic_array_create(). */
dynamic_array dynamic_array_create_func(int element_size, int initial_size);

/*! \brief Helper function for dynamic_array_reserve(). */
int dynamic_array_reserve_func(dynamic_array* array, int element_size, int capacity);

/*! \brief Helper function for dynamic_array_resize(). */
int dynamic_array_resize_func(dynamic_array* array, int element_size, int new_size);

/*! \brief Helper function for dynamic_array_insert_range(). */
int dynamic_array_insert_range_func(dynamic_array* array, int element_size, int index_start, int length);

/*! \brief Helper function for dynamic_array_insert_at(). */
int dynamic_array_insert_at_func(dynamic_array* array, int element_size, int index, void* new_value);

/*! \brief Helper function for dynamic_array_insert_end(). */
int dynamic_array_insert_end_func(dynamic_array* array, int element_size, void* new_value);

/*! \brief Helper function for dynamic_array_remove_range() and dynamic_array_remove_at(). */
void dynamic_array_remove_range_func(dynamic_array* array, int element_size, int index_start, int length);

/*! \brief Helper function for dynamic_array_remove_end(). */
void dynamic_array_remove_end_func(dynamic_array* array, int element_size);

/*! \brief Helper function for dynamic_array_swap(). */
void dynamic_array_swap_func(dynamic_array* array1, dynamic_array* array2, int element_size);

/*! @endcond */ /* INCLUDE_HELPERS */

#endif /* #ifndef _DYNAMIC_ARRAY_ */

/** @} */ /* end of group dynamic_array */
