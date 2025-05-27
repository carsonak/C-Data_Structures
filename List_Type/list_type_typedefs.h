#ifndef DS_LIST_TYPE_TYPEDEFS_H
#define DS_LIST_TYPE_TYPEDEFS_H

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
 * data_tostr - a function that stringifies an object.
 * @data: the object to stringify.
 *
 * Return: pointer to the stringified data(must be null terminated),
 * NULL error.
 */
typedef char *(data_tostr)(void const *const data);

typedef struct list_node list_node;
typedef struct deque deque;

#endif /* DS_LIST_TYPE_TYPEDEFS_H */
