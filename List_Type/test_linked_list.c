#define _POSIX_C_SOURCE 200809L
#include "linked_list.h"
#include "tau/tau.h"
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH ((unsigned int)256)

static char original[] = "original";
static char n1d[] = "one", n2d[] = "two", n3d[] = "three";

/**
 * fail_dup - failing duplicating function.
 * @d: unused.
 *
 * Return: NULL.
 */
static void *fail_dup(void const *const d)
{
	(void)d;
	return (NULL);
}

/**
 * dup_str - makes a copy of a string.
 * @str: pointer to the string.
 *
 * Return: pointer to the new string, NULL on failure.
 */
static void *dup_str(void const *const str)
{
	char const *const s = str;

	if (!s)
		return (NULL);

	unsigned int len = 0;

	while (s[len] && len <= MAX_STRING_LENGTH)
		++len;

	char *const s_dup = malloc(sizeof(*s_dup) * (len + 1));

	if (s_dup)
	{
		memcpy(s_dup, s, sizeof(*s_dup) * len);
		s_dup[len] = '\0';
	}

	return (s_dup);
}

TAU_MAIN()

/*######################################################################*/
/*######################################################################*/

TEST(node_creation, new_NULL_NULL_returns_NULL)
{
	linked_node *n = ln_new(NULL, NULL);

	REQUIRE(n != NULL, "ln_new() should return non-null pointer");

	CHECK(ln_get_data(n) == NULL, "data should be NULL");
	CHECK(ln_get_next(n) == NULL, "pointer to next node should be NULL");
	CHECK(ln_get_prev(n) == NULL, "pointer to previous node should be NULL");
	free(n);
}

TEST(node_creation, new_d_NULL_returns_node_with_unchanged_d_pointer)
{
	char input[] = "input";
	linked_node *n = ln_new(input, NULL);

	REQUIRE(n != NULL, "ln_new() should return non-null pointer");

	CHECK(ln_get_data(n) == input, "data should point to the same object");
	CHECK(ln_get_next(n) == NULL, "pointer to next node should be NULL");
	CHECK(ln_get_prev(n) == NULL, "pointer to previous node should be NULL");
	free(n);
}

TEST(node_creation, new_NULL_f_returns_NULL)
{
	linked_node *n = ln_new(NULL, dup_str);

	REQUIRE(n != NULL, "ln_new() should return non-null pointer");

	CHECK(ln_get_data(n) == NULL, "data should be NULL");
	CHECK(ln_get_next(n) == NULL, "pointer to next node should be NULL");
	CHECK(ln_get_prev(n) == NULL, "pointer to previous node should be NULL");
	free(n);
}

TEST(node_creation, new_d_f_returns_correct_node)
{
	char input[] = "input";
	linked_node *n = ln_new(input, dup_str);

	REQUIRE(n != NULL, "ln_new() should return non-null pointer");
	char *const output = ln_get_data(n);

	CHECK(output != input, "data should point to a duplicated object");
	CHECK_STREQ(output, input, "duplicated data should be equal to input");
	CHECK(ln_get_next(n) == NULL, "pointer to next node should be NULL");
	CHECK(ln_get_prev(n) == NULL, "pointer to previous node should be NULL");
	free(output);
	free(n);
}

TEST(node_creation, new_d_faildup_returns_NULL)
{
	char input[] = "input";
	linked_node *n = ln_new(input, fail_dup);

	REQUIRE(
		n == NULL, "ln_new() should return NULL if data duplication fails"
	);
}

/*######################################################################*/
/*######################################################################*/

TEST(data_get, get_data_handles_NULL)
{
	CHECK(ln_get_data(NULL) == NULL, "should not process NULL");
}

TEST(data_get, get_data_returns_correct_data)
{
	char input[] = "input";
	linked_node *n1 = ln_new(input, NULL);

	REQUIRE(n1 != NULL, "ln_new() should return non-null pointer");
	CHECK(ln_get_data(n1) == input, "data should point to the same object");
	free(n1);
}

TEST(data_get, get_data_returns_correct_duplicated_data)
{
	char input[] = "input";
	linked_node *n1 = ln_new(input, dup_str);

	REQUIRE(n1 != NULL, "ln_new() should return non-null pointer");
	char *const curr_data = ln_get_data(n1);

	CHECK(curr_data != input, "data should point to a duplicated object");
	CHECK_STREQ(
		curr_data, input, "duplicated object should be equal to input"
	);
	free(curr_data);
	free(n1);
}

