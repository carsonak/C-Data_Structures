#ifndef DS_LIST_TYPE_STRUCTS_H
#define DS_LIST_TYPE_STRUCTS_H

#include <stdint.h> /* intmax_t */

#include "list_type_typedefs.h"

/**
 * struct deque - a `deque` type.
 * @len: number of nodes in the deque.
 * @head: pointer to the head node of the deque.
 * @tail: pointer to the tail node of the deque.
 */
struct deque
{
	intmax_t len;
	list_node *head;
	list_node *tail;
};

/**
 * struct list_node - a doubly linked node.
 * @next: pointer to the next node.
 * @prev: pointer to the previous node.
 * @data: pointer to the data for the node.
 */
struct list_node
{
	struct list_node *restrict next;
	struct list_node *restrict prev;
	void *data;
};

#endif /* DS_LIST_TYPE_STRUCTS_H */
