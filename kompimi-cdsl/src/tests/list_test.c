/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../list.h"

void test_create_destroy()
{
    list lst = list_create();
    list_destroy(&lst);
}

void test_insert_end_iterate(int list_size)
{
    list lst = list_create();
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    list_destroy(&lst);
}

void test_insert_beginning_iterate(int list_size)
{
    list lst = list_create();
    int i;
    for (i=list_size - 1; i >= 0; i--)
    {
        list_insert_beginning(&lst, (void *)i);
    }
    assert (lst.size == list_size);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    list_destroy(&lst);
}

void test_insert_beginning_end_iterate(int list_size)
{
    list lst = list_create();
    int i;
    for (i=list_size/2; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
        list_insert_beginning(&lst, (void *)(list_size-i-1));
    }
    assert (lst.size == list_size);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    list_destroy(&lst);
}

void test_insert_after_iterate(int list_size)
{
    list lst = list_create();
    int i;
    for (i=0; i < list_size; i+=2)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size/2);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
	if ((i % 2) == 0)
	{
	    int success = list_insert_after(&iter, (void *)(i + 1));
	    assert(success);
	}
        i++;
    LIST_ITERATE_END()
    assert (lst.size == list_size);
    list_destroy(&lst);
}

void test_insert_before_iterate(int list_size)
{
    list lst = list_create();
    int i;
    for (i=1; i < list_size; i+=2)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size/2);
    i = 1;
    LIST_ITERATE(&lst, iter)
	int success;
        assert((int)list_get_data(iter) == i);
        success = list_insert_before(&iter, (void *)(i - 1));
	assert(success);
        assert((int)list_get_data(iter) == i);
        i += 2;
    LIST_ITERATE_END()
    assert (lst.size == list_size);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    list_destroy(&lst);
}

void test_insert_remove_end(int list_size)
{
    list lst = list_create();
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    for (i=0; i < list_size; i++)
    {
        assert(lst.size == list_size - i);
        assert((int)list_get_data(list_first(&lst)) == 0);
        assert((int)list_get_data(list_last(&lst)) == list_size-i-1);
        list_remove_end(&lst);
    }
    assert(lst.size == 0);
    list_destroy(&lst);
}

void test_insert_remove_beginning(int list_size)
{
    list lst = list_create();
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    for (i=0; i < list_size; i++)
    {
        assert(lst.size == list_size - i);
        assert((int)list_get_data(list_first(&lst)) == i);
        assert((int)list_get_data(list_last(&lst)) == list_size-1);
        list_remove_beginning(&lst);
    }
    assert(lst.size == 0);
    list_destroy(&lst);
}

void test_remove(int list_size, int modulus)
{
    list_iter iter;
    list lst = list_create();
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert(lst.size == list_size);
    i = 0;
    for (iter = list_first(&lst); !list_at_end(iter); )
    {
        assert((int)list_get_data(iter) == i);
	if ((i % modulus) == 0)
	{
	    list_next(&iter);
	}
	else
	{
	    list_remove(&iter);
	}
	i++;
    }
    assert(lst.size == (list_size + modulus - 1)/modulus);
    i = 0;
    LIST_ITERATE(&lst, iter)
        assert((int)list_get_data(iter) == i);
        i += modulus;
    LIST_ITERATE_END()
    list_destroy(&lst);
}

void test_backwards_iterate(int list_size)
{
    list lst = list_create();
    list_iter iter;
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size);
    i = list_size - 1;
    iter = list_last(&lst);
    while (1)
    {
        assert((int)list_get_data(iter) == i);
	if (list_at_beginning(iter))
	{
	    break;
	}
	list_prev(&iter);
        i--;
    }
    list_destroy(&lst);
}

void test_random_walk(int list_size, int num_steps)
{
    list lst = list_create();
    list_iter iter;
    int repeat;
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size);
    iter = list_first(&lst);
    for (i=0; i < list_size/2; i++)
    {
	list_next(&iter);
    }
    i = list_size/2;
    for (repeat=0; repeat < num_steps; repeat++)
    {
        assert((int)list_get_data(iter) == i);
	if (list_at_beginning(iter) ||
	    ((rand() % 2) && !list_at_end(iter)))
	{
	    list_next(&iter);
	    i++;
	}
	else
	{
	    list_prev(&iter);
	    i--;
	}
    }
    list_destroy(&lst);
}

void test_random_operations(int list_size, int num_operations)
{
    list lst = list_create();
    list_iter iter;
    int repeat;
    int i;
    for (i=0; i < list_size; i++)
    {
        list_insert_end(&lst, (void *)i);
    }
    assert (lst.size == list_size);
    iter = list_first(&lst);
    for (repeat=0; repeat < num_operations; repeat++)
    {
	/* No asserts - just rely on the implementation's invariant
	   checking and runtime faults. */
	int success;
        switch (rand() % 5)
	{
	case 0:
	    if (!list_at_end(iter))
	    {
		list_next(&iter);
	    }
	    break;
	case 1:
	    if (!list_at_beginning(iter))
	    {
		list_prev(&iter);
	    }
	    break;
	case 2:
	    success = list_insert_after(&iter, (void*)(rand()));
	    assert(success);
	    break;
	case 3:
	    success = list_insert_before(&iter, (void*)(rand()));
	    assert(success);
	    break;
	case 4:
	    list_remove(&iter);
	    break;
	default:
	    assert(0);
	}
    }
    list_destroy(&lst);
}

void test_swap(int size1, int size2)
{
    list lst1 = list_create();
    list lst2 = list_create();
    int i;
    for (i=0; i < size1; i++)
    {
        list_insert_end(&lst1, (void *)i);
    }
    for ( ; i < size1 + size2; i++)
    {
        list_insert_end(&lst2, (void *)i);
    }
    assert (lst1.size == size1);
    assert (lst2.size == size2);

    list_swap(&lst1, &lst2);
    assert (lst2.size == size1);
    assert (lst1.size == size2);
    i = 0;
    LIST_ITERATE(&lst2, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    LIST_ITERATE(&lst1, iter)
        assert((int)list_get_data(iter) == i);
        i++;
    LIST_ITERATE_END()
    assert (i == size1 + size2);
}


int main()
{
    test_create_destroy();
    test_insert_end_iterate(10000);
    test_insert_beginning_iterate(10000);
    test_insert_beginning_end_iterate(10000);
    test_insert_after_iterate(10000);
    test_insert_before_iterate(10000);
    test_remove(10000, 7);
    test_insert_remove_end(10000);
    test_insert_remove_beginning(10000);
    test_backwards_iterate(10000);
    test_random_walk(1000, 3000);
    test_random_operations(1000, 10000);
    test_swap(1000, 2000);
    return 0;
}
