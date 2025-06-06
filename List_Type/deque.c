#include <string.h> /* strcpy */

#include "deque.h"
#include "list_node.h"
#include "list_type_structs.h"
#include "matrix.h"

/**
 * dq_new - allocate and initialise memory for a `deque`.
 *
 * Return: pointer to the new list.
 */
deque *dq_new(void) { return (calloc(1, sizeof(deque))); }

/**
 * dq_clear - free all the nodes of a `deque`.
 * @dq: the `deque` to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
void dq_clear(deque *const restrict dq, free_func *free_data)
{
	if (!dq || !dq->head)
		return;

	list_node *next_node = lstnode_get_next(dq->head);

	while (dq->head)
	{
		void *d = lstnode_del(dq->head);

		if (free_data)
			free_data(d);

		dq->head = next_node;
		next_node = dq->head ? lstnode_get_next(dq->head) : NULL;
	}

	dq->head = NULL;
	dq->tail = NULL;
	dq->len = 0;
}

/**
 * dq_del - frees a `deque` from memory.
 * @dq: pointer to the `deque` to delete.
 * @free_data: pointer to a function that can free data in the `deque`.
 *
 * Return: NULL always.
 */
void *dq_del(deque *const restrict dq, free_func *free_data)
{
	dq_clear(dq, free_data);
	free(dq);
	return (NULL);
}

/**
 * dq_push_head - add a new node to the head of the `deque`.
 * @dq: the `deque` to operate on.
 * @data: data that the new node will hold.
 * @copy_data: function that will be called to duplicate `data`.
 *
 * Return: pointer to the new node, NULL on failure.
 */
list_node *
dq_push_head(deque *const restrict dq, void *const data, dup_func *copy_data)
{
	if (!dq)
		return (NULL);

	list_node *const restrict nw = lstnode_new(data, copy_data);

	if (!nw)
		return (NULL);

	dq->head = lstnode_insert_before(dq->head, nw);
	if (!dq->tail)
		dq->tail = nw;

	++(dq->len);
	return (nw);
}

/**
 * dq_push_tail - add a node to the end of a `deque`.
 * @dq: the `deque` to operate on.
 * @data: data that the node will hold.
 * @copy_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the newly added node, NULL if dq is NULL or failure.
 */
list_node *dq_push_tail(
	deque *const restrict dq, void *const restrict data, dup_func *copy_data
)
{
	if (!dq)
		return (NULL);

	list_node *const restrict nw = lstnode_new(data, copy_data);
	if (!nw)
		return (NULL);

	dq->tail = lstnode_insert_after(dq->tail, nw);
	if (!dq->head)
		dq->head = nw;

	++(dq->len);
	return (nw);
}

/**
 * dq_pop_head - pop a node from the head of a `deque` and return its data.
 * @dq: the `deque` to operate on.
 *
 * Return: pointer to the data in the popped node, NULL if dq or head is NULL.
 */
void *dq_pop_head(deque *const restrict dq)
{
	if (!dq || !dq->head)
		return (NULL);

	list_node *const node = dq->head;

	dq->head = lstnode_get_next(node);
	void *const d = lstnode_del(node);

	if (!dq->head)
		dq->tail = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/**
 * dq_pop_tail - pop a node from the tail of a `deque`.
 * @dq: the `deque` to operate on.
 *
 * Return: pointer to the data that was in the node.
 */
void *dq_pop_tail(deque *const restrict dq)
{
	if (!dq || !dq->tail)
		return (NULL);

	list_node *node = dq->tail;

	dq->tail = lstnode_get_prev(node);
	void *const d = lstnode_del(node);

	if (!dq->tail)
		dq->head = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/**
 * dq_from_array - create a new `deque` from an array of objects.
 * @data_array: the array of objects.
 * @len: number of items in the array.
 * @type_size: size of the type in the array.
 * @copy_data: function that will be used to copy the objects.
 * @delete_data: function that will be used to delete objects.
 *
 * Return: pointer to the new `deque`, NULL on failure.
 */
deque *dq_from_array(
	void *const restrict data_array, const intmax_t len,
	const size_t type_size, dup_func *copy_data, free_func *delete_data
)
{
	if (!data_array || len == 0 || type_size == 0)
		return (NULL);

	/* Avoid memory leaks by rejecting imbalanced allocation deallocation. */
	if (copy_data && !delete_data)
		return (NULL);

	deque *restrict new_q = dq_new();

	if (!new_q)
		return (NULL);

	for (intmax_t i = 0; i < len; ++i)
	{
		void *data = (char *)data_array + (type_size * i);

		if (!dq_push_tail(new_q, data, copy_data))
		{
			new_q = dq_del(new_q, delete_data);
			break;
		}
	}

	return (new_q);
}

/**
 * dq_to_array - create an array from a `deque`.
 * @dq: the `deque`.
 * @copy_data: optional pointer to a function that will be used to duplicate
 * the data, if not provided, array will contain pointers to the original data
 * in the `deque`.
 * @free_data: optional pointer to a function that will be used to free data in
 * the array in case of failure. If `copy_data` is provided this function must
 * also be provided, otherwise no data duplication will occur.
 *
 * Return: pointer to the data array on success, NULL on failure.
 */
void **dq_to_array(
	const deque *const restrict dq, dup_func *copy_data, free_func *free_data
)
{
	if (!dq || !dq->head || dq->len < 1)
		return (NULL);

	list_node *node = NULL;
	size_t d_i = 0;
	void **const restrict data_array =
		calloc(dq->len + 1, sizeof(*data_array));

	if (!data_array)
		return (NULL);

	for (node = dq->head, d_i = 0; node; node = lstnode_get_next(node), ++d_i)
	{
		void *data = lstnode_get_data(node);

		data_array[d_i] = data;
		if (copy_data && free_data)
		{
			data_array[d_i] = copy_data(data);
			if (!data_array[d_i] && data)
				return (delete_2D_array(data_array, dq->len, free_data));
		}
	}

	return (data_array);
}

/**
 * dq_tostr - stringify a `deque`.
 * @dq: the `deque` to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
 */
char *dq_tostr(deque const *const restrict dq, data_tostr *print_data)
{
	if (!dq)
		return (NULL);

	if (!dq->head)
	{
		char *const restrict s = malloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (linked_list_tostr(dq->head, print_data));
}

/**
 * dq_tostr_reversed - stringify a `deque` from tail to head.
 * @dq: the `deque` to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
 */
char *dq_tostr_reversed(deque const *const restrict dq, data_tostr *print_data)
{
	if (!dq)
		return (NULL);

	if (!dq->tail)
	{
		char *const restrict s = malloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (linked_list_tostr_reversed(dq->tail, print_data));
}
