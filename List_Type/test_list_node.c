#include <stdlib.h> /* *alloc */
#include <string.h> /* memcpy */

#include "list_node.h"
#include "list_type_structs.h"
#include "tau/tau.h"

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

/* ###################################################################### */
/* ########################## node_creation ############################# */
/* ###################################################################### */

struct node_creation
{
	list_node *output;
};

TEST_F_SETUP(node_creation) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(node_creation) { tau->output = lstnode_del(tau->output); }

TEST_F(node_creation, new_NULL_NULL_returns_NULL)
{
	tau->output = lstnode_new(NULL, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_d_NULL_returns_node_with_unchanged_d_pointer)
{
	char input[] = "input";
	tau->output = lstnode_new(input, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->output), input);
	CHECK_PTR_EQ(lstnode_get_next(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_NULL_f_returns_NULL)
{
	tau->output = lstnode_new(NULL, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_d_f_returns_correct_node)
{
	char input[] = "input";
	tau->output = lstnode_new(input, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);
	char *const output_data = lstnode_get_data(tau->output);

	CHECK_PTR_NE(output_data, input);
	CHECK_STREQ(output_data, input);
	CHECK_PTR_EQ(lstnode_get_next(tau->output), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->output), NULL);
	free(output_data);
}

TEST(node_creation, new_d_faildup_returns_NULL)
{
	char input[] = "input";

	REQUIRE_PTR_EQ(lstnode_new(input, fail_dup), NULL);
}

/* ###################################################################### */
/* ########################## data_get ################################## */
/* ###################################################################### */

struct data_get
{
	list_node *output;
};

TEST_F_SETUP(data_get) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(data_get) { tau->output = lstnode_del(tau->output); }

TEST_F(data_get, get_data_returns_correct_data)
{
	char input[] = "input";
	tau->output = lstnode_new(input, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->output), input);
}

TEST_F(data_get, get_data_returns_correct_duplicated_data)
{
	char input[] = "input";
	tau->output = lstnode_new(input, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);
	char *const curr_data = lstnode_get_data(tau->output);

	CHECK(curr_data != input);
	CHECK_STREQ(curr_data, input);
	free(curr_data);
}

/* ###################################################################### */
/* ############################# data_swap ############################## */
/* ###################################################################### */

struct data_swap
{
	list_node n1, n2;
};

TEST_F_SETUP(data_swap)
{
	lstnode_set_data(&tau->n1, n1d);
	lstnode_set_data(&tau->n2, n2d);
}

TEST_F_TEARDOWN(data_swap) { (void)tau; }

TEST_F(data_swap, swap_NULLs)
{
	CHECK_PTR_EQ(lstnode_swap(NULL, NULL), NULL);
	CHECK_PTR_EQ(lstnode_swap(&tau->n1, NULL), NULL);
	CHECK_PTR_EQ(lstnode_swap(NULL, &tau->n1), &tau->n1);
}

TEST_F(data_swap, swap_n_d_NULL)
{
	CHECK_PTR_EQ(lstnode_get_data(lstnode_swap(&tau->n1, &tau->n2)), n1d);
	CHECK_PTR_EQ(lstnode_get_data(&tau->n1), n2d);
}

/* ###################################################################### */
/* ############################ node_insertion ########################## */
/* ###################################################################### */

struct node_insertion
{
	list_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_insertion)
{
	tau->n1 = lstnode_new(n1d, NULL);
	tau->n2 = lstnode_new(n2d, NULL);
	tau->n3 = lstnode_new(n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		tau->n1 = lstnode_del(tau->n1);
		tau->n2 = lstnode_del(tau->n2);
		tau->n3 = lstnode_del(tau->n3);
	}

	REQUIRE(tau->n1 != NULL);
	REQUIRE(tau->n2 != NULL);
	REQUIRE(tau->n3 != NULL);
}

TEST_F_TEARDOWN(node_insertion)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
}

TEST(node_insertion, insert_NULLs)
{
	CHECK_PTR_EQ(lstnode_insert_after(NULL, NULL), NULL);
	CHECK(lstnode_insert_before(NULL, NULL) == NULL);
}

