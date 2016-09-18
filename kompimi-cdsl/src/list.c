/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <assert.h>

#include <malloc.h>
#include <string.h>

#include "list.h"

/*! \brief Splits a full node into two consecutive nodes, distributing
  its elements among them. */
static int split_node(list_iter* iter);

/*! \brief Merge a given node with its neighbors, if possible,
  to limit wasted space. */
static void rebalance_nodes(list_iter* iter);

/*! \brief Inserts a new node containing zero elements after the given node. */
static int insert_empty_node_after(list* lst, list_node* node);

/*! \brief Inserts a new node containing zero elements before the given node. */
static int insert_empty_node_before(list* lst, list_node* node);

/*! \brief Inserts a new empty node into an empty list with no nodes. */
static int insert_empty_sole_node(list* lst);

/*! \brief Removes a node from the linked list of nodes. */
static void remove_node(list* lst, list_node* node);

/*! \brief Verifies the current list data structure is valid and
    satisfies all algorithmic invariants. */
static void check_list_invariants(list* lst);

/*! \brief Verifies the list iterator data structure is valid and
    satisfies all algorithmic invariants. */
static void check_iter_invariants(list_iter* iter);

/* \brief Returns nonzero if and only if the given list contains the
   given node in its linked list of nodes. */
static int list_contains_node(list* lst, list_node* node_to_find);

/* \brief Fixes up an iterator with an invalid offset by moving it
   across nodes. */
static void fixup_iter_node(list_iter* iter);

list list_create(void)
{
    list result;
    result.size = 0;
    result.first_node = NULL;
    result.last_node = NULL;
    check_list_invariants(&result);
    return result;
}

void list_destroy(list* lst)
{
    list_node* node;
    list_node* next_node;
    for (node = lst->first_node; node != NULL; node = next_node)
    {
        next_node = node->next;
        free(node);
    }

    lst->size = 0;
    lst->first_node = NULL;
    lst->last_node = NULL;
}

int list_insert_after(list_iter* iter, void* value)
{
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
    assert (!list_at_end(*iter));
    if (iter->node->count == ELEMENTS_PER_LIST_NODE)
    {
        if (!split_node(iter))
        {
            return 0;
        }
    }
    {
        list_node* node = iter->node;
        memmove(node->data + iter->offset + 2,
                node->data + iter->offset + 1,
                (node->count - (iter->offset + 1)) * sizeof(void *));
        node->count++;
        iter->lst->size++;
        node->data[iter->offset + 1] = value;
    }
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
    return 1;
}

int list_insert_before(list_iter* iter, void* value)
{
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
    if (iter->node == NULL)
    {
        /* Insert before end iterator is insert at end */
        return list_insert_end(iter->lst, value);
    }
    if (iter->node->count == ELEMENTS_PER_LIST_NODE)
    {
        if (!split_node(iter))
        {
            return 0;
        }
    }
    {
        list_node* node = iter->node;
        memmove(node->data + iter->offset + 1,
                node->data + iter->offset,
                (node->count - iter->offset) * sizeof(void *));
        node->count++;
        iter->lst->size++;
        node->data[iter->offset] = value;
        iter->offset++;
    }
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
    return 1;
}

int list_insert_beginning(list* lst, void* value)
{
    if (lst->first_node == NULL)
    {
        if (!insert_empty_sole_node(lst))
        {
            return 0;
        }
    }
    if (lst->first_node->count == ELEMENTS_PER_LIST_NODE)
    {
        if (!insert_empty_node_before(lst, lst->first_node))
        {
            return 0;
        }
    }
    {
        list_node* node = lst->first_node;
        memmove(node->data + 1,
                node->data,
                node->count * sizeof(void *));
        node->count++;
        lst->size++;
        node->data[0] = value;
    }
    check_list_invariants(lst);
    return 1;
}

int list_insert_end(list* lst, void* value)
{
    if (lst->last_node == NULL)
    {
        if (!insert_empty_sole_node(lst))
        {
            return 0;
        }
    }
    if (lst->last_node->count == ELEMENTS_PER_LIST_NODE)
    {
        if (!insert_empty_node_after(lst, lst->last_node))
        {
            return 0;
        }
    }
    {
        list_node* node = lst->last_node;
        node->data[node->count] = value;
        node->count++;
        lst->size++;
    }
    check_list_invariants(lst);
    return 1;
}

