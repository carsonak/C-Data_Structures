#ifndef LIST_TYPES_H
#define LIST_TYPES_H

#include <stdio.h> /* FILE */

/**
 * dup_func - a function that duplicates an object.
 * @data: pointer to the object to be duplicated.
 *
 * Return: pointer to the duplicate, NULL on failure.
 */
typedef void *(dup_func)(void const *const data);

/**
 * delete_func - a function that deletes an object.
 * @data: pointer to the object to delete.
 */
typedef void(delete_func)(void *const data);

/**
 * print_func - a function that prints an object.
 * @stream: pointer to a stream to print to.
 * @data: the object to print.
 *
 * Return: number of characters printed, negative number on error.
 */
typedef int(print_func)(FILE *stream, void const *const data);

typedef struct linked_node linked_node;
typedef struct linked_node single_link_node;
typedef struct linked_node double_link_node;

typedef struct linked_list queue;
typedef struct linked_list deque;
typedef struct linked_list stack;

#endif /* LIST_TYPES_H */
