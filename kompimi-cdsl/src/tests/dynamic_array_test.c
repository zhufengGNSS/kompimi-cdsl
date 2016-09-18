/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../dynamic_array.h"

void test_create_destroy(int list_size)
{
    dynamic_array a = dynamic_array_create(int, list_size);
    dynamic_array_destroy(&a);
}

void test_fill_no_grow(int list_size)
{
    dynamic_array a = dynamic_array_create(int, list_size);
    int i;
    for (i=0; i < list_size; i++)
    {
        SET_IDX(a, int, i, i);
    }
    for (i=0; i < list_size; i++)
    {
        assert(IDX(a, int, i) == i);
    }
    dynamic_array_destroy(&a);
}

void test_iterate(int list_size)
{
    dynamic_array a = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < list_size; i++)
    {
        dynamic_array_insert_end(&a, int, &i);
    }
    i = 0;
    DYNAMIC_ARRAY_ITERATE(a, int, iter)
        assert(*iter == i);
        i++;
    DYNAMIC_ARRAY_ITERATE_END()
    dynamic_array_destroy(&a);
}

void test_insert_end(int list_size)
{
    dynamic_array a = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < list_size; i++)
    {
        dynamic_array_insert_end(&a, int, &i);
    }
    for (i=0; i < list_size; i++)
    {
        assert(IDX(a, int, i) == i);
    }
    dynamic_array_destroy(&a);
}

void test_insert_remove_end(int list_size)
{
    dynamic_array a = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < list_size; i++)
    {
        dynamic_array_insert_end(&a, int, &i);
    }
    for (i=0; i < list_size; i++)
    {
        assert(a.size == list_size - i);
        assert(IDX(a, int, 0) == 0);
        assert(IDX(a, int, list_size-i-1) == list_size-i-1);
        dynamic_array_remove_end(&a, int);
    }
    assert(a.size == 0);
    dynamic_array_destroy(&a);
}

void test_insert_at(int list_size)
{
    dynamic_array a = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < list_size; i+=2)
    {
        dynamic_array_insert_end(&a, int, &i);
        assert(a.size == i/2 + 1);
    }
    for (i=1; i < list_size; i+=2)
    {
        dynamic_array_insert_at(&a, int, i, &i);
        assert(a.size == 5000 + i/2 + 1);
    }
    for (i=0; i < list_size; i++)
    {
        assert(IDX(a, int, i) == i);
    }
    dynamic_array_destroy(&a);
}

void test_remove_at(int list_size)
{
    dynamic_array a = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < list_size; i++)
    {
        dynamic_array_insert_end(&a, int, &i);
    }
    for (i=0; i < list_size/2; i++)
    {
        dynamic_array_remove_at(&a, int, i);
        assert(a.size == list_size - i - 1);
    }
    for (i=0; i < list_size/2; i++)
    {
        assert(IDX(a, int, i) == i*2 + 1);
    }
    dynamic_array_destroy(&a);
}

void test_swap(int size1, int size2)
{
    dynamic_array a1 = dynamic_array_create(int, 0);
    dynamic_array a2 = dynamic_array_create(int, 0);
    int i;
    for (i=0; i < size1; i++)
    {
        dynamic_array_insert_end(&a1, int, &i);
    }
    for ( ; i < size1 + size2; i++)
    {
        dynamic_array_insert_end(&a2, int, &i);
    }
    assert (a1.size == size1);
    assert (a2.size == size2);

    dynamic_array_swap(&a1, &a2, int);
    assert (a2.size == size1);
    assert (a1.size == size2);
    i = 0;
    DYNAMIC_ARRAY_ITERATE(a2, int, iter)
        assert(*iter == i);
        i++;
    DYNAMIC_ARRAY_ITERATE_END()
    DYNAMIC_ARRAY_ITERATE(a1, int, iter)
        assert(*iter == i);
        i++;
    DYNAMIC_ARRAY_ITERATE_END()
    assert (i == size1 + size2);
}

int main()
{
    test_create_destroy(10);
    test_fill_no_grow(10);
    test_insert_end(10000);
    test_insert_remove_end(10000);
    test_iterate(10000);
    test_insert_at(10000);
    test_remove_at(10000);
    test_swap(1000, 2000);

    return 0;
}