void list_remove(list_iter* iter)
{
    list_node* node;
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
    assert (!list_at_end(*iter));
    node = iter->node;
    memmove(node->data + iter->offset,
            node->data + iter->offset + 1,
            (node->count - (iter->offset + 1)) * sizeof(void *));
    node->count--;
    iter->lst->size--;
    rebalance_nodes(iter);
    /* Need to fix up if deleted rightmost element in the node */
    fixup_iter_node(iter);
    check_list_invariants(iter->lst);
    check_iter_invariants(iter);
}

void list_remove_beginning(list* lst)
{
    list_node* node = lst->first_node;
    memmove(node->data,
	    node->data + 1,
	    (node->count - 1) * sizeof(void *));
    node->count--;
    lst->size--;
    if (node->count == 0)
    {
	remove_node(lst, node);
    }
    check_list_invariants(lst);
}

void list_remove_end(list* lst)
{
    lst->last_node->count--;
    lst->size--;
    if (lst->last_node->count == 0)
    {
	remove_node(lst, lst->last_node);
    }
    check_list_invariants(lst);
}

list_iter list_first(list* lst)
{
    list_iter result;
    result.lst = lst;
    result.node = lst->first_node; /* Okay if NULL */
    result.offset = 0;
    return result;
}

list_iter list_last(list* lst)
{
    list_iter result;
    result.lst = lst;
    result.node = lst->last_node; /* Okay if NULL */
    if (result.node != NULL)
    {
        result.offset = result.node->count - 1;
    }
    else
    {
        result.offset = 0;
    }
    return result;
}

void list_swap(list* lst1, list* lst2)
{
    /* Just use memberwise struct copy */
    list temp;
    temp = *lst1;
    *lst1 = *lst2;
    *lst2 = temp;

    check_list_invariants(lst1);
    check_list_invariants(lst2);
}

static int split_node(list_iter* iter)
{
    list_node* node = iter->node;
    assert(node->count == ELEMENTS_PER_LIST_NODE);
    if (!insert_empty_node_after(iter->lst, node))
    {
        return 0;
    }
    node->next->count = node->count - node->count/2;
    node->count = node->count/2;
    memcpy(node->next->data,
	   node->data + node->count,
           node->next->count * sizeof(void *));
    if (iter->offset >= node->count)
    {
        iter->node = node->next;
        iter->offset -= node->count;
    }
    return 1;
}

static void rebalance_nodes(list_iter* iter)
{
    list_node* node = iter->node;
    if (node->next == NULL || node->prev == NULL)
    {
        /* If it's the first or last node, it's allowed to be
           underfull, but not empty */
        if (node->count == 0)
        {
            fixup_iter_node(iter);
	    remove_node(iter->lst, node);
        }
        return;
    }

    /* Sum the number of elements in this node, its predecessor, and
       its successor. Fit the result into as few nodes as possible. */
    {
        int elements_sum;
        elements_sum = node->next->count + node->count + node->prev->count;
        if (elements_sum <= ELEMENTS_PER_LIST_NODE)
	{
            /* Merge into one node - can happen if prev/next is
	       first/last node */
	    memmove(node->data + node->prev->count,
		    node->data,
		    node->count * sizeof(void *));
	    memcpy(node->data,
		   node->prev->data,
		   node->prev->count * sizeof(void *));
	    memcpy(node->data + node->prev->count + node->count,
		   node->next->data,
		   node->next->count * sizeof(void *));
	    node->count = elements_sum;
	    iter->offset += node->prev->count;
            remove_node(iter->lst, node->prev);
            remove_node(iter->lst, node->next);
	}
        else if ((elements_sum + 1)/2 <= ELEMENTS_PER_LIST_NODE)
        {
            /* Merge into two nodes */
            int node1_count = (elements_sum + 0)/2;
            int node2_count = (elements_sum + 1)/2;
            if (node->prev->count < node1_count)
            {
                int move_1 = node1_count - node->prev->count;
                int move_2 = node->count - move_1;
                memcpy(node->prev->data + node->prev->count,
                       node->data,
                       move_1 * sizeof(void *));
                memmove(node->data,
                        node->data + move_1,
                        move_2 * sizeof(void *));
                memcpy(node->data + move_2,
                       node->next->data,
                       node->next->count * sizeof(void *));
		iter->offset -= move_1;
            }
            else
            {
                int move_1 = node->prev->count - node1_count;
                memmove(node->data + move_1,
                        node,
                        node->count * sizeof(void *));
                memcpy(node->data,
                       node->prev->data + node1_count,
                       move_1 * sizeof(void *));
                memcpy(node->data + node->count + move_1,
                       node->next->data,
                       node->next->count * sizeof(void *));
		iter->offset += move_1;
            }
            node->prev->count = node1_count;
            node->count = node2_count;
            remove_node(iter->lst, node->next);
        }
    }
}

