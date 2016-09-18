/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include "../list.h"
#include "dllist.h"
#include "perf_test.h"

int main()
{
    int iteration_list_size = 1000000;

    {
        list lst = list_create();
        time_elapsed("insert_end_cdsl_list", 20000000,
            list_insert_end(&lst, (void *)0);
        );
        list_destroy(&lst);
    }

    {
        dllist dllst = dllist_create();
        time_elapsed("insert_end_dllist", 20000000,
            dllist_insert_end(&dllst, (void *)0);
        );
        dllist_destroy(&dllst);
    }
    
    {
        list lst = list_create();
        time_elapsed("insert_beginning_cdsl_list", 20000000,
            list_insert_beginning(&lst, (void *)0);
        );
        list_destroy(&lst);
    }

    {
        dllist dllst = dllist_create();
        time_elapsed("insert_beginning_dllist", 20000000,
            dllist_insert_beginning(&dllst, (void *)0);
        );
        dllist_destroy(&dllst);
    }
    
    {
        list lst = list_create();
	int i;
	for (i=0; i < 20000000; i++)
	{
	    list_insert_end(&lst, (void *)0);
	}
        time_elapsed("remove_end_cdsl_list", 20000000,
            list_remove_end(&lst);
        );
        list_destroy(&lst);
    }

    {
        dllist dllst = dllist_create();
	int i;
	for (i=0; i < 20000000; i++)
	{
            dllist_insert_end(&dllst, (void *)0);
	}
        time_elapsed("remove_end_dllist", 20000000,
            dllist_remove_end(&dllst);
        );
        dllist_destroy(&dllst);
    }
    
    {
        list lst = list_create();
	int i;
	for (i=0; i < 20000000; i++)
	{
	    list_insert_end(&lst, (void *)0);
	}
        time_elapsed("remove_beginning_cdsl_list", 20000000,
            list_remove_beginning(&lst);
        );
        list_destroy(&lst);
    }

    {
        dllist dllst = dllist_create();
	int i;
	for (i=0; i < 20000000; i++)
	{
            dllist_insert_end(&dllst, (void *)0);
	}
        time_elapsed("remove_beginning_dllist", 20000000,
            dllist_remove_beginning(&dllst);
        );
        dllist_destroy(&dllst);
    }
    
    {
	list_iter iter;
        list lst = list_create();
        list_insert_end(&lst, (void*)0);
        iter = list_first(&lst);
        time_elapsed("insert_middle_cdsl_list", 10000000,
            list_insert_after(&iter, (void *)0);
            list_insert_before(&iter, (void *)0);
        );
        list_destroy(&lst);
    }

    {
	dllist_node* iter;
        dllist dllst = dllist_create();
        dllist_insert_end(&dllst, (void*)0);
        iter = dllst.first_node;
        time_elapsed("insert_middle_dllist", 20000000,
            dllist_insert_after(&dllst, iter, (void *)0);
            dllist_insert_before(&dllst, iter, (void *)0);
        );
        dllist_destroy(&dllst);
    }
    
    {
        int i;
        list lst = list_create();
        for (i = 0; i < iteration_list_size; i++)
        {
            list_insert_end(&lst, (void *)i);
        }
        time_elapsed("iterate_cdsl_list", 250,
            int sum = 0;
            LIST_ITERATE(&lst, iter)
                sum += (int)list_get_data(iter);
            LIST_ITERATE_END()
        );
        list_destroy(&lst);
    }

    {
        int i;
        dllist dllst = dllist_create();
        for (i = 0; i < iteration_list_size; i++)
        {
            dllist_insert_end(&dllst, (void *)i);
        }
        time_elapsed("iterate_dllist", 250,
            int sum = 0;
            dllist_node* node;
            for (node = dllst.first_node; node != NULL; node=node->next)
            {
                sum += (int)node->data;
            }
        );
        dllist_destroy(&dllst);
    }

    return 0;
}
