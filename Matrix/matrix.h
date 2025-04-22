#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>	  /* free */
#include <inttypes.h> /* intmax_t */

#include "typedefs.h"

void *delete_2D_array(void **array, size_t size, delete_func *free_row);
void **dup_2D_array(
	void **const array, const intmax_t size,
	dup_func *copy_data, delete_func *free_data);

#endif /* MATRIX_H */
