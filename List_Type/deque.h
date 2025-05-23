#ifndef LIST_LIST_TYPE_H
#define LIST_LIST_TYPE_H

#include <assert.h> /* asserts */
#include <stdint.h> /* intmax_t */
#include <stdlib.h> /* *alloc */

#include "attribute_macros.h"
#include "list_typedefs.h"

struct deque
{
	intmax_t len;
	linked_node *head;
	linked_node *tail;
};

void *dq_del(deque *const dq, free_func *free_data);
deque *dq_new(void) ATTR_MALLOC ATTR_MALLOC_FREE(dq_del);

linked_node *
dq_push_head(deque *const restrict dq, void *const data, dup_func *copy_data);
linked_node *dq_push_tail(
	deque *const restrict dq, void *const restrict data, dup_func *copy_data
);
void *dq_pop_head(deque *const restrict dq);
void *dq_pop_tail(deque *const restrict dq);
void dq_clear(deque *const restrict dq, free_func *free_data);
deque *dq_from_array(
	void *const restrict data_array, const intmax_t len,
	const size_t type_size, dup_func *copy_data, free_func *delete_data
);
void **
dq_to_array(const deque *const dq, dup_func *copy_data, free_func *free_data);
long int dq_print(FILE *stream, deque const *const dq, print_func *print_data);
long int
dq_print_reversed(FILE *stream, deque const *const dq, print_func *print_data);

#endif /* LIST_LIST_TYPE_H */
