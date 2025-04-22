#include "linked_list.h"
#include "list_structs.h"

/**
 * ln_new - allocates memory for a linked node and initialises it.
 * @data: the object to initialise with.
 * @duplicate_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the node, NULL on failure.
 */
linked_node *ln_new(void *const data, dup_func *duplicate_data)
{
	linked_node *new_node = calloc(1, sizeof(*new_node));

	if (!new_node)
		return (NULL);

	new_node->data = data;
	if (duplicate_data)
	{
		new_node->data = duplicate_data(data);
		if (!new_node->data && data)
		{
			free(new_node);
			return (NULL);
		}
	}

	return (new_node);
}

/**
 * ln_insert_after - inserts a linked node after another.
 * @this_node: the node to insert after.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
linked_node *
ln_insert_after(linked_node *const this_node, linked_node *const other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->previous = this_node;
	other_node->next = this_node->next;
	if (this_node->next)
		this_node->next->previous = other_node;

	this_node->next = other_node;
	return (other_node);
}

/**
 * ln_insert_before - insert a linked node before another.
 * @this_node: the node to insert before.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
linked_node *
ln_insert_before(linked_node *const this_node, linked_node *other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->next = this_node;
	other_node->previous = this_node->previous;
	if (this_node->previous)
		this_node->previous->next = other_node;

	this_node->previous = other_node;
	return (other_node);
}

/**
 * ln_remove - deletes a linked node and returns its data.
 * @node: the node to delete.
 *
 * Return: pointer to the data of the node.
 */
void *ln_remove(linked_node *const node)
{
	void *d = NULL;

	if (!node)
		return (NULL);

	d = node->data;
	node->data = NULL;
	if (node->next)
		node->next->previous = node->previous;

	if (node->previous)
		node->previous->next = node->next;

	node->next = NULL;
	node->previous = NULL;
	free(node);
	return (d);
}

/**
 * ln_get_data - return the data in a linked node.
 * @node: the node.
 *
 * Return: pointer to the data.
 */
void *ln_get_data(linked_node const *const node)
{
	if (!node)
		return (NULL);

	return (node->data);
}

/**
 * ln_swap_data - replace data in a linked node.
 * @node: pointer to the node to modify.
 * @data: pointer to the data to swap in.
 * @copy_data: function that will be called to duplicate data, if NULL
 * only pointer to `data` is stored.
 *
 * Return: pointer to the old data in node, NULL on failure.
 */
void *
ln_swap_data(linked_node *const node, void *const data, dup_func *copy_data)
{
	if (!node)
		return (NULL);

	void *old_data = ln_get_data(node);

	node->data = data;
	if (copy_data)
	{
		node->data = copy_data(data);
		if (!node->data && data)
		{
			node->data = old_data;
			return (NULL);
		}
	}

	return (old_data);
}

/**
 * ln_get_next - return a linked node's successor.
 * @node: the node to query.
 *
 * Return: pointer to the next node.
 */
linked_node *ln_get_next(linked_node const *const node)
{
	if (!node)
		return (NULL);

	return (node->next);
}

/**
 * ln_get_prev - return a linked node's predecessor.
 * @node: the node to query.
 *
 * Return: pointer to the previous node.
 */
linked_node *ln_get_prev(linked_node const *const node)
{
	if (!node)
		return (NULL);

	return (node->previous);
}

/**
 * ll_clear - free a linked list.
 * @head: pointer to the head of the doubly linked list.
 * @free_data: function that will be called to free data in the nodes.
 *
 * Return: NULL always.
 */
void *ll_clear(linked_node *const head, delete_func *free_data)
{
	if (!head)
		return (NULL);

	linked_node *walk = head;
	void *data = NULL;

	while (walk->next)
	{
		walk = ln_get_next(walk);
		data = ln_remove(walk->previous);
		if (free_data)
			free_data(data);
	}

	data = ln_remove(walk);
	if (free_data)
		free_data(data);

	return (NULL);
}

/**
 * sll_print - print all nodes of a doubly linked list.
 * @stream: pointer to a stream to output to.
 * @head: head of the doubly linked list to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int sll_print(
	FILE *stream, single_link_node const *const head, print_func *print_data
)
{
	if (!stream || !head)
		return (-1);

	long int total_bytes = 0;
	int bytes_printed = 0;
	single_link_node const *walk = head;

	if (print_data)
		bytes_printed = print_data(stream, sln_get_data(walk));
	else
		bytes_printed = fprintf(stream, "%p", sln_get_data(walk));

	if (bytes_printed < 0)
		return (bytes_printed);

	total_bytes += bytes_printed;
	walk = sln_get_next(walk);
	while (walk)
	{
		bytes_printed = fprintf(stream, " --> ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		if (print_data)
			bytes_printed = print_data(stream, sln_get_data(walk));
		else
			bytes_printed = fprintf(stream, "%p", sln_get_data(walk));

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = sln_get_next(walk);
	}

	bytes_printed = fprintf(stream, "\n");
	if (bytes_printed < 0)
		return (bytes_printed);

	return (total_bytes + bytes_printed);
}

/**
 * dll_print - print all nodes of a doubly linked list.
 * @stream: pointer to a stream to print to.
 * @head: pointer to the head of the doubly linked list to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int dll_print(
	FILE *stream, double_link_node const *const head, print_func *print_data
)
{
	if (!stream || !head)
		return (-1);

	long int total_bytes = 0;
	int bytes_printed = 0;
	double_link_node const *walk = head;

	if (print_data)
		bytes_printed = print_data(stream, ln_get_data(walk));
	else
		bytes_printed = fprintf(stream, "%p", ln_get_data(walk));

	if (bytes_printed < 0)
		return (bytes_printed);

	total_bytes += bytes_printed;
	walk = ln_get_next(walk);
	while (walk)
	{
		bytes_printed = fprintf(stream, " <--> ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		if (print_data)
			bytes_printed = print_data(stream, ln_get_data(walk));
		else
			bytes_printed = fprintf(stream, "%p", ln_get_data(walk));

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = ln_get_next(walk);
	}

	bytes_printed = fprintf(stream, "\n");
	if (bytes_printed < 0)
		return (bytes_printed);

	return (total_bytes + bytes_printed);
}

/**
 * dll_print_reversed - print all nodes of a deque from tail to head.
 * @stream: pointer to the stream to output to.
 * @tail: pointer to the tail of the deque to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int dll_print_reversed(
	FILE *stream, double_link_node const *const tail, print_func *print_data
)
{
	if (!stream || !tail)
		return (-1);

	long int total_bytes = 0;
	int bytes_printed = 0;
	double_link_node const *walk = tail;

	if (print_data)
		bytes_printed = print_data(stream, ln_get_data(walk));
	else
		bytes_printed = fprintf(stream, "%p", ln_get_data(walk));

	if (bytes_printed < 0)
		return (bytes_printed);

	total_bytes += bytes_printed;
	walk = ln_get_prev(walk);
	while (walk)
	{
		bytes_printed = fprintf(stream, " <--> ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		void *d = ln_get_data(walk);

		if (print_data)
			bytes_printed = print_data(stream, d);
		else
			bytes_printed = fprintf(stream, "%p", d);

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = ln_get_prev(walk);
	}

	bytes_printed = fprintf(stream, "\n");
	if (bytes_printed < 0)
		return (bytes_printed);

	return (total_bytes + bytes_printed);
}
