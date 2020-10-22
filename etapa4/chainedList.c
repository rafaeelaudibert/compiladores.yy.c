#include <stdlib.h>
#include <stdio.h>
#include "chainedList.h"

/// Creates a ChainedList node, pointing to NULL, and a val as `val`
///
/// @param val A pointer to void value, which this ChainedList will point to
///
/// @returns A new pointer to a ChainedList node
ChainedList *create_chained_list(void *val)
{
    ChainedList *cl = (ChainedList *)malloc(sizeof(ChainedList));
    cl->next = NULL;
    cl->val = (void *)val;

    return cl;
}

/// Reverts a chainedList, this is:
///     if we have a list a -> b -> c -> NULL, we will have a list c -> b -> a -> NULL
/// Important to notice that this happens inplace
///
/// @param list A ChainedList which will be reverted. This action will destroy the list
///
/// @returns A new pointer to a ChainedList node
ChainedList *revert_chained_list(ChainedList *list)
{
    ChainedList *next = NULL, *before = NULL, *curr = list;
    while (curr)
    {
        next = curr->next;
        curr->next = before;

        before = curr;
        curr = next;
    }

    return before;
}

/// Frees entirely a ChainedList*, including the val inside of it
///
/// @param list A ChainedList* to be freed
void free_chained_list(ChainedList *list)
{
    ChainedList *next = NULL;
    while (list)
    {
        next = list->next;
        free(list->val);
        free(list);
        list = next;
    }
}

/// Prints a chained list
///
/// @param list A ChainedList* to be printed
void print_chained_list(ChainedList *list)
{
    printf("[");
    while (list)
    {
        printf((char *)list->val);
        if (list->next)
        {
            printf(", ");
        }
        list = list->next;
    }
    printf("]\n");
}