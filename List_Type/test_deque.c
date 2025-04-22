#include "deque.h"
#include "tau/tau.h"
#include <stdlib.h> /* free */

#define MAX_STRING_LENGTH 256U

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

/**
 * dup_llint - make a copy of an int pointer.
 * @n: pointer to the int.
 *
 * Return: pointer to a new int.
 */
static void *dup_llint(void const *const num)
{
	long long int const *const n = num;

	if (!n)
		return (NULL);

	long long int *const ptr = calloc(1, sizeof(*ptr));

	if (ptr)
		*ptr = *n;

	return (ptr);
}

TAU_MAIN()

TEST(deque_creation, new_returns_empty_q)
{
	deque *const q = dq_new();

	REQUIRE(q, "dq_new() returns non-null");
	CHECK(dq_len(q) == 0, "length should be 0");
	CHECK(dq_peek_head(q) == NULL, "head of the deque should be empty");
	CHECK(dq_peek_tail(q) == NULL, "tail of the deque should be empty");
	free(q);
}

/*######################################################################*/
/*######################################################################*/

struct adding_items
{
	double_link_node *n1, *n2, *n3;
	deque *q;
};

TEST_F_SETUP(adding_items)
{
	tau->q = dq_new();
	REQUIRE(tau->q, "dq_new() returns non-null");
}

TEST_F_TEARDOWN(adding_items)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(adding_items, pshtail_null_q_returns_null)
{
	CHECK(dq_push_tail(NULL, NULL, NULL) == NULL, "pointer to deque required");
	CHECK(dq_push_tail(NULL, n1d, NULL) == NULL, "pointer to deque required");
	CHECK(
		dq_push_tail(NULL, NULL, dup_str) == NULL, "pointer to deque required"
	);
	CHECK(
		dq_push_tail(NULL, n1d, dup_str) == NULL, "pointer to deque required"
	);
}

TEST(adding_items, pshhead_null_q_returns_null)
{
	CHECK(dq_push_head(NULL, NULL, NULL) == NULL, "pointer to deque required");
	CHECK(dq_push_head(NULL, n1d, NULL) == NULL, "pointer to deque required");
	CHECK(
		dq_push_head(NULL, NULL, dup_str) == NULL, "pointer to deque required"
	);
	CHECK(
		dq_push_head(NULL, n1d, dup_str) == NULL, "pointer to deque required"
	);
}

TEST_F(adding_items, pshtail_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_push_tail(tau->q, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == NULL, "head should point to the added node");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should point to the added node");
}

TEST_F(adding_items, pshhead_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_push_head(tau->q, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == NULL, "head should point to the added node");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should point to the added node");
}

TEST_F(adding_items, pshtail_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_push_tail(tau->q, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK_STREQ((char *)dq_peek_head(tau->q), n1d, "head should be n1");
	CHECK_STREQ((char *)dq_peek_tail(tau->q), n1d, "tail should be n2");

	free(dq_peek_head(tau->q));
}

TEST_F(adding_items, pshhead_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_push_head(tau->q, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK_STREQ((char *)dq_peek_head(tau->q), n1d, "head should be n1");
	CHECK_STREQ((char *)dq_peek_tail(tau->q), n1d, "tail should be n2");

	free(dq_peek_head(tau->q));
}

TEST_F(adding_items, pshtail_3_nodes)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should point to the added node");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should point to the added node");

	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 2, "there should only be 2 items in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be n2");

	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be n3");
}

TEST_F(adding_items, pshhead_3_nodes)
{
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should point to the added node");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should point to the added node");

	tau->n2 = dq_push_head(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 2, "there should only be 2 items in the deque");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be n2");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be n1");

	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be n1");
}

TEST_F(adding_items, pshtail_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_push_tail(tau->q, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_tail() should return NULL on failure");
	CHECK(dq_peek_head(tau->q) == NULL, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should be unchanged");
}

TEST_F(adding_items, pshhead_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_push_head(tau->q, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_head() should return NULL on failure");
	CHECK(dq_peek_head(tau->q) == NULL, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should be unchanged");
}

TEST_F(adding_items, pshtail_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_tail() should return NULL on failure");
	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be unchanged");
}

TEST_F(adding_items, pshhead_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_head(tau->q, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_head() should return NULL on failure");
	CHECK(dq_len(tau->q) == 1, "there should only be 1 item in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be unchanged");
}

TEST_F(adding_items, pshhead_pshtail_pshtail)
{
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 2, "there should only be 2 items in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be n2");

	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be n3");
}

TEST_F(adding_items, pshhead_pshtail_pshhead)
{
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be n3");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be n2");
}

TEST_F(adding_items, pshhead_pshhead_pshtail)
{
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_head(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be n3");
}

TEST_F(adding_items, pshtail_pshhead_pshtail)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_head(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 2, "there should only be 2 items in the deque");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be n2");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be n1");

	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be n3");
}

TEST_F(adding_items, pshtail_pshhead_pshhead)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_head(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be n1");
}

TEST_F(adding_items, pshtail_pshtail_pshhead)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(dq_len(tau->q) == 3, "there should only be 3 items in the deque");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be n3");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be n2");
}

