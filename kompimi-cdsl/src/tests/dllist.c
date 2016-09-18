/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <stdlib.h>

#include "dllist.h"

dllist dllist_create()
{
    dllist result;
    result.first_node = NULL;
    result.last_node = NULL;
    return result;
}

void dllist_insert_beginning(dllist* lst, void* data)
{
    dllist_node* new_node = (dllist_node *)malloc(sizeof(dllist_node));
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = lst->first_node;
    lst->first_node = new_node;
    if (new_node->next != NULL)
    {
	new_node->next->prev = new_node;
    }
    else
    {
	lst->last_node = new_node;
    }
}

void dllist_insert_end(dllist* lst, void* data)
{
    dllist_node* new_node = (dllist_node *)malloc(sizeof(dllist_node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = lst->last_node;
    lst->last_node = new_node;
    if (new_node->prev != NULL)
    {
	new_node->prev->next = new_node;
    }
    else
    {
	lst->first_node = new_node;
    }
}

void dllist_remove_end(dllist* lst)
{
    dllist_node* node = lst->last_node;
    lst->last_node = node->prev;
    if (node->prev != NULL)
    {
	node->prev->next = NULL;
    }
    else
    {
	lst->first_node = NULL;
    }
    free(node);
}

void dllist_remove_beginning(dllist* lst)
{
    dllist_node* node = lst->first_node;
    lst->first_node = node->next;
    if (node->next != NULL)
    {
	node->next->prev = NULL;
    }
    else
    {
	lst->first_node = NULL;
    }
    free(node);
}

void dllist_insert_before(dllist* lst, dllist_node* iter, void* data)
{
    dllist_node* new_node = (dllist_node *)malloc(sizeof(dllist_node));
    new_node->data = data;
    new_node->prev = iter->prev;
    new_node->next = iter;
    new_node->next->prev = new_node;
    if (new_node->prev != NULL)
    {
	new_node->prev->next = new_node;
    }
    else
    {
	lst->first_node = new_node;
    }
}

void dllist_insert_after(dllist* lst, dllist_node* iter, void* data)
{
    dllist_node* new_node = (dllist_node *)malloc(sizeof(dllist_node));
    new_node->data = data;
    new_node->next = iter->next;
    new_node->prev = iter;
    new_node->prev->next = new_node;
    if (new_node->next != NULL)
    {
	new_node->next->prev = new_node;
    }
    else
    {
	lst->last_node = new_node;
    }
}

void dllist_destroy(dllist *lst)
{
    dllist_node* node;
    dllist_node* next_node;
    for (node = lst->first_node; node != NULL; node=next_node)
    {
	next_node = node->next;
	free(node);
    }
}
