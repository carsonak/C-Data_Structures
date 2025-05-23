#ifndef LIST_TYPE_STRUCTS_H
#define LIST_TYPE_STRUCTS_H

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
	linked_node *head;
	linked_node *tail;
};

/**
 * struct linked_node - a doubly linked node.
 * @next: pointer to the next node.
 * @prev: pointer to the previous node.
 * @data: pointer to the data for the node.
 */
struct linked_node
{
	struct linked_node *restrict next;
	struct linked_node *restrict prev;
	void *data;
};

#endif /* LIST_TYPE_STRUCTS_H */
