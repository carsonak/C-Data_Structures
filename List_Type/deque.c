#include "deque.h"
#include "linked_list.h"
#include "list_structs.h"

/**
 * dq_new - allocate memory for an empty deque.
 *
 * Return: pointer to the new deque, NULL on failure.
 */
deque *dq_new(void) { return (calloc(1, sizeof(deque))); }

/**
 * dq_len - return total items in the deque.
 * @dq: pointer to the deque.
 *
 * Return: total items in the deque.
 */
size_t dq_len(deque const *const dq)
{
	if (!dq)
		return (0);

	return (dq->length);
}

/**
 * dq_peek_head - return data from the first node without deleting it.
 * @dq: the deque to operate on.
 *
 * Return: pointer to data from the first node.
 */
void *dq_peek_head(deque const *const dq)
{
	if (!dq)
		return (NULL);

	return (dln_get_data(dq->head));
}

/**
 * dq_peek_tail - return data from the last node without deleting it.
 * @dq: the node to operate on.
 *
 * Return: pointer to data from the last node.
 */
void *dq_peek_tail(deque const *const dq)
{
	if (!dq)
		return (NULL);

	return (dln_get_data(dq->tail));
}

/**
 * dq_push_tail - add a node to the end of a deque.
 * @dq: the deque to operate on.
 * @data: data that the node will hold.
 * @copy_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the newly added node, NULL if dq is NULL or failure.
 */
double_link_node *
dq_push_tail(deque *const dq, void *const data, dup_func *copy_data)
{
	double_link_node *nw = NULL;

	if (!dq)
		return (NULL);

	nw = dln_new(data, copy_data);
	if (!nw)
		return (NULL);

	dq->tail = dln_insert_after(dq->tail, nw);
	if (!dq->head)
		dq->head = nw;

	++(dq->length);
	return (nw);
}

/**
 * dq_pop_tail - pop a node from the tail of a deque.
 * @dq: the deque to operate on.
 *
 * Return: pointer to the data that was in the node.
 */
void *dq_pop_tail(deque *dq)
{
	if (!dq || !dq->tail)
		return (NULL);

	double_link_node *node = dq->tail;

	dq->tail = dln_get_prev(node);
	void *d = dln_remove(node);

	if (!dq->tail)
		dq->head = NULL;

	if (dq->length)
		--(dq->length);

	return (d);
}

/**
 * dq_push_head - add a new node to the head of the deque.
 * @dq: the deque to operate on.
 * @data: data that the new node will hold.
 * @copy_data: function that will be called to duplicate `data`.
 *
 * Return: pointer to the new node, NULL on failure.
 */
double_link_node *
dq_push_head(deque *const dq, void *const data, dup_func *copy_data)
{
	if (!dq)
		return (NULL);

	double_link_node *nw = dln_new(data, copy_data);

	if (!nw)
		return (NULL);

	dq->head = dln_insert_before(dq->head, nw);
	if (!dq->tail)
		dq->tail = nw;

	++(dq->length);
	return (nw);
}

/**
 * dq_pop_head - pop a node from the head of a deque and return its data.
 * @dq: the deque to operate on.
 *
 * Return: pointer to the data in the popped node, NULL if dq or head is NULL.
 */
void *dq_pop_head(deque *const dq)
{
	if (!dq || !dq->head)
		return (NULL);

	double_link_node *node = dq->head;

	dq->head = dln_get_next(node);
	void *d = dln_remove(node);

	if (!dq->head)
		dq->tail = NULL;

	if (dq->length)
		--(dq->length);

	return (d);
}

