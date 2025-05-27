#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>	  /* free */
#include <inttypes.h> /* intmax_t */

#include "matrix_typedefs.h"

void *delete_2D_array(
	void **const restrict array, const size_t size, free_func *free_row
);
void **dup_2D_array(
	void **const restrict array, const intmax_t size, dup_func *copy_data,
	free_func *free_data
);

#endif /* MATRIX_H */
