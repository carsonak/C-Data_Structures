#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include <stdlib.h> /* *alloc */
#include <string.h> /* memcpy */
#include <stdbool.h>
#include <stdio.h> /* *printf */

typedef struct Generic_Type Generic_Type;

/*!
 * @brief enumeration of Generic_Type types.
 */
enum gentyp_id
{
	CHAR,
	INT,
	SIZET,
	DOUBLE,
};

typedef struct Generic_Array Generic_Array;

/*!
 * @brief numeration of Generic_Array types.
 */
enum genarr_id
{
	STRING,
	INT_ARR,
	SIZET_ARR,
	DOUBLE_ARR,
};

/*!
 * @brief union of different pointers.
 */
union pointer_types
{
	char *p_char;
	int *p_int;
	size_t *p_size_t;
	double *p_double;
};

Generic_Array *genarr_new(
	enum genarr_id type, const void *const data, const size_t len);
void *genarr_delete(Generic_Array *const ga);
enum genarr_id genarr_type(Generic_Array const *const ga);
size_t genarr_len(Generic_Array const *const ga);
size_t genarr_size(Generic_Array const *const ga);
bool genarr_index_get(
	Generic_Array const *const ga, const size_t i, void *const out);
bool genarr_index_put(
	Generic_Array const *const ga, const size_t i, void const *const val);
int genarr_print(Generic_Array const *const ga, FILE *stream);

#endif /* GENERIC_TYPE_H */