/**
 * clear_deque - delete a deque.
 * @dq: the deque to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
static void clear_deque(deque *const dq, delete_func *free_data)
{
	double_link_node *next_node = NULL;
	void *d = NULL;

	if (!dq || !dq->head)
		return;

	next_node = dln_get_next(dq->head);
	while (next_node)
	{
		d = dln_remove(dq->head);
		if (free_data)
			free_data(d);

		dq->head = next_node;
		next_node = dln_get_next(dq->head);
	}

	d = dln_remove(dq->head);
	if (free_data)
		free_data(d);

	dq->head = NULL;
	dq->tail = NULL;
	dq->length = 0;
}

/**
 * dq_delete - frees a deque from memory.
 * @nullable_ptr: pointer to the deque to delete.
 * @free_data: pointer to a function that can free data in the deque.
 *
 * Return: NULL always.
 */
void *dq_delete(deque *const nullable_ptr, delete_func *free_data)
{
	clear_deque(nullable_ptr, free_data);
	free(nullable_ptr);
	return (NULL);
}

/**
 * dq_from_array - create a new deque from an array of objects.
 * @data_array: the array of objects.
 * @len: number of items in the array.
 * @type_size: size of the type in the array.
 * @copy_data: function that will be used to copy the objects.
 * @delete_data: function that will be used to delete objects.
 *
 * Return: pointer to the new deque, NULL on failure.
 */
deque *dq_from_array(
	void *const data_array, const size_t len, const size_t type_size,
	dup_func *copy_data, delete_func *delete_data
)
{
	deque *new_q = NULL;

	if (!data_array || len == 0 || type_size == 0)
		return (NULL);

	/*Avoid memory leaks by rejecting imbalanced allocation deallocation.*/
	if (copy_data && !delete_data)
		return (NULL);

	new_q = dq_new();
	if (!new_q)
		return (NULL);

	for (size_t i = 0; i < len; ++i)
	{
		void *data = (char *)data_array + (type_size * i);

		if (!dq_push_tail(new_q, data, copy_data))
		{
			new_q = dq_delete(new_q, delete_data);
			break;
		}
	}

	return (new_q);
}

/**
 * dq_to_array - create an array from a deque.
 * @q: the deque.
 * @copy_data: optional pointer to a function that will be used to duplicate
 * the data, if not provided, array will contain pointers to the original data
 * in the deque.
 * @free_data: optional pointer to a function that will be used to free data in
 * the array in case of failure. If `copy_data` is provided this function must
 * also be provided, otherwise no data duplication will occur.
 *
 * Return: pointer to the data array on success, NULL on failure.
 */
void **
dq_to_array(const deque *const q, dup_func *copy_data, delete_func *free_data)
{
	void **data_array = NULL;
	double_link_node *node = NULL;
	size_t d_i = 0;

	if (!q || !q->head || q->length < 1)
		return (NULL);

	data_array = calloc(q->length + 1, sizeof(*data_array));
	if (!data_array)
		return (NULL);

	for (node = q->head, d_i = 0; node; node = dln_get_next(node), ++d_i)
	{
		void *data = dln_get_data(node);

		data_array[d_i] = data;
		if (copy_data && free_data)
		{
			data_array[d_i] = copy_data(data);
			if (!data_array[d_i] && data)
				return (delete_2D_array(data_array, q->length, free_data));
		}
	}

	return (data_array);
}

/**
 * dq_print - print all nodes of a deque.
 * @stream: pointer to the stream to print to.
 * @dq: the deque to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
 */
long int dq_print(FILE *stream, deque const *const dq, print_func *print_data)
{
	if (!stream || !dq)
		return (-1);

	if (!dq->head)
		return (fprintf(stream, "(NULL)\n"));

	return (dll_print(stream, dq->head, print_data));
}

/**
 * dq_print_reversed - print all nodes of a deque from tail to head.
 * @stream: pointer to the stream to print to.
 * @dq: the deque to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
 */
long int
dq_print_reversed(FILE *stream, deque const *const dq, print_func *print_data)
{
	if (!stream || !dq)
		return (-1);

	if (!dq->tail)
		return (fprintf(stream, "(NULL)\n"));

	return (dll_print_reversed(stream, dq->tail, print_data));
}
