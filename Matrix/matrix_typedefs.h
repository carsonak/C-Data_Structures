#ifndef TYPEDEFS_H
#define TYPEDEFS_H

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
typedef void(free_func)(void *const data);

/**
 * data_tostr - a function that stringifies an object.
 * @data: the object to stringify.
 *
 * Return: pointer to the stringified data, NULL error.
 */
typedef char *(data_tostr)(void const *const data);

#endif /* TYPEDEFS_H */