/*######################################################################*/
/*######################################################################*/

struct data_swap
{
	linked_node *n1;
};

TEST_F_SETUP(data_swap)
{
	tau->n1 = ln_new(original, NULL);
	REQUIRE(tau->n1 != NULL, "ln_new() should return non-null pointer");
}

TEST_F_TEARDOWN(data_swap) { free(tau->n1); }

TEST(data_swap, swap_NULL_NULL_NULL)
{
	CHECK(
		ln_swap_data(NULL, NULL, NULL) == NULL, "should not swap NULL pointer"
	);
	CHECK(
		ln_swap_data(NULL, "NULL", NULL) == NULL,
		"should not swap NULL pointer"
	);
	CHECK(
		ln_swap_data(NULL, NULL, fail_dup) == NULL,
		"should not swap NULL pointer"
	);
	CHECK(
		ln_swap_data(NULL, "NULL", fail_dup) == NULL,
		"should not swap NULL pointer"
	);
}

TEST_F(data_swap, swap_n_d_NULL)
{
	char input[] = "input";

	CHECK(
		ln_swap_data(tau->n1, input, NULL) == original,
		"should return old data"
	);

	CHECK(ln_get_data(tau->n1) == input, "data is equal to the input");
}

TEST_F(data_swap, swap_n_NULL_NULL_nullifies_data)
{
	CHECK(
		ln_swap_data(tau->n1, NULL, NULL) == original, "should return old data"
	);

	CHECK(ln_get_data(tau->n1) == NULL, "data should be NULL");
}

TEST_F(data_swap, swap_n_d_faildup_returns_NULL)
{
	CHECK(
		ln_swap_data(tau->n1, "input", fail_dup) == NULL,
		"should return NULL on failure to duplicate"
	);

	CHECK(ln_get_data(tau->n1) == original, "data should be unchanged");
}

TEST_F(data_swap, swap_n_d_f)
{
	char *const input = "input";

	CHECK(
		ln_swap_data(tau->n1, input, dup_str) == original,
		"should return old data"
	);
	char *const curr_data = ln_get_data(tau->n1);

	CHECK(curr_data != input, "data should be duplicated");
	CHECK_STREQ(curr_data, input, "data should be equal to input");
	free(curr_data);
}

/*######################################################################*/
/*######################################################################*/

struct node_insertion
{
	linked_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_insertion)
{
	tau->n1 = ln_new((char *const)n1d, NULL);
	tau->n2 = ln_new((char *const)n2d, NULL);
	tau->n3 = ln_new((char *const)n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->n3);
	}

	REQUIRE(tau->n1 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n2 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n3 != NULL, "ln_new() should return non-null pointer");
}

