#ifndef LINKED_NODE_TYPE_H
#define LINKED_NODE_TYPE_H

#include <assert.h> /* asserts */
#include <stdlib.h> /* *alloc */

#include "attribute_macros.h"
#include "list_type_typedefs.h"

linked_node *node_pop(linked_node *const restrict node);
void *node_del(linked_node *const restrict node);
linked_node *node_new(
	void *data, dup_func *copy_data
) ATTR_MALLOC ATTR_MALLOC_FREE(node_del);

linked_node *node_get_next(const linked_node *const node) ATTR_NONNULL;
linked_node *node_get_prev(const linked_node *const node) ATTR_NONNULL;
void *node_get_data(const linked_node *const node) ATTR_NONNULL;
linked_node *node_set_next(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
) ATTR_NONNULL_IDX(1);
linked_node *node_set_prev(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
) ATTR_NONNULL_IDX(1);
void *
node_set_data(linked_node *const restrict node, void *const restrict data);

linked_node *node_insert_after(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
);
linked_node *node_insert_before(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
);
linked_node *node_swap(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
);

void *linked_list_del(linked_node *const head, free_func *free_data);

long int linked_list_print(
	FILE *restrict stream, linked_node const *const restrict head,
	print_func *print_data
);
long int linked_list_print_reversed(
	FILE *restrict stream, linked_node const *const restrict tail,
	print_func *print_data
);

#endif /* LINKED_NODE_TYPE_H */
