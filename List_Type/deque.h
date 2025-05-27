#ifndef DS_DEQUE_TYPE_H
#define DS_DEQUE_TYPE_H

#include <stdint.h> /* intmax_t */

#include "attribute_macros.h"
#include "list_type_typedefs.h"

/* alloc and free */

void *dq_del(deque *const restrict dq, free_func *free_data);
deque *dq_new(void) ATTR_MALLOC ATTR_MALLOC_FREE(dq_del);

/* manipulate */

list_node *
dq_push_head(deque *const restrict dq, void *const data, dup_func *copy_data);
list_node *dq_push_tail(
	deque *const restrict dq, void *const restrict data, dup_func *copy_data
);
void *dq_pop_head(deque *const restrict dq);
void *dq_pop_tail(deque *const restrict dq);
void dq_clear(deque *const restrict dq, free_func *free_data);

/* array conversion */

deque *dq_from_array(
	void *const restrict data_array, const intmax_t len,
	const size_t type_size, dup_func *copy_data, free_func *delete_data
);
void **dq_to_array(
	const deque *const restrict dq, dup_func *copy_data, free_func *free_data
);

/* print */

char *dq_tostr(deque const *const restrict dq, data_tostr *print_data);
char *
dq_tostr_reversed(deque const *const restrict dq, data_tostr *print_data);

#endif /* DS_DEQUE_TYPE_H */