/*######################################################################*/
/*######################################################################*/

struct removing_items
{
	double_link_node *n1, *n2, *n3;
	deque *q;
};

TEST_F_SETUP(removing_items)
{
	tau->q = dq_new();
	REQUIRE(tau->q, "dq_new() returns non-null");
}

TEST_F_TEARDOWN(removing_items)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(removing_items, poptail_null_should_return_NULL)
{
	CHECK(dq_pop_tail(NULL) == NULL, "pointer to deque is required");
}

TEST_F(removing_items, poptail_empty_q_changes_nothing)
{
	CHECK(dq_pop_tail(tau->q) == NULL, "nothing should be done");

	CHECK(dq_len(tau->q) == 0, "deque should have 0 items");
	CHECK(dq_peek_head(tau->q) == NULL, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should be unchanged");
}

TEST_F(removing_items, poptail_1_node_from_1)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(
		(char *)dq_pop_tail(tau->q), n1d, "first node should have been removed"
	);
	tau->n1 = NULL;
	CHECK(dq_len(tau->q) == 0, "deque should have 0 items");
	CHECK(dq_peek_head(tau->q) == NULL, "head should be NULL");
	CHECK(dq_peek_tail(tau->q) == NULL, "tail should be NULL");
}

TEST_F(removing_items, poptail_1_node_from_2)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(
		(char *)dq_pop_tail(tau->q), n2d, "first node should have been removed"
	);
	tau->n2 = NULL;
	CHECK(dq_len(tau->q) == 1, "deque should have 1 item");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be unchanged");
}

TEST_F(removing_items, poptail_1_node_from_3)
{
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(
		(char *)dq_pop_tail(tau->q), n3d, "first node should have been removed"
	);
	tau->n3 = NULL;
	CHECK(dq_len(tau->q) == 2, "deque should have 2 items");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be n1");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be unchanged");
}

/*######################################################################*/
/*######################################################################*/

struct add_and_remove_pshtail
{
	double_link_node *n1, *n2, *n3, *n4;
	deque *q;
};

TEST_F_SETUP(add_and_remove_pshtail)
{
	tau->q = dq_new();
	tau->n1 = dq_push_tail(tau->q, n1d, NULL);
	tau->n2 = dq_push_tail(tau->q, n2d, NULL);
	if (!tau->q || !tau->n1 || !tau->n2)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->q);
	}

	REQUIRE(tau->q, "dq_new() returns non-null");
	REQUIRE(tau->n1, "push_tail() returns non-null");
	REQUIRE(tau->n2, "push_tail() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pshtail)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
	free(tau->n4);
}

TEST_F(add_and_remove_pshtail, pshtail_poptail)
{
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_tail(tau->q) == n3d, "n3 is removed from tail");
	tau->n3 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be updated to n2.");
}

TEST_F(add_and_remove_pshtail, pshtail_pophead)
{
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from head");
	tau->n1 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be updated to n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3");
}

TEST_F(add_and_remove_pshtail, pshhead_pophead)
{
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_head(tau->q) == n3d, "n3 is removed from head");
	tau->n3 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be updated to n1");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be unchanged");
}

TEST_F(add_and_remove_pshtail, pshhead_poptail)
{
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from tail");
	tau->n2 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be updated to n1");
}

TEST_F(add_and_remove_pshtail, poptail_pushtail)
{
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from the tail");
	tau->n2 = NULL;
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove_pshtail, poptail_pushhead)
{
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from the tail");
	tau->n2 = NULL;
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be updated to n1.");
}

TEST_F(add_and_remove_pshtail, pophead_pushhead)
{
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from the tail");
	tau->n1 = NULL;
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be updated to n2.");
}

TEST_F(add_and_remove_pshtail, pophead_pushtail)
{
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from the tail");
	tau->n1 = NULL;
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be updated to n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3.");
}

/*######################################################################*/
/*######################################################################*/

struct add_and_remove_pshhead
{
	double_link_node *n1, *n2, *n3, *n4;
	deque *q;
};

TEST_F_SETUP(add_and_remove_pshhead)
{
	tau->q = dq_new();
	tau->n2 = dq_push_head(tau->q, n2d, NULL);
	tau->n1 = dq_push_head(tau->q, n1d, NULL);
	if (!tau->q || !tau->n1 || !tau->n2)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->q);
	}

	REQUIRE(tau->q, "dq_new() returns non-null");
	REQUIRE(tau->n2, "push_head() returns non-null");
	REQUIRE(tau->n1, "push_head() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pshhead)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
	free(tau->n4);
}

TEST_F(add_and_remove_pshhead, pshtail_poptail)
{
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_tail(tau->q) == n3d, "n3 is removed from tail");
	tau->n3 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be updated to n2.");
}

TEST_F(add_and_remove_pshhead, pshtail_pophead)
{
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from head");
	tau->n1 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be updated to n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3");
}

