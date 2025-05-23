#include "matrix.h"

/**
 * delete_2D_array - free a 2 dimensional array from memory.
 * @array: a 2 dimensional array.
 * @size: number of columns in the array, should be greater than 0.
 * @free_row: pointer to a function that will be used to free the rows.
 *
 * Return: NULL always.
 */
void *delete_2D_array(
	void **const restrict array, const size_t size, free_func *free_row
)
{
	if (!array || size < 1)
		return (NULL);

	for (size_t a_i = 0; a_i < size; ++a_i)
	{
		free_row(array[a_i]);
		array[a_i] = NULL;
	}

	free(array);
	return (NULL);
}

/**
 * dup_2D_array - duplicates a 2 dimensional array in memory.
 * @array: the array to duplicate.
 * @size: number of columns in the array, should be greater than 0.
 * @copy_data: optional pointer to a function that will be used to duplicate
 * the data, if not provided, array will contain pointers to the original data
 * in the array.
 * @free_data: optional pointer to a function that will be used to free data in
 * the array in case of failure. If `copy_data` is provided this function must
 * also be provided, otherwise no data duplication will occur.
 *
 * Return: the duplicated array on success, NULL on error.
 */
void **dup_2D_array(
	void **const restrict array, const intmax_t size, dup_func *copy_data,
	free_func *free_data
)
{
	intmax_t arr_i = 0;
	void **array_dup = NULL;

	if (!array || size < 1)
		return (NULL);

	array_dup = calloc(size + 1, sizeof(*array_dup));
	if (!array_dup)
		return (NULL);

	for (arr_i = 0; arr_i < size; ++arr_i)
	{
		array_dup[arr_i] = array[arr_i];
		if (copy_data && free_data)
		{
			array_dup[arr_i] = copy_data(array[arr_i]);
			if (!array_dup[arr_i])
				return (delete_2D_array(array_dup, arr_i, free_data));
		}
	}

	return (array_dup);
}
