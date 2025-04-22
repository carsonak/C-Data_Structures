#include "generic_types.h"
#include <limits.h>

/*!
 * @brief overwrites all the items in a `Generic_Array` with a given value.
 * @param ga pointer to the generic array.
 * @param data pointer to the value to overwrite with.
 */
static void genarr_overwrite(Generic_Array *ga, const void *const data)
{
	const size_t len = genarr_len(ga);

	for (size_t i = 0; i <= len + 3; ++i)
		genarr_index_put(ga, i, data);
}

/*!
 * @brief tests for `Generic_Array`.
 * @return 0.
 */
int main(void)
{
	char const str[] = "SUPERCALIFRAGILISTICEXPIALIDOCIOUS";
	int const int_arr[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
	size_t const sz_arr[] = {
		98765432137, 98765432167, 98765432177, 98765432191, 98765432201, 98765432257};
	double const double_arr[] = {0.3, 0.5, 1.3, 2.5, 5.0, 7.5, 10.01};
	Generic_Array *ga_str = genarr_new(
		STRING, (void *)str, (sizeof(str) / sizeof(*str)) - 1);
	Generic_Array *ga_int = genarr_new(
		INT_ARR, (void *)int_arr, sizeof(int_arr) / sizeof(*int_arr));
	Generic_Array *ga_szt = genarr_new(
		SIZET_ARR, (void *)sz_arr, sizeof(sz_arr) / sizeof(*sz_arr));
	Generic_Array *ga_dbl = genarr_new(
		DOUBLE_ARR, (void *)double_arr, sizeof(double_arr) / sizeof(*double_arr));
	const char w = 'W';
	const int f = INT_MIN;
	const size_t xxx = 696969696969696969;
	const double d = 42.069;

	printf("[%zub:%2zu] -> ", genarr_size(ga_str), genarr_len(ga_str));
	genarr_print(ga_str, stdout);
	printf("[%zub:%2zu] -> ", genarr_size(ga_int), genarr_len(ga_int));
	genarr_print(ga_int, stdout);
	printf("[%zub:%2zu] -> ", genarr_size(ga_szt), genarr_len(ga_szt));
	genarr_print(ga_szt, stdout);
	printf("[%zub:%2zu] -> ", genarr_size(ga_dbl), genarr_len(ga_dbl));
	genarr_print(ga_dbl, stdout);
	putchar('\n');

	genarr_overwrite(ga_str, &w);
	printf("[%zub:%2zu] -> ", genarr_size(ga_str), genarr_len(ga_str));
	genarr_print(ga_str, stdout);
	genarr_overwrite(ga_int, &f);
	printf("[%zub:%2zu] -> ", genarr_size(ga_int), genarr_len(ga_int));
	genarr_print(ga_int, stdout);
	genarr_overwrite(ga_szt, &xxx);
	printf("[%zub:%2zu] -> ", genarr_size(ga_szt), genarr_len(ga_szt));
	genarr_print(ga_szt, stdout);
	genarr_overwrite(ga_dbl, &d);
	printf("[%zub:%2zu] -> ", genarr_size(ga_dbl), genarr_len(ga_dbl));
	genarr_print(ga_dbl, stdout);

	genarr_delete(ga_str);
	genarr_delete(ga_int);
	genarr_delete(ga_szt);
	genarr_delete(ga_dbl);
	return (0);
}