static int insert_empty_node_after(list* lst, list_node* node)
{
    list_node* new_node = (list_node *)malloc(sizeof(list_node));
    if (new_node == NULL)
    {
        return 0;
    }
    new_node->count = 0;
    new_node->next = node->next;
    new_node->prev = node;
    node->next = new_node;
    if (new_node->next != NULL)
    {
        new_node->next->prev = new_node;
    }
    else
    {
        lst->last_node = new_node;
    }
    return 1;
}

static int insert_empty_node_before(list* lst, list_node* node)
{
    list_node* new_node = (list_node *)malloc(sizeof(list_node));
    if (new_node == NULL)
    {
        return 0;
    }
    new_node->count = 0;
    new_node->prev = node->prev;
    new_node->next = node;
    node->prev = new_node;
    if (new_node->prev != NULL)
    {
        new_node->prev->next = new_node;
    }
    else
    {
        lst->first_node = new_node;
    }
    return 1;
}

static int insert_empty_sole_node(list* lst)
{
    list_node* new_node = (list_node *)malloc(sizeof(list_node));
    if (new_node == NULL)
    {
        return 0;
    }
    new_node->count = 0;
    new_node->prev = NULL;
    new_node->next = NULL;
    lst->first_node = new_node;
    lst->last_node = new_node;
    return 1;
}

static void remove_node(list* lst, list_node* node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        lst->first_node = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        lst->last_node = node->prev;
    }
    free(node);
}

static void check_list_invariants(list* lst)
{
#ifndef NDEBUG
    list_node* node;
    int count_sum, num_nodes;
    assert(lst != NULL);
    assert(lst->size >= 0);
    assert((lst->size > 0 && lst->first_node != NULL && lst->last_node != NULL) ||
           (lst->size == 0 && lst->first_node == NULL && lst->last_node == NULL));

    count_sum = 0;
    num_nodes = 0;
    for (node = lst->first_node; node != NULL; node=node->next)
    {
        assert ((node == lst->first_node) || (node->prev != NULL));
        assert ((node == lst->last_node)  || (node->next != NULL));
        assert (node->count >= 1);
        assert (node->count <= ELEMENTS_PER_LIST_NODE);
        count_sum += node->count;
	num_nodes++;
    }
    assert (lst->size == count_sum);
    /* Rebalancing and splitting guarantees this, -2 is for first and
       last node, since these nodes can have as little as one element.
       Note that, rearranging, this guarantees that:

         num_nodes <= 2*lst->size/ELEMENTS_PER_LIST_NODE + 2
    */
    assert (count_sum >= (num_nodes - 2) * ((int)ELEMENTS_PER_LIST_NODE)/2);
#endif
}

static void check_iter_invariants(list_iter* iter)
{
#ifndef NDEBUG
    assert (iter->lst != NULL);
    assert ((iter->node == NULL) ||
            (iter->offset >= 0 && iter->offset < iter->node->count));
    assert (iter->node == NULL ||
	    list_contains_node(iter->lst, iter->node));
#endif
}

static int list_contains_node(list* lst, list_node* node_to_find)
{
    list_node* node;
    for (node = lst->first_node; node != NULL; node=node->next)
    {
        if (node == node_to_find)
        {
            return 1;
        }
    }
    return 0;
}

static void fixup_iter_node(list_iter* iter)
{
    while (iter->node != NULL &&
           iter->offset >= iter->node->count)
    {
        iter->offset -= iter->node->count;
        iter->node = iter->node->next;
    }
    while (iter->node != iter->lst->first_node &&
           iter->offset < 0)
    {
        iter->node = iter->node->prev;
        iter->offset += iter->node->count;
    }
}
