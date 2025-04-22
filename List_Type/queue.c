#include "queue.h"
#include "linked_list.h"
#include "list_structs.h"

/**
 * queue_new - allocate memory for an empty queue.
 *
 * Return: pointer to the new queue, NULL on failure.
 */
queue *queue_new(void) { return (calloc(1, sizeof(queue))); }

/**
 * queue_len - return total items in the queue.
 * @q: pointer to the queue.
 *
 * Return: total items in the queue.
 */
size_t queue_len(queue const *const q)
{
	if (!q)
		return (0);

	return (q->length);
}

/**
 * enqueue - add a node to the end of a queue.
 * @q: the queue to operate on.
 * @data: data that the node will hold.
 * @copy_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the newly added node, NULL if q is NULL or failure.
 */
single_link_node *
enqueue(queue *const q, void *const data, dup_func *copy_data)
{
	if (!q)
		return (NULL);

	single_link_node *const nw = sln_new(data, copy_data);

	if (!nw)
		return (NULL);

	q->tail = sln_insert_after(q->tail, nw);
	if (!q->head)
		q->head = nw;

	++(q->length);
	return (nw);
}

/**
 * dequeue - pop a node from the start of a queue and return its data.
 * @q: the queue to operate on.
 *
 * Return: pointer to the data in the popped node, NULL if q or head is NULL.
 */
void *dequeue(queue *const q)
{
	if (!q || !q->head)
		return (NULL);

	single_link_node *const node = q->head;

	q->head = sln_get_next(node);
	void *const d = sln_remove(node);

	if (!q->head)
		q->tail = NULL;

	if (q->length)
		--(q->length);

	return (d);
}

/**
 * queue_peek_first - return data from the first node without deleting it.
 * @q: the queue to operate on.
 *
 * Return: pointer to data from the first node.
 */
void *queue_peek_first(queue const *const q)
{
	if (!q)
		return (NULL);

	return (sln_get_data(q->head));
}

/**
 * queue_peek_last - return data from the last node without deleting it.
 * @q: the node to operate on.
 *
 * Return: pointer to data from the last node.
 */
void *queue_peek_last(queue const *const q)
{
	if (!q)
		return (NULL);

	return (sln_get_data(q->tail));
}

/**
 * clear_queue - delete a queue.
 * @q: the queue to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
static void clear_queue(queue *const q, delete_func *free_data)
{
	if (!q)
		return;

	q->head = sll_clear(q->head, free_data);
	q->tail = NULL;
	q->length = 0;
}

/**
 * queue_delete - frees a queue from memory.
 * @nullable_ptr: pointer to the queue to delete.
 * @free_data: pointer to a function that can free data in the queue.
 *
 * Return: NULL always.
 */
void *queue_delete(queue *const nullable_ptr, delete_func *free_data)
{
	clear_queue(nullable_ptr, free_data);
	free(nullable_ptr);
	return (NULL);
}

/**
 * queue_from_array - create a new queue from an array of objects.
 * @data_array: the array of objects.
 * @len: number of items in the array.
 * @type_size: size of the type in the array.
 * @copy_data: pointer to a function that will be used to copy the objects.
 * @delete_data: pointer to a function that will be used to delete objects.
 * This function must be provided if `copy_data` is provided, otherwise leaks
 * may occur when function returns early.
 *
 * Return: pointer to the new queue, NULL on failure.
 */
queue *queue_from_array(
	void *const data_array, const size_t len, const size_t type_size,
	dup_func *copy_data, delete_func *delete_data
)
{
	if (!data_array || len == 0 || type_size == 0)
		return (NULL);

	/*Avoid memory leaks by rejecting imbalanced allocation deallocation.*/
	if (copy_data && !delete_data)
		return (NULL);

	queue *new_q = queue_new();

	if (!new_q)
		return (NULL);

	for (size_t i = 0; i < len; ++i)
	{
		void *data = (char *)data_array + (type_size * i);

		if (!enqueue(new_q, data, copy_data))
		{
			new_q = queue_delete(new_q, delete_data);
			break;
		}
	}

	return (new_q);
}

/**
 * queue_to_array - create an array from a queue.
 * @q: the queue.
 * @copy_data: optional pointer to a function that will be used to duplicate
 * the data, if not provided, array will contain pointers to the original data
 * in the queue.
 * @free_data: optional pointer to a function that will be used to free data in
 * the array in case of failure. If `copy_data` is provided this function must
 * also be provided, otherwise no data duplication will occur.
 *
 * Return: pointer to the data array on success, NULL on failure.
 */
void **queue_to_array(
	const queue *const q, dup_func *copy_data, delete_func *free_data
)
{
	void **data_array = NULL;
	single_link_node *node = NULL;
	size_t d_i = 0;

	if (!q || !q->head || q->length < 1)
		return (NULL);

	data_array = calloc(q->length + 1, sizeof(*data_array));
	if (!data_array)
		return (NULL);

	for (node = q->head, d_i = 0; node; node = sln_get_next(node), ++d_i)
	{
		void *data = sln_get_data(node);

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
 * queue_print - print all nodes of a queue.
 * @stream: pointer to the stream to print to.
 * @q: the queue to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int
queue_print(FILE *stream, queue const *const q, print_func *print_data)
{
	if (!stream || !q)
		return (-1);

	if (!q->head)
		return (fprintf(stream, "(NULL)\n"));

	return (sll_print(stream, q->head, print_data));
}
