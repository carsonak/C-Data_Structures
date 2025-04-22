#ifndef LIST_STRUCTS_H
#define LIST_STRUCTS_H

#include <inttypes.h> /* intmax_t */

/**
 * struct linked_node - a singly linked list node.
 * @data: data that the node holds.
 * @next: pointer to the next node.
 * @previous: pointer to the previous node.
 */
struct linked_node
{
	void *data;
	struct linked_node *next;
	/*This second pointer makes insertions and deletions easier.*/
	struct linked_node *previous;
};

/**
 * struct deque - a deque data structure.
 * @length: number items in the deque.
 * @head: a pointer to the head of the deque.
 * @tail: a pointer to the tail of the deque.
 */
struct linked_list
{
	intmax_t length;
	struct linked_node *head;
	struct linked_node *tail;
};

#endif /* LIST_STRUCTS_H */
