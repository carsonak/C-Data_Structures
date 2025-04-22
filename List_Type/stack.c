#include "stack.h"
#include "linked_list.h"
#include "list_structs.h"

/**
 * stk_new - allocates space for an empty stack.
 *
 * Return: pointer to the new stack, NULL on failure.
 */
stack *stk_new(void) { return (calloc(1, sizeof(stack))); }

/**
 * stk_len - return number of nodes in the stack.
 * @s: the stack to operate on.
 *
 * Return: number of nodes in the stack.
 */
size_t stk_len(stack const *const s)
{
	if (!s)
		return (0);

	return (s->length);
}

/**
 * stk_peek - return data at the top of the stack without deleting the node.
 * @s: the stack to operate on.
 *
 * Return: pointer to data at the top of the stack.
 */
void *stk_peek(stack const *const s)
{
	if (!s)
		return (NULL);

	return (sln_get_data(s->head));
}

/**
 * stk_push - push an item onto a.
 * @s: the stack to operate on.
 * @data: data that will be stored in the node.
 * @copy_data: function that will be used to duplicate the data.
 *
 * Return: pointer to the newly added node, NULL if s is NULL or failure.
 */
single_link_node *
stk_push(stack *const s, void *const data, dup_func *copy_data)
{
	if (!s)
		return (NULL);

	single_link_node *new_top = sln_new(data, copy_data);

	if (!new_top)
		return (NULL);

	s->head = sln_insert_before(s->head, new_top);
	++(s->length);
	return (new_top);
}

/**
 * stk_pop - pop the top node on the stack and returns the data.
 * @s: the stack to operate on.
 *
 * Return: pointer to the data that was in the top node, NULL if s is NULL.
 */
void *stk_pop(stack *const s)
{
	if (!s || !s->head)
		return (NULL);

	single_link_node *old_top = s->head;

	s->head = sln_get_next(old_top);
	void *data = sln_remove(old_top);

	if (s->length)
		--(s->length);

	return (data);
}

/**
 * clear_stack - delete a stack.
 * @s: the stack to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
static void clear_stack(stack *const s, delete_func *free_data)
{
	if (!s || !s->head)
		return;

	s->head = sll_clear(s->head, free_data);
	s->length = 0;
}

/**
 * stk_delete - delete a stack.
 * @nullable_ptr: pointer to the stack to delete.
 * @free_data: pointer to a function that can free data in the queue.
 *
 * Return: NULL always.
 */
void *stk_delete(stack *const nullable_ptr, delete_func *free_data)
{
	clear_stack(nullable_ptr, free_data);
	free(nullable_ptr);
	return (NULL);
}

/**
 * stk_print - print all nodes of a stack.
 * @stream: pointer to stream to write to.
 * @s: the stack to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, a negative number on error.
 */
long int stk_print(FILE *stream, stack const *const s, print_func *print_data)
{
	if (!stream || !s)
		return (-1);

	if (!s->head)
		return (fprintf(stream, "(NULL)\n"));

	single_link_node *walk = s->head;
	long int total_bytes = 0;
	int bytes_printed = 0;
	while (walk)
	{
		bytes_printed = fprintf(stream, "+ ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		void *d = sln_get_data(walk);

		if (print_data)
			bytes_printed = print_data(stream, d);
		else
			bytes_printed = fprintf(stream, "%p", d);

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		fprintf(stream, "\n++++++++++++\n");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = sln_get_next(walk);
	}

	return (total_bytes);
}
