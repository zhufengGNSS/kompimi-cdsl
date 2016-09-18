/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

/* A very simple doubly-linked list, to act as a baseline for
   performance comparisons against the more complex official list
   class. */

#ifndef _DLLIST_
#define _DLLIST_

typedef struct dllist_node_type
{
    void* data;
    struct dllist_node_type* next;
    struct dllist_node_type* prev;
} dllist_node;

typedef struct
{
    dllist_node* first_node;
    dllist_node* last_node;
} dllist;

dllist dllist_create();
void dllist_insert_beginning(dllist* lst, void* data);
void dllist_insert_end(dllist* lst, void* data);
void dllist_remove_end(dllist* lst);
void dllist_remove_beginning(dllist* lst);
void dllist_insert_before(dllist* lst, dllist_node* iter, void* data);
void dllist_insert_after(dllist* lst, dllist_node* iter, void* data);
void dllist_destroy(dllist *lst);

#endif /* #if _DLLIST_ */
