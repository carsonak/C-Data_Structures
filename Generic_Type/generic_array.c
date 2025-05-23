#include "generic_types.h"

/*!
 * @brief prints out a formatted array.
 * @param stream pointer to stream to print to.
 * @param arr pointer to the array to format.
 * @param len number of items in the array.
 * @return total number of bytes printed, -1 on error.
 */
#define DEFINE_PRINT_TYPE_ARRAY_FUNCTION(TYPE, FORMAT)                                     \
	static int print_##TYPE##_array(FILE *stream, TYPE const *const arr, const size_t len) \
	{                                                                                      \
		int bytes_written = 0, total_bytes = 0;                                            \
                                                                                           \
		bytes_written = fprintf(stream, "{");                                              \
		if (bytes_written < 0)                                                             \
			return (bytes_written);                                                        \
                                                                                           \
		total_bytes += bytes_written;                                                      \
		for (size_t i = 0; i < len; ++i)                                                   \
		{                                                                                  \
			bytes_written = fprintf(stream, FORMAT, arr[i]);                               \
			if (bytes_written < 0)                                                         \
				return (bytes_written);                                                    \
                                                                                           \
			total_bytes += bytes_written;                                                  \
			if (i < len - 1)                                                               \
			{                                                                              \
				bytes_written = fprintf(stream, ", ");                                     \
				if (bytes_written < 0)                                                     \
					return (bytes_written);                                                \
                                                                                           \
				total_bytes += bytes_written;                                              \
			}                                                                              \
		}                                                                                  \
                                                                                           \
		bytes_written = fprintf(stream, "}\n");                                            \
		if (bytes_written < 0)                                                             \
			return (bytes_written);                                                        \
                                                                                           \
		return (total_bytes + bytes_written);                                              \
	}

/*!
 * @brief creates a new array of length `len`.
 * @param arr data to initialise with.
 * @param len number of items in the array.
 * @return pointer to the new array.
 */
