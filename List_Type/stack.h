#ifndef STACK_H
#define STACK_H

#include <stddef.h> /* size_t */
#include <stdio.h>  /* *printf */
#include <stdlib.h> /* *alloc */

#include "list_types.h"

stack *stk_new(void);
size_t stk_len(stack const *const s);
void *stk_peek(stack const *const s);
single_link_node *
stk_push(stack *const s, void *const data, dup_func *copy_data);
void *stk_pop(stack *const s);
void *stk_delete(stack *const nullable_ptr, delete_func *free_data);
long int stk_print(FILE *stream, stack const *const s, print_func *print_data);

#endif /* STACK_H */
