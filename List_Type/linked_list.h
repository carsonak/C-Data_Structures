#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>  /* *printf */
#include <stdlib.h> /* *alloc */

#include "list_types.h"

linked_node *ln_new(void *const data, dup_func *duplicate_data);
linked_node *
ln_insert_after(linked_node *const this_node, linked_node *const other_node);
linked_node *
ln_insert_before(linked_node *const this_node, linked_node *other_node);
void *ln_remove(linked_node *node);
void *ln_get_data(linked_node const *const node);
void *
ln_swap_data(linked_node *const node, void *const data, dup_func *copy_data);
linked_node *ln_get_next(linked_node const *const node);
linked_node *ln_get_prev(linked_node const *const node);
void *ll_clear(linked_node *const head, delete_func *free_data);
long int sll_print(
	FILE *stream, single_link_node const *const head, print_func *print_data
);
long int dll_print(
	FILE *stream, double_link_node const *const head, print_func *print_data
);
long int dll_print_reversed(
	FILE *stream, double_link_node const *const tail, print_func *print_data
);

#endif /* LINKED_LIST_H */
