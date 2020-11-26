#ifndef CHAINED_LIST_H
#define CHAINED_LIST_H

typedef struct ChainedList
{
    struct ChainedList *next;
    void *val;
} ChainedList;

ChainedList *create_chained_list(void *val);
ChainedList *revert_chained_list(ChainedList *list);
ChainedList *append_end(ChainedList *to, ChainedList *from);
void free_chained_list(ChainedList *list);
void print_chained_list(ChainedList *list);

#endif
