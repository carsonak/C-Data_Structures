#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h> /* size_t */
#include <stdio.h>  /* *printf */
#include <stdlib.h> /* *alloc */

#include "list_types.h"

void *delete_2D_array(void **array, size_t size, delete_func *free_row);

queue *queue_new(void);
void *queue_delete(queue *const nullable_ptr, delete_func *free_data);
size_t queue_len(queue const *const q);
single_link_node *
enqueue(queue *const q, void *const data, dup_func *copy_data);
void *dequeue(queue *const q);
void *queue_peek_first(queue const *const q);
void *queue_peek_last(queue const *const q);
queue *queue_from_array(
	void *const data_array, const size_t len, const size_t type_size,
	dup_func *copy_data, delete_func *delete_data
);
void **queue_to_array(
	const queue *const q, dup_func *copy_data, delete_func *free_data
);
long int
queue_print(FILE *stream, queue const *const q, print_func *print_data);

#endif /* QUEUE_H */
