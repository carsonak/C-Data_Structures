#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h> /* size_t */
#include <stdio.h>  /* *printf */
#include <stdlib.h> /* *alloc */

#include "list_types.h"

void *delete_2D_array(void **array, size_t size, delete_func *free_row);

deque *dq_new(void);
size_t dq_len(deque const *const dq);
void *dq_peek_head(deque const *const dq);
void *dq_peek_tail(deque const *const dq);
double_link_node *
dq_push_head(deque *const dq, void *const data, dup_func *copy_data);
void *dq_pop_head(deque *const dq);
double_link_node *
dq_push_tail(deque *const dq, void *const data, dup_func *copy_data);
void *dq_pop_tail(deque *dq);
void *dq_delete(deque *const nullable_ptr, delete_func *free_data);
long int dq_print(FILE *stream, deque const *const dq, print_func *print_data);
long int
dq_print_reversed(FILE *stream, deque const *const dq, print_func *print_data);
deque *dq_from_array(
	void *const data_array, const size_t len, const size_t type_size,
	dup_func *copy_data, delete_func *delete_data
);

#endif /* DEQUE_H */