TEST_F(add_and_remove_pshhead, pshhead_pophead)
{
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_head(tau->q) == n3d, "n3 is removed from head");
	tau->n3 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be updated to n1");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be unchanged");
}

TEST_F(add_and_remove_pshhead, pshhead_poptail)
{
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from tail");
	tau->n2 = NULL;

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be updated to n1");
}

TEST_F(add_and_remove_pshhead, poptail_pushtail)
{
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from the tail");
	tau->n2 = NULL;
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n1d, "head should be unchanged");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove_pshhead, poptail_pushhead)
{
	CHECK(dq_pop_tail(tau->q) == n2d, "n2 is removed from the tail");
	tau->n2 = NULL;
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n1d, "tail should be updated to n1.");
}

TEST_F(add_and_remove_pshhead, pophead_pushhead)
{
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from the tail");
	tau->n1 = NULL;
	tau->n3 = dq_push_head(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n3d, "head should be updated to n3");
	CHECK(dq_peek_tail(tau->q) == n2d, "tail should be updated to n2.");
}

TEST_F(add_and_remove_pshhead, pophead_pushtail)
{
	CHECK(dq_pop_head(tau->q) == n1d, "n1 is removed from the tail");
	tau->n1 = NULL;
	tau->n3 = dq_push_tail(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(dq_len(tau->q) == 2, "number of items is unchanged");
	CHECK(dq_peek_head(tau->q) == n2d, "head should be updated to n2");
	CHECK(dq_peek_tail(tau->q) == n3d, "tail should be updated to n3.");
}

/*######################################################################*/
/*######################################################################*/

TEST(deleting_deque, delete_should_clear_all_items)
{
	deque *q = dq_new();
	double_link_node *n1 = dq_push_tail(q, n1d, NULL);
	double_link_node *n2 = dq_push_tail(q, n2d, NULL);
	double_link_node *n3 = dq_push_tail(q, n3d, NULL);

	if (!q || !n1 || !n2 || !n3)
	{
		free(n1);
		free(n2);
		free(n3);
		free(q);
	}

	REQUIRE((q && n1 && n2 && n3), "failed to create deque");

	q = dq_delete(q, NULL);
}

TEST(deleting_deque, delete_f_should_clear_all_items)
{
	deque *q = dq_new();
	double_link_node *n1 = dq_push_tail(q, n1d, dup_str);
	double_link_node *n2 = dq_push_tail(q, n2d, dup_str);
	double_link_node *n3 = dq_push_tail(q, n3d, dup_str);

	if (!q || !n1 || !n2 || !n3)
	{
		free(dq_peek_head(q));
		free(n1);
		/*WARNING: data in n2 might not be freed.*/
		free(n2);
		free(dq_peek_tail(q));
		free(n3);
		free(q);
	}

	REQUIRE((q && n1 && n2 && n3), "failed to create deque");

	q = dq_delete(q, free);
}

/*######################################################################*/
/*######################################################################*/

TEST(dqfa, dqfa_invalid_args)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));

	CHECK(
		dq_from_array(NULL, 0, 0, NULL, NULL) == NULL,
		"data_array, len and type_size are required"
	);

	CHECK(
		dq_from_array(NULL, arr_len, 0, NULL, NULL) == NULL,
		"data_array and type_size are required"
	);
	CHECK(
		dq_from_array(NULL, 0, sizeof(*arr), NULL, NULL) == NULL,
		"data_array and len are required"
	);
	CHECK(
		dq_from_array(NULL, arr_len, sizeof(*arr), NULL, NULL) == NULL,
		"data_array is required"
	);

	CHECK(
		dq_from_array(arr, 0, 0, NULL, NULL) == NULL,
		"len and type_size are required"
	);
	CHECK(
		dq_from_array(arr, arr_len, 0, NULL, NULL) == NULL,
		"type_size is required"
	);
	CHECK(
		dq_from_array(arr, 0, sizeof(*arr), NULL, NULL) == NULL,
		"len is required"
	);

	CHECK(
		dq_from_array(arr, arr_len, sizeof(*arr), dup_llint, NULL) == NULL,
		"a dup function should always be accompanied by a delete function"
	);
}

TEST(dqfa, deque_from_array)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));
	deque *q = dq_from_array(arr, arr_len, sizeof(*arr), NULL, NULL);

	REQUIRE(q, "dq_from_array() should return non-null pointer");

	CHECK(
		dq_len(q) == arr_len, "there should be %zu items in the deque", arr_len
	);
	CHECK(*(long long int *)dq_pop_tail(q) == arr[4]);
	CHECK(*(long long int *)dq_pop_tail(q) == arr[3]);
	CHECK(*(long long int *)dq_pop_tail(q) == arr[2]);
	CHECK(*(long long int *)dq_pop_tail(q) == arr[1]);
	CHECK(*(long long int *)dq_pop_tail(q) == arr[0]);
	CHECK(dq_len(q) == 0, "there should now be 0 items in the deque");
	CHECK(dq_peek_head(q) == NULL, "head should be NULL");
	CHECK(dq_peek_tail(q) == NULL, "tail should be NULL");

	q = dq_delete(q, NULL);
}