TEST_F(node_insertion, insert_after_NULL)
{
	REQUIRE_PTR_EQ(lstnode_insert_after(NULL, tau->n2), tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_NULL_after)
{
	REQUIRE_PTR_EQ(lstnode_insert_after(tau->n1, NULL), NULL);

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
}

TEST_F(node_insertion, insert_after_1_node)
{
	REQUIRE_PTR_EQ(lstnode_insert_after(tau->n1, tau->n2), tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_after_2_nodes)
{
	list_node *output = lstnode_insert_after(tau->n1, tau->n2);

	REQUIRE(output == tau->n2);
	output = lstnode_insert_after(tau->n2, tau->n3);
	REQUIRE(output == tau->n3);

	CHECK_PTR_EQ(lstnode_get_next(tau->n3), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(lstnode_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), tau->n2);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_after_between_2_nodes)
{
	list_node *output = lstnode_insert_after(tau->n1, tau->n3);

	REQUIRE(output == tau->n3);
	output = lstnode_insert_after(tau->n1, tau->n2);
	REQUIRE(output == tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n3), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(lstnode_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), tau->n2);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_before_NULL)
{
	REQUIRE(lstnode_insert_before(NULL, tau->n2) == tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_NULL_before)
{
	REQUIRE_PTR_EQ(lstnode_insert_before(tau->n1, NULL), NULL);

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
}

TEST_F(node_insertion, insert_before_1_node)
{
	REQUIRE(lstnode_insert_before(tau->n1, tau->n2) == tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n1);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_before_2_nodes)
{
	list_node *output = lstnode_insert_before(tau->n1, tau->n2);

	REQUIRE(output == tau->n2);
	output = lstnode_insert_before(tau->n2, tau->n3);
	REQUIRE(output == tau->n3);

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n1);
	CHECK_PTR_EQ(lstnode_get_next(tau->n3), tau->n2);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n3);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_before_between_2_nodes)
{
	list_node *output = lstnode_insert_before(tau->n1, tau->n3);

	REQUIRE(output == tau->n3);
	output = lstnode_insert_before(tau->n1, tau->n2);
	REQUIRE(output == tau->n2);

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n1);
	CHECK_PTR_EQ(lstnode_get_next(tau->n3), tau->n2);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n3);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), NULL);

	CHECK_PTR_EQ(lstnode_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->n3), n3d);
}

/* ###################################################################### */
/* ########################### node_deletion ############################ */
/* ###################################################################### */

struct node_deletion
{
	list_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_deletion)
{
	tau->n1 = lstnode_new(n1d, NULL);
	tau->n2 = lstnode_new(n2d, NULL);
	tau->n3 = lstnode_new(n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		tau->n1 = lstnode_del(tau->n1);
		tau->n2 = lstnode_del(tau->n2);
		tau->n3 = lstnode_del(tau->n3);
	}

	REQUIRE(tau->n1 != NULL);
	REQUIRE(tau->n2 != NULL);
	REQUIRE(tau->n3 != NULL);

	/* n1->n2->n3 */
	lstnode_insert_after(lstnode_insert_after(tau->n1, tau->n2), tau->n3);
}

TEST_F_TEARDOWN(node_deletion)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
}

TEST(node_deletion, remove_NULL) { CHECK_PTR_EQ(lstnode_del(NULL), NULL); }

TEST(node_deletion, remove_n)
{
	list_node *n1 = lstnode_new(original, NULL);
	void *const restrict data = lstnode_del(n1);

	CHECK_PTR_EQ(data, original);
}

TEST_F(node_deletion, remove_first_node_in_list)
{
	CHECK_PTR_EQ(lstnode_del(tau->n1), n1d);
	tau->n1 = NULL;

	CHECK_PTR_EQ(lstnode_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(lstnode_get_next(tau->n3), NULL);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), tau->n2);
}

TEST_F(node_deletion, remove_middle_node_in_list)
{
	CHECK_PTR_EQ(lstnode_del(tau->n2), n2d);
	tau->n2 = NULL;

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), tau->n3);
	CHECK_PTR_EQ(lstnode_get_next(tau->n3), NULL);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n3), tau->n1);
}

TEST_F(node_deletion, remove_last_node_in_list)
{
	CHECK_PTR_EQ(lstnode_del(tau->n3), n3d);
	tau->n3 = NULL;

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), tau->n2);
	CHECK_PTR_EQ(lstnode_get_next(tau->n2), NULL);

	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), tau->n1);
}

TEST_F(node_deletion, remove_first_node_of_2_in_list)
{
	lstnode_del(tau->n3);
	tau->n3 = NULL;

	CHECK_PTR_EQ(lstnode_del(tau->n1), n1d);
	tau->n1 = NULL;

	CHECK_PTR_EQ(lstnode_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n2), NULL);
}