#define DEFINE_GENARR_TYPE_NEW_FUNCTION(TYPE, TAG)                                     \
	static Generic_Array *genarr_##TYPE##_new(TYPE const *const arr, const size_t len) \
	{                                                                                  \
		Generic_Array *new_ga = malloc(sizeof(*new_ga) + (sizeof(*arr) * len));        \
                                                                                       \
		if (!new_ga)                                                                   \
			return (NULL);                                                             \
                                                                                       \
		*new_ga = (Generic_Array){                                                     \
			.type = TAG,                                                               \
			.len = len,                                                                \
			.data.p_##TYPE = arr ? (TYPE *)(new_ga + 1) : NULL,                        \
		};                                                                             \
                                                                                       \
		if (arr)                                                                       \
			memcpy(new_ga->data.p_##TYPE, arr, sizeof(*arr) * len);                    \
                                                                                       \
		return (new_ga);                                                               \
	}

/*!
 * @brief generic array type.
 */
struct Generic_Array
{
	enum genarr_id type;	  //!< the type of data in the array.
	size_t len;				  //!< number of items in the array.
	union pointer_types data; //!< union of possible types of data pointers.
};

DEFINE_GENARR_TYPE_NEW_FUNCTION(char, STRING)
DEFINE_GENARR_TYPE_NEW_FUNCTION(int, INT_ARR)
DEFINE_GENARR_TYPE_NEW_FUNCTION(size_t, SIZET_ARR)
DEFINE_GENARR_TYPE_NEW_FUNCTION(double, DOUBLE_ARR)

/*!
 * @brief creates a `Generic_Array` of the given type and size.
 * @param type enumerated tag for type of the `Generic_Array`.
 * @param data optional content to initialise the `Generic_Array` with.
 * @param len number of items in data.
 * @return pointer to the new `Generic_Array`, NULL on failure.
 */
Generic_Array *genarr_new(
	enum genarr_id type, void const *const data, const size_t len)
{
	Generic_Array *new_ga = NULL;

	switch (type)
	{
	case STRING:
		/* Accounting for '\0'. */
		new_ga = genarr_char_new(data, len + (len ? 1 : 0));
		if (new_ga && len)
		{
			--new_ga->len;
			new_ga->data.p_char[len] = 0;
		}

		break;
	case INT_ARR:
		new_ga = genarr_int_new(data, len);
		break;
	case SIZET_ARR:
		new_ga = genarr_size_t_new(data, len);
		break;
	case DOUBLE_ARR:
		new_ga = genarr_double_new(data, len);
		break;
	default:
		break;
	}

	return (new_ga);
}

/*!
 * @brief free a `Generic_Array` from memory.
 * @param ga pointer to the `Generic_Array`.
 * @return NULL always.
 */
void *genarr_delete(Generic_Array *const ga)
{
	if (!ga)
		return (NULL);

	ga->type = -1;
	ga->len = 0;
	free(ga);
	return (NULL);
}

/*!
 * @brief return the enumerated type of the `Generic_Array`.
 * @param ga pointer to the array.
 * @return the enumerated type of the Generic Array, -1 if NULL.
 */
enum genarr_id genarr_type(Generic_Array const *const ga)
{
	if (!ga)
		return (-1);

	return (ga->type);
}

/*!
 * @brief return number of items in the array.
 * @param ga pointer to the array.
 * @return number of items in the array, -1 if NULL.
 */
size_t genarr_len(Generic_Array const *const ga)
{
	if (!ga)
		return (-1);

	return (ga->len);
}

/*!
 * @brief return the total size of the `Generic_Array` in bytes.
 * @param ga pointer to the Generic Array.
 * @return total size of the `Generic_Array` in bytes.
 */
size_t genarr_size(Generic_Array const *const ga)
{
	if (!ga)
		return (-1);

	switch (ga->type)
	{
	case STRING:
		return (sizeof(*ga) + (ga->len * sizeof(*ga->data.p_char) + (ga->len ? 1 : 0)));
	case INT_ARR:
		return (sizeof(*ga) + (ga->len * sizeof(*ga->data.p_int)));
	case SIZET_ARR:
		return (sizeof(*ga) + (ga->len * sizeof(*ga->data.p_size_t)));
	case DOUBLE_ARR:
		return (sizeof(*ga) + (ga->len * sizeof(*ga->data.p_double)));
	default:
		return (-1);
	}
}

/*!
 * @brief if possible retrieve value at index `i` into `out`.
 * This function can also be used to test if an index is within bounds of the
 * array. If out pointer is NULL, the function will check the index and return
 * true if it is within bounds otherwise false.
 *
 * @param ga pointer to a Generic Array.
 * @param i index to retrieve.
 * @param out pointer to a a variable to store the value at `i`.
 * @return true if operation was successful, false otherwise e.g, Out of Bounds.
 */
bool genarr_index_get(Generic_Array const *const ga, const size_t i, void *const out)
{
	if (!ga || (i >= ga->len))
		return (false);

	if (!out)
		return (true);

	switch (ga->type)
	{
	case STRING:
		*((char *)out) = ga->data.p_char[i];
		break;
	case INT_ARR:
		*((int *)out) = ga->data.p_int[i];
		break;
	case SIZET_ARR:
		*((size_t *)out) = ga->data.p_size_t[i];
		break;
	case DOUBLE_ARR:
		*((double *)out) = ga->data.p_double[i];
		break;
	default:
		break;
	}

	return (true);
}

/*!
 * @brief if possible insert a value at the given index.
 * @param ga pointer to the Generic Array.
 * @param i the index to insert at.
 * @param val pointer to the value to insert.
 * @return true on success, false on failure, e.g, Out of Bounds.
 */
bool genarr_index_put(
	Generic_Array const *const ga, const size_t i, void const *const val)
{
	if (!ga || (i >= ga->len) || !val)
		return (false);

	switch (ga->type)
	{
	case STRING:
		ga->data.p_char[i] = *((char *)val);
		break;
	case INT_ARR:
		ga->data.p_int[i] = *((int *)val);
		break;
	case SIZET_ARR:
		ga->data.p_size_t[i] = *((size_t *)val);
		break;
	case DOUBLE_ARR:
		ga->data.p_double[i] = *((double *)val);
		break;
	default:
		break;
	}

	return (true);
}

DEFINE_PRINT_TYPE_ARRAY_FUNCTION(int, "%d")
DEFINE_PRINT_TYPE_ARRAY_FUNCTION(size_t, "%zu")
DEFINE_PRINT_TYPE_ARRAY_FUNCTION(double, "%f")

/*!
 * @brief prints out a `Generic_Array` to a stream.
 * @param ga pointer to the generic array to print.
 * @param stream pointer to the stream to use.
 * @return number of bytes printed, -1 on failure.
 */
int genarr_print(Generic_Array const *const ga, FILE *stream)
{
	int bytes_written = 0;

	if (!ga)
		return (0);

	switch (ga->type)
	{
	case STRING:
		bytes_written = fprintf(stream, "'%s'\n", ga->data.p_char);
		break;
	case INT_ARR:
		bytes_written = print_int_array(stream, ga->data.p_int, ga->len);
		break;
	case SIZET_ARR:
		bytes_written = print_size_t_array(stream, ga->data.p_size_t, ga->len);
		break;
	case DOUBLE_ARR:
		bytes_written = print_double_array(stream, ga->data.p_double, ga->len);
		break;
	default:
		fprintf(stream, "Error: invalid Generic_Array type\n");
		break;
	}

	return (bytes_written);
}
