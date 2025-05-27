/* vsnprintf */
#define _ISOC99_SOURCE

#include <assert.h> /* asserts */
#include <stdarg.h> /* va_arg, vsnprintf */
#include <stdio.h>  /* vsnprintf */
#include <stdlib.h> /* *alloc */
#include <string.h> /* strlen, strcat */

#include "list_node.h"
#include "list_type_structs.h"

static char *format_str(
	const char *const restrict fmt, ...
) ATTR_NONNULL ATTR_FORMAT(printf, 1, 2);

/**
 * node_get_next - get next node.
 * @node: pointer to a `list_node`, should not be NULL.
 *
 * Return: pointer to the next node.
 */
list_node *lstnode_get_next(const list_node *const restrict node)
{
	assert(node);
	return (node->next);
}

/**
 * node_get_prev - get previous node.
 * @node: pointer to a `list_node`, should not be NULL.
 *
 * Return: pointer to the previous node.
 */
list_node *lstnode_get_prev(const list_node *const restrict node)
{
	assert(node);
	return (node->prev);
}

/**
 * node_get_data - get data of a node.
 * @node: pointer to a `list_node`, should not be NULL.
 *
 * Return: pointer to the data in the node.
 */
void *lstnode_get_data(const list_node *const restrict node)
{
	assert(node);
	return (node->data);
}

/**
 * node_set_next - swap out the next node of a another node.
 * @this_node: pointer to a `list_node` to be modified, should not be NULL.
 * @other_node: pointer to the `list_node` to be swapped in.
 *
 * Return: pointer to the old next node.
 */
list_node *lstnode_set_next(
	list_node *const restrict this_node, list_node *const restrict other_node
)
{
	assert(this_node);
	list_node *const tmp = this_node->next;

	this_node->next = other_node;
	return (tmp);
}

/**
 * node_set_prev - swap out the previous node of a another node.
 * @this_node: pointer to a `list_node` to be modified, should not be NULL.
 * @other_node: pointer to the `list_node` to be swapped in.
 *
 * Return: pointer to the old previous node.
 */
list_node *lstnode_set_prev(
	list_node *const restrict this_node, list_node *const restrict other_node
)
{
	assert(this_node);
	list_node *const tmp = this_node->prev;

	this_node->prev = other_node;
	return (tmp);
}

/**
 * node_set_data - swap the data of a node with new data.
 * @node: pointer to a `list_node` to modify, should not be NULL.
 * @data: pointer to the data to swap in.
 *
 * Return: pointer to the old data.
 */
void *
lstnode_set_data(list_node *const restrict node, void *const restrict data)
{
	assert(node);
	void *const tmp = node->data;

	node->data = data;
	return (tmp);
}

/**
 * node_new - allocate memory for a new `list_node` and initialise it.
 * @data: pointer to the data to be stored in the new node.
 * @copy_data: pointer to a function that can duplicate `data`.
 *
 * Return: pointer to the created node, NULL on failure.
 */
list_node *lstnode_new(void *data, dup_func *copy_data)
{
	list_node *node = calloc(1, sizeof(list_node));

	if (!node)
		return (NULL);

	lstnode_set_data(node, data);
	if (copy_data)
	{
		lstnode_set_data(node, copy_data(data));
		if (!lstnode_get_data(node) && data)
		{
			free(node);
			node = NULL;
		}
	}

	return (node);
}

/**
 * node_pop - remove a node from a linked list.
 * @node: pointer to a `list_node`.
 *
 * Return: pointer to the popped list_node.
 */
list_node *lstnode_pop(list_node *const restrict node)
{
	if (!node)
		return (NULL);

	list_node *const restrict next = lstnode_set_next(node, NULL);
	list_node *const restrict prev = lstnode_set_prev(node, NULL);

	if (next)
		lstnode_set_prev(next, prev);

	if (prev)
		lstnode_set_next(prev, next);

	return (node);
}

/**
 * node_del - pop and free a `list_node`.
 * @node: pointer to a `list_node`.
 *
 * Return: pointer to the data in the node.
 */
void *lstnode_del(list_node *const restrict node)
{
	if (!node)
		return (NULL);

	void *const data = lstnode_set_data(node, NULL);

	free(lstnode_pop(node));
	return (data);
}

/**
 * node_insert_after - insert a node after another node.
 * @this_node: pointer to a node.
 * @other_node: pointer to the node to be inserted.
 *
 * Return: pointer to the newly inserted node, NULL if `other_node` is NULL.
 */
list_node *lstnode_insert_after(
	list_node *const restrict this_node, list_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	list_node *const restrict this_next =
		lstnode_set_next(this_node, other_node);
	if (this_next)
		lstnode_set_prev(this_next, other_node);

	lstnode_set_next(other_node, this_next);
	lstnode_set_prev(other_node, this_node);
	return (other_node);
}

/**
 * node_insert_before - insert a node before another node.
 * @this_node: pointer to a node.
 * @other_node: pointer to the node to be inserted.
 *
 * Return: pointer to the newly inserted node, NULL if `other_node` is NULL.
 */
