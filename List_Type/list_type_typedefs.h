#ifndef LIST_TYPE_TYPEDEFS_H
#define LIST_TYPE_TYPEDEFS_H

#include <stdio.h> /* FILE */

/**
 * dup_func - a function that duplicates an object.
 * @data: pointer to the object to be duplicated.
 *
 * Return: pointer to the duplicated object, NULL on failure.
 */
typedef void *(dup_func)(void const *const data);

/**
 * free_func - a function that frees an object from memory.
 * @data: pointer to the object to delete.
 */
typedef void(free_func)(void *const data);

/**
 * print_func - a function that prints an object.
 * @stream: pointer to a stream to print to.
 * @data: the object to print.
 *
 * Return: number of characters printed, negative number on error.
 */
typedef int(print_func)(FILE *stream, void const *const data);

typedef struct linked_node linked_node;
typedef struct deque deque;

#endif /* LIST_TYPE_TYPEDEFS_H */