TEST_F_TEARDOWN(node_insertion)
{
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(node_insertion, insert_NULLs)
{
	CHECK(ln_insert_after(NULL, NULL) == NULL, "nothing is done");
	CHECK(ln_insert_before(NULL, NULL) == NULL, "nothing is done");
}

TEST_F(node_insertion, insert_after_NULL)
{
	REQUIRE(
		ln_insert_after(NULL, tau->n2) == tau->n2,
		"the newly inserted node should be n2"
	);

	CHECK(ln_get_next(tau->n2) == NULL, "n2 next should be unchanged");
	CHECK(ln_get_prev(tau->n2) == NULL, "n2 previous should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "data should be unchanged");
}

TEST_F(node_insertion, insert_NULL_after)
{
	REQUIRE(
		ln_insert_after(tau->n1, NULL) == tau->n1,
		"the newly inserted node should be n1"
	);

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be unchanged");
	CHECK(ln_get_prev(tau->n2) == NULL, "n1 previous should be unchanged");
	CHECK(ln_get_data(tau->n1) == n1d, "data should be unchanged");
}

TEST_F(node_insertion, insert_after_1_node)
{
	REQUIRE(
		ln_insert_after(tau->n1, tau->n2) == tau->n2,
		"the newly inserted node should be n2"
	);

	CHECK(ln_get_next(tau->n2) == NULL, "n2 next should be unchanged");
	CHECK(ln_get_next(tau->n1) == tau->n2, "n1 next should be n2");

	CHECK(ln_get_prev(tau->n2) == tau->n1, "n2 previous should be n1");
	CHECK(ln_get_prev(tau->n1) == NULL, "n1 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
}

TEST_F(node_insertion, insert_after_2_nodes)
{
	linked_node *output = ln_insert_after(tau->n1, tau->n2);

	REQUIRE(output == tau->n2, "the newly inserted node should be n2");
	output = ln_insert_after(tau->n2, tau->n3);
	REQUIRE(output == tau->n3, "the newly inserted node should be n3");

	CHECK(ln_get_next(tau->n3) == NULL, "n3 next should be unchanged");
	CHECK(ln_get_next(tau->n2) == tau->n3, "n2 next should be n3");
	CHECK(ln_get_next(tau->n1) == tau->n2, "n1 next should be n2");

	CHECK(ln_get_prev(tau->n3) == tau->n2, "n3 previous should be n2");
	CHECK(ln_get_prev(tau->n2) == tau->n1, "n2 previous should be n1");
	CHECK(ln_get_prev(tau->n1) == NULL, "n1 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
	CHECK(ln_get_data(tau->n3) == n3d, "n3 data should be unchanged");
}

TEST_F(node_insertion, insert_after_between_2_nodes)
{
	linked_node *output = ln_insert_after(tau->n1, tau->n3);

	REQUIRE(output == tau->n3, "the newly inserted node should be n3");
	output = ln_insert_after(tau->n1, tau->n2);
	REQUIRE(output == tau->n2, "the newly inserted node should be n2");

	CHECK(ln_get_next(tau->n3) == NULL, "n3 next should be unchanged");
	CHECK(ln_get_next(tau->n2) == tau->n3, "n2 next should be n3");
	CHECK(ln_get_next(tau->n1) == tau->n2, "n1 next should be n2");

	CHECK(ln_get_prev(tau->n3) == tau->n2, "n3 previous should be n2");
	CHECK(ln_get_prev(tau->n2) == tau->n1, "n2 previous should be n1");
	CHECK(ln_get_prev(tau->n1) == NULL, "n1 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
	CHECK(ln_get_data(tau->n3) == n3d, "n3 data should be unchanged");
}

TEST_F(node_insertion, insert_before_NULL)
{
	REQUIRE(
		ln_insert_before(NULL, tau->n2) == tau->n2,
		"the newly inserted node should be n2"
	);

	CHECK(ln_get_next(tau->n2) == NULL, "n2 next should be unchanged");
	CHECK(ln_get_prev(tau->n2) == NULL, "n2 previous should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "data should be unchanged");
}

TEST_F(node_insertion, insert_NULL_before)
{
	REQUIRE(
		ln_insert_before(tau->n1, NULL) == tau->n1,
		"the newly inserted node should be n1"
	);

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be unchanged");
	CHECK(ln_get_prev(tau->n2) == NULL, "n2 previous should be unchanged");
	CHECK(ln_get_data(tau->n1) == n1d, "data should be unchanged");
}

TEST_F(node_insertion, insert_before_1_node)
{
	REQUIRE(
		ln_insert_before(tau->n1, tau->n2) == tau->n2,
		"the newly inserted node should be n2"
	);

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be unchanged");
	CHECK(ln_get_next(tau->n2) == tau->n1, "n2 next should be n1");

	CHECK(ln_get_prev(tau->n1) == tau->n2, "n2 previous should be n1");
	CHECK(ln_get_prev(tau->n2) == NULL, "n1 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
}

TEST_F(node_insertion, insert_before_2_nodes)
{
	linked_node *output = ln_insert_before(tau->n1, tau->n2);

	REQUIRE(output == tau->n2, "the newly inserted node should be n2");
	output = ln_insert_before(tau->n2, tau->n3);
	REQUIRE(output == tau->n3, "the newly inserted node should be n3");

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be unchanged");
	CHECK(ln_get_next(tau->n2) == tau->n1, "n2 next should be n1");
	CHECK(ln_get_next(tau->n3) == tau->n2, "n3 next should be n2");

	CHECK(ln_get_prev(tau->n1) == tau->n2, "n1 previous should be n2");
	CHECK(ln_get_prev(tau->n2) == tau->n3, "n2 previous should be n3");
	CHECK(ln_get_prev(tau->n3) == NULL, "n3 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
	CHECK(ln_get_data(tau->n3) == n3d, "n3 data should be unchanged");
}

TEST_F(node_insertion, insert_before_between_2_nodes)
{
	linked_node *output = ln_insert_before(tau->n1, tau->n3);

	REQUIRE(output == tau->n3, "the newly inserted node should be n3");
	output = ln_insert_before(tau->n1, tau->n2);
	REQUIRE(output == tau->n2, "the newly inserted node should be n2");

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be unchanged");
	CHECK(ln_get_next(tau->n2) == tau->n1, "n2 next should be n1");
	CHECK(ln_get_next(tau->n3) == tau->n2, "n3 next should be n2");

	CHECK(ln_get_prev(tau->n1) == tau->n2, "n1 previous should be n2");
	CHECK(ln_get_prev(tau->n2) == tau->n3, "n2 previous should be n3");
	CHECK(ln_get_prev(tau->n3) == NULL, "n3 previous should be unchanged");

	CHECK(ln_get_data(tau->n1) == n1d, "n1 data should be unchanged");
	CHECK(ln_get_data(tau->n2) == n2d, "n2 data should be unchanged");
	CHECK(ln_get_data(tau->n3) == n3d, "n3 data should be unchanged");
}

/*######################################################################*/
/*######################################################################*/

struct node_deletion
{
	linked_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_deletion)
{
	tau->n1 = ln_new((char *const)n1d, NULL);
	tau->n2 = ln_new((char *const)n2d, NULL);
	tau->n3 = ln_new((char *const)n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->n3);
	}

	REQUIRE(tau->n1 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n2 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n3 != NULL, "ln_new() should return non-null pointer");

	/*n1->n2->n3*/
	ln_insert_after(ln_insert_after(tau->n1, tau->n2), tau->n3);
}

TEST_F_TEARDOWN(node_deletion)
{
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(node_deletion, remove_NULL)
{
	CHECK(ln_remove(NULL) == NULL, "nothing should be done");
}

TEST(node_deletion, remove_n)
{
	linked_node *n1 = ln_new(original, NULL);

	CHECK(
		ln_remove(n1) == original, "node should be freed and its data returned"
	);
}

TEST_F(node_deletion, remove_first_node_in_list)
{
	CHECK(ln_remove(tau->n1) == n1d, "data returned should be n1's");
	tau->n1 = NULL;

	CHECK(ln_get_next(tau->n2) == tau->n3, "n2 next should be n3");
	CHECK(ln_get_next(tau->n3) == NULL, "n3 next should be unchanged");

	CHECK(ln_get_prev(tau->n2) == NULL, "n2 previous should be NULL");
	CHECK(ln_get_prev(tau->n3) == tau->n2, "n3 previous should be n2");
}

TEST_F(node_deletion, remove_middle_node_in_list)
{
	CHECK(ln_remove(tau->n2) == n2d, "data returned should be n2's");
	tau->n2 = NULL;

	CHECK(ln_get_next(tau->n1) == tau->n3, "n1 next should be n3");
	CHECK(ln_get_next(tau->n3) == NULL, "n3 next should be unchanged");

	CHECK(ln_get_prev(tau->n1) == NULL, "n1 previous should be unchanged");
	CHECK(ln_get_prev(tau->n3) == tau->n1, "n3 previous should be n1");
}

TEST_F(node_deletion, remove_last_node_in_list)
{
	CHECK(ln_remove(tau->n3) == n3d, "data returned should be n3's");
	tau->n3 = NULL;

	CHECK(ln_get_next(tau->n1) == tau->n2, "n1 next should be n2");
	CHECK(ln_get_next(tau->n2) == NULL, "n2 next should be NULL");

	CHECK(ln_get_prev(tau->n1) == NULL, "n1 previous should be unchanged");
	CHECK(ln_get_prev(tau->n2) == tau->n1, "n2 previous should be n1");
}

TEST_F(node_deletion, remove_first_node_of_2_in_list)
{
	ln_remove(tau->n3);
	tau->n3 = NULL;

	CHECK(ln_remove(tau->n1) == n1d, "data returned should be n1's");
	tau->n1 = NULL;

	CHECK(ln_get_next(tau->n2) == NULL, "n2 next should be unchanged");
	CHECK(ln_get_prev(tau->n2) == NULL, "n2 next should be NULL");
}

TEST_F(node_deletion, remove_second_node_of_2_in_list)
{
	ln_remove(tau->n3);
	tau->n3 = NULL;

	CHECK(ln_remove(tau->n2) == n2d, "data returned should be n2's");
	tau->n2 = NULL;

	CHECK(ln_get_next(tau->n1) == NULL, "n1 next should be NULL");
	CHECK(ln_get_prev(tau->n1) == NULL, "n1 next should be unchanged");
}

TEST(node_deletion, clear_NULL)
{
	/*Should produce no leaks or UB.*/
	dll_clear(NULL, NULL);
}

TEST_F(node_deletion, clear_dll)
{
	/*Should produce no leaks or UB.*/
	dll_clear(tau->n1, NULL);
	tau->n1 = NULL;
	tau->n2 = NULL;
	tau->n3 = NULL;
}

/*######################################################################*/
/*######################################################################*/

static char string_stream[MAX_STRING_LENGTH] = {'X'};

/**
 * print_string - prints a string.
 * @stream: stream to print to.
 * @str: pointer to the string.
 *
 * Return: same as fprintf.
 */
static int print_string(FILE *stream, void const *const str)
{
	char const *const s = str;

	if (!s)
		return (fprintf(stream, "NULL"));

	return (fprintf(stream, "%s", s));
}

struct print_dll
{
	linked_node *n1, *n2, *n3;
	FILE *stream;
};

TEST_F_SETUP(print_dll)
{
	string_stream[MAX_STRING_LENGTH - 1] = 0;
	tau->stream = fmemopen(string_stream, MAX_STRING_LENGTH, "w");
	REQUIRE(tau->stream != NULL, "failed to open memstream");

	tau->n1 = ln_new((char *const)n1d, NULL);
	tau->n2 = ln_new((char *const)n2d, NULL);
	tau->n3 = ln_new((char *const)n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->n3);
	}

	REQUIRE(tau->n1 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n2 != NULL, "ln_new() should return non-null pointer");
	REQUIRE(tau->n3 != NULL, "ln_new() should return non-null pointer");
}

TEST_F_TEARDOWN(print_dll)
{
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
	REQUIRE(fclose(tau->stream) == 0, "failed to close memstream");
	memset(string_stream, 'X', MAX_STRING_LENGTH - 1);
}

TEST_F(print_dll, print_null_arguments)
{
	CHECK(
		dll_print(NULL, NULL, NULL) < 0,
		"stream pointer and head pointer are required"
	);
	CHECK(
		dll_print(tau->stream, NULL, NULL) < 0,
		"stream pointer and head pointer are required"
	);
	CHECK(
		dll_print(NULL, tau->n1, NULL) < 0,
		"stream pointer and head pointer are required"
	);
}

TEST_F(print_dll, print_one_node_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%p\n", (void *)n1d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, print_one_node)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%s\n", n1d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, print_two_nodes_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_after(tau->n1, tau->n2);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p\n", (void *)n1d,
			(void *)n2d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, print_two_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_after(tau->n1, tau->n2);
	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%s <--> %s\n", n1d, n2d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, print_three_nodes_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_after(ln_insert_after(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p <--> %p\n", (void *)n1d,
			(void *)n2d, (void *)n3d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, print_three_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_after(ln_insert_after(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%s <--> %s <--> %s\n", n1d, n2d, n3d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

/*######################################################################*/
/*######################################################################*/

TEST_F(print_dll, printrev_null_arguments)
{
	CHECK(
		dll_print_reversed(NULL, NULL, NULL) < 0,
		"stream pointer and head pointer are required"
	);
	CHECK(
		dll_print_reversed(tau->stream, NULL, NULL) < 0,
		"stream pointer and head pointer are required"
	);
	CHECK(
		dll_print_reversed(NULL, tau->n1, NULL) < 0,
		"stream pointer and head pointer are required"
	);
}

TEST_F(print_dll, printrev_one_node_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%p\n", (void *)n1d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, printrev_one_node)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%s\n", n1d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, printrev_two_nodes_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_before(tau->n1, tau->n2);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p\n", (void *)n1d,
			(void *)n2d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, printrev_two_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_before(tau->n1, tau->n2);
	REQUIRE(
		snprintf(expected, MAX_STRING_LENGTH, "%s <--> %s\n", n1d, n2d) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, printrev_three_nodes_no_print_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_before(ln_insert_before(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p <--> %p\n", (void *)n1d,
			(void *)n2d, (void *)n3d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, NULL) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}

TEST_F(print_dll, printrev_three_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	ln_insert_before(ln_insert_before(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%s <--> %s <--> %s\n", n1d, n2d, n3d
		) > 0,
		"failed to print to buffer"
	);

	CHECK(
		dll_print_reversed(tau->stream, tau->n1, print_string) > 0,
		"number of bytes printed should be > 0"
	);
	REQUIRE(fflush(tau->stream) == 0, "failed to flush buffer");

	CHECK_STREQ(
		string_stream, expected, "output and expected should be equal"
	);
}