list_node *lstnode_insert_before(
	list_node *const restrict this_node, list_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	list_node *const restrict this_prev =
		lstnode_set_prev(this_node, other_node);
	if (this_prev)
		lstnode_set_next(this_prev, other_node);

	lstnode_set_next(other_node, this_node);
	lstnode_set_prev(other_node, this_prev);
	return (other_node);
}

/**
 * node_swap - swap data of two nodes.
 * @this_node: pointer to the node to be swapped out.
 * @other_node: pointer to the node to be swapped in.
 *
 * Return: pointer to the swapped in node, NULL if `other_node` is NULL.
 */
list_node *lstnode_swap(
	list_node *const restrict this_node, list_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	lstnode_set_data(
		other_node, lstnode_set_data(this_node, lstnode_get_data(other_node))
	);
	return (other_node);
}

/**
 * linked_list_del - free a linked list.
 * @head: pointer to the start of the linked list.
 * @free_data: function that will be called to free data in the nodes.
 *
 * Return: NULL always.
 */
void *linked_list_del(list_node *const head, free_func *free_data)
{
	if (!head)
		return (NULL);

	list_node *restrict walk = head, *next = lstnode_get_next(head);

	while (walk)
	{
		void *data = lstnode_del(walk);
		if (free_data)
			free_data(data);

		walk = next;
		next = walk ? lstnode_get_next(walk) : NULL;
	}

	return (NULL);
}

/**
 * format_str - return a printf style formatted string.
 * @fmt: pointer to a string with format specifiers.
 *
 * Return: pointer to the formatted string, NULL on failure.
 */
static char *format_str(const char *const restrict fmt, ...)
{
	intmax_t size = 0;
	char *restrict out_str = NULL;
	va_list vars;

	va_start(vars, fmt);
	size = vsnprintf(NULL, 0, fmt, vars);
	va_end(vars);
	if (size < 0)
		return (NULL);

	out_str = malloc(sizeof(*out_str) * (++size));
	if (!out_str)
		return (NULL);

	va_start(vars, fmt);
	size = vsnprintf(out_str, size, fmt, vars);
	va_end(vars);
	if (size < 0)
	{
		free(out_str);
		out_str = NULL;
	}

	return (out_str);
}

/**
 * linked_list_tostr - stringify a linked list.
 * @head: pointer to the start of the linked list to stringify.
 * @stringify_data: function that will be called to stringify the data
 * in the linked list nodes.
 *
 * Return: pointer to the stringified linked list, NULL on error.
 */
char *linked_list_tostr(
	list_node const *const restrict head, data_tostr *stringify_data
)
{
	if (!head)
		return (NULL);

	const char link[] = " <--> ";
	char *restrict list_str = NULL, *restrict data_str = NULL;
	intmax_t list_str_len = 0;
	list_node const *restrict walk = head;

	if (stringify_data)
		list_str = stringify_data(lstnode_get_data(walk));
	else
		list_str = format_str("%p", lstnode_get_data(walk));

	if (!list_str)
		return (NULL);

	list_str_len += strlen(list_str);
	walk = lstnode_get_next(walk);
	while (list_str && walk)
	{
		void *d = lstnode_get_data(walk);

		if (stringify_data)
			data_str = stringify_data(d);
		else
			data_str = format_str("%p", d);

		if (!data_str)
			goto malloc_error;

		list_str_len += sizeof(link) - 1 + strlen(data_str);
		list_str = realloc(list_str, list_str_len + 1);
		if (list_str)
			strcat(strcat(list_str, link), data_str);

		free(data_str);
		data_str = NULL;
		walk = lstnode_get_next(walk);
	}

	if (0)
	{
malloc_error:
		free(list_str);
		list_str = NULL;
	}

	return (list_str);
}

/**
 * linked_list_tostr_reversed - stringify a linked list in reverse.
 * @tail: pointer to the tail of the linked list.
 * @stringify_data: function that will be called to stringify the data
 * in the linked list nodes.
 *
 * Return: pointer to the stringified linked list, NULL on error.
 */
char *linked_list_tostr_reversed(
	list_node const *const restrict tail, data_tostr *stringify_data
)
{
	if (!tail)
		return (NULL);

	const char link[] = " <--> ";
	char *restrict list_str = NULL, *restrict data_str = NULL;
	intmax_t list_str_len = 0;
	list_node const *restrict walk = tail;

	if (stringify_data)
		list_str = stringify_data(lstnode_get_data(walk));
	else
		list_str = format_str("%p", lstnode_get_data(walk));

	if (!list_str)
		return (NULL);

	list_str_len += strlen(list_str);
	walk = lstnode_get_prev(walk);
	while (list_str && walk)
	{
		void *d = lstnode_get_data(walk);

		if (stringify_data)
			data_str = stringify_data(d);
		else
			data_str = format_str("%p", d);

		if (!data_str)
			goto malloc_error;

		list_str_len += sizeof(link) - 1 + strlen(data_str);
		list_str = realloc(list_str, list_str_len + 1);
		if (list_str)
			strcat(strcat(list_str, link), data_str);

		free(data_str);
		data_str = NULL;
		walk = lstnode_get_prev(walk);
	}

	if (0)
	{
malloc_error:
		free(list_str);
		list_str = NULL;
	}

	return (list_str);
}