TEST_F(node_deletion, remove_second_node_of_2_in_list)
{
	lstnode_del(tau->n3);
	tau->n3 = NULL;

	CHECK_PTR_EQ(lstnode_del(tau->n2), n2d);
	tau->n2 = NULL;

	CHECK_PTR_EQ(lstnode_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(lstnode_get_prev(tau->n1), NULL);
}

TEST(node_deletion, clear_NULL)
{
	/* Should produce no leaks or UB. */
	linked_list_del(NULL, NULL);
}

TEST_F(node_deletion, clear_ll)
{
	/* Should produce no leaks or UB. */
	linked_list_del(tau->n1, NULL);
	tau->n1 = NULL;
	tau->n2 = NULL;
	tau->n3 = NULL;
}

/* ###################################################################### */
/* ############################# print_ll ############################### */
/* ###################################################################### */

/**
 * format_string - duplicates a string.
 * @str: pointer to the string.
 *
 * Return: pointer to the duplicated string, NULL on failure.
 */
static char *format_string(void const *const str)
{
	char const *restrict s = str;
	char *restrict out_str = NULL;

	if (s)
		out_str = malloc(strlen(s) + 1);
	else
	{
		out_str = malloc(sizeof("NULL"));
		s = "NULL";
	}

	if (out_str)
		strcpy(out_str, s);

	return (out_str);
}

struct print_ll
{
	list_node *restrict n1, *restrict n2, *restrict n3;
	char *restrict output;
};

TEST_F_SETUP(print_ll)
{
	tau->output = NULL;
	tau->n1 = lstnode_new(n1d, NULL);
	tau->n2 = lstnode_new(n2d, NULL);
	tau->n3 = lstnode_new(n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->n3);
	}

	REQUIRE(tau->n1 && tau->n2 && tau->n3);
}

TEST_F_TEARDOWN(print_ll)
{
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
	free(tau->output);
}

TEST(print_ll, print_null_arguments)
{
	CHECK_PTR_EQ(linked_list_tostr(NULL, NULL), NULL);
	CHECK_PTR_EQ(linked_list_tostr(NULL, format_string), NULL);
}

TEST_F(print_ll, print_one_node_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%p", (void *)n1d) > 0);

	tau->output = linked_list_tostr(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, print_one_node)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%s", n1d) > 0);

	tau->output = linked_list_tostr(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, print_two_nodes_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_after(tau->n1, tau->n2);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p", (void *)n1d, (void *)n2d
		) > 0
	);

	tau->output = linked_list_tostr(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, print_two_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_after(tau->n1, tau->n2);
	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%s <--> %s", n1d, n2d) > 0);

	tau->output = linked_list_tostr(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, print_three_nodes_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_after(lstnode_insert_after(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p <--> %p", (void *)n1d,
			(void *)n2d, (void *)n3d
		) > 0
	);

	tau->output = linked_list_tostr(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, print_three_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_after(lstnode_insert_after(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%s <--> %s <--> %s", n1d, n2d, n3d
		) > 0
	);

	tau->output = linked_list_tostr(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

/* ###################################################################### */
/* ###################################################################### */

TEST(print_ll, printrev_null_arguments)
{
	CHECK_PTR_EQ(linked_list_tostr_reversed(NULL, NULL), NULL);
	CHECK_PTR_EQ(linked_list_tostr_reversed(NULL, format_string), NULL);
}

TEST_F(print_ll, printrev_one_node_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%p", (void *)n1d) > 0);

	tau->output = linked_list_tostr_reversed(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, printrev_one_node)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%s", n1d) > 0);

	tau->output = linked_list_tostr_reversed(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, printrev_two_nodes_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_before(tau->n1, tau->n2);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p", (void *)n1d, (void *)n2d
		) > 0
	);

	tau->output = linked_list_tostr_reversed(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, printrev_two_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_before(tau->n1, tau->n2);
	REQUIRE(snprintf(expected, MAX_STRING_LENGTH, "%s <--> %s", n1d, n2d) > 0);

	tau->output = linked_list_tostr_reversed(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, printrev_three_nodes_no_stringify_function)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_before(lstnode_insert_before(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%p <--> %p <--> %p", (void *)n1d,
			(void *)n2d, (void *)n3d
		) > 0
	);

	tau->output = linked_list_tostr_reversed(tau->n1, NULL);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}

TEST_F(print_ll, printrev_three_nodes)
{
	char expected[MAX_STRING_LENGTH] = {'\0'};

	lstnode_insert_before(lstnode_insert_before(tau->n1, tau->n2), tau->n3);
	REQUIRE(
		snprintf(
			expected, MAX_STRING_LENGTH, "%s <--> %s <--> %s", n1d, n2d, n3d
		) > 0
	);

	tau->output = linked_list_tostr_reversed(tau->n1, format_string);
	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_STREQ(tau->output, expected);
}
