#include <stdlib.h> /* free */

#include "deque.h"
#include "list_node.h"
#include "list_type_structs.h"
#include "tau/tau.h"

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
	deque *const dq = dq_new();

	REQUIRE(dq, "dq_new() returns non-null");
	CHECK(dq->len == 0);
	CHECK(dq->head == NULL);
	CHECK(dq->tail == NULL);
	free(dq);
}

/* ###################################################################### */
/* ###################################################################### */

struct adding_items
{
	list_node *n1, *n2, *n3;
	deque *dq;
};

TEST_F_SETUP(adding_items)
{
	tau->dq = dq_new();
	REQUIRE(tau->dq, "dq_new() returns non-null");
}

TEST_F_TEARDOWN(adding_items)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_del(tau->dq, NULL);
}

TEST(adding_items, pushtail_null_q_returns_null)
{
	CHECK(dq_push_tail(NULL, NULL, NULL) == NULL);
	CHECK(dq_push_tail(NULL, n1d, NULL) == NULL);
	CHECK(
		dq_push_tail(NULL, NULL, dup_str) == NULL
	);
	CHECK(
		dq_push_tail(NULL, n1d, dup_str) == NULL
	);
}

TEST(adding_items, pushhead_null_q_returns_null)
{
	CHECK(dq_push_head(NULL, NULL, NULL) == NULL);
	CHECK(dq_push_head(NULL, n1d, NULL) == NULL);
	CHECK(
		dq_push_head(NULL, NULL, dup_str) == NULL
	);
	CHECK(
		dq_push_head(NULL, n1d, dup_str) == NULL
	);
}

TEST_F(adding_items, pushtail_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_push_tail(tau->dq, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), NULL);
}

TEST_F(adding_items, pushhead_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_push_head(tau->dq, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), NULL);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), NULL);
}

TEST_F(adding_items, pushtail_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_STREQ((char *)lstnode_get_data(tau->dq->head), n1d);
	CHECK_STREQ((char *)lstnode_get_data(tau->dq->tail), n1d);

	free(lstnode_set_data(tau->dq->head, NULL));
}

TEST_F(adding_items, pushhead_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_push_head(tau->dq, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_STREQ((char *)lstnode_get_data(tau->dq->head), n1d);
	CHECK_STREQ((char *)lstnode_get_data(tau->dq->tail), n1d);

	free(lstnode_set_data(tau->dq->head, NULL));
}

TEST_F(adding_items, pushtail_3_nodes)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);

	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);

	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushhead_3_nodes)
{
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);

	tau->n2 = dq_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);

	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushtail_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_tail() should return NULL on failure");
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(adding_items, pushhead_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_push_head(tau->dq, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_head() should return NULL on failure");
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(adding_items, pushtail_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_tail() should return NULL on failure");
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushhead_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_head(tau->dq, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_head() should return NULL on failure");
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushhead_pushtail_pushtail)
{
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);

	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushhead_pushtail_pushhead)
{
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(adding_items, pushhead_pushhead_pushtail)
{
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushtail_pushhead_pushtail)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);

	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushtail_pushhead_pushhead)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushtail_pushtail_pushhead)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

/* ###################################################################### */
/* ########################## removing_items ############################ */
/* ###################################################################### */

struct removing_items
{
	list_node *n1, *n2, *n3;
	deque *dq;
};

TEST_F_SETUP(removing_items)
{
	tau->dq = dq_new();
	REQUIRE(tau->dq, "dq_new() returns non-null");
}

TEST_F_TEARDOWN(removing_items)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_del(tau->dq, NULL);
}

TEST(removing_items, poptail_null_should_return_NULL)
{
	CHECK(dq_pop_tail(NULL) == NULL);
}

TEST_F(removing_items, poptail_empty_q_changes_nothing)
{
	CHECK(dq_pop_tail(tau->dq) == NULL);

	CHECK(tau->dq->len == 0);
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(removing_items, poptail_1_node_from_1)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ((char *)dq_pop_tail(tau->dq), n1d);
	tau->n1 = NULL;
	CHECK(tau->dq->len == 0);
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(removing_items, poptail_1_node_from_2)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ((char *)dq_pop_tail(tau->dq), n2d);
	tau->n2 = NULL;
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(removing_items, poptail_1_node_from_3)
{
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ((char *)dq_pop_tail(tau->dq), n3d);
	tau->n3 = NULL;
	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

/* ###################################################################### */
/* ##################### add_and_remove_pushtail ######################### */
/* ###################################################################### */

struct add_and_remove_pushtail
{
	list_node *n1, *n2, *n3, *n4;
	deque *dq;
};

TEST_F_SETUP(add_and_remove_pushtail)
{
	tau->dq = dq_new();
	tau->n1 = dq_push_tail(tau->dq, n1d, NULL);
	tau->n2 = dq_push_tail(tau->dq, n2d, NULL);
	if (!tau->dq || !tau->n1 || !tau->n2)
	{
		tau->n1 = lstnode_del(tau->n1);
		tau->n2 = lstnode_del(tau->n2);
		tau->dq->head = NULL;
		tau->dq->tail = NULL;
		tau->dq = dq_del(tau->dq, NULL);
	}

	REQUIRE(tau->dq, "dq_new() returns non-null");
	REQUIRE(tau->n1, "push_tail() returns non-null");
	REQUIRE(tau->n2, "push_tail() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pushtail)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
	tau->n4 = lstnode_del(tau->n4);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_del(tau->dq, NULL);
}

TEST_F(add_and_remove_pushtail, pushtail_poptail)
{
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_tail(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pushtail_pophead)
{
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushtail, pushhead_pophead)
{
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_head(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pushhead_poptail)
{
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushtail, poptail_pushtail)
{
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushtail, poptail_pushhead)
{
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushtail, pophead_pushhead)
{
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pophead_pushtail)
{
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

/* ###################################################################### */
/* ###################### add_and_remove_pushhead ######################## */
/* ###################################################################### */

struct add_and_remove_pushhead
{
	list_node *n1, *n2, *n3, *n4;
	deque *dq;
};

TEST_F_SETUP(add_and_remove_pushhead)
{
	tau->dq = dq_new();
	tau->n2 = dq_push_head(tau->dq, n2d, NULL);
	tau->n1 = dq_push_head(tau->dq, n1d, NULL);
	if (!tau->dq || !tau->n1 || !tau->n2)
	{
		tau->n1 = lstnode_del(tau->n1);
		tau->n2 = lstnode_del(tau->n2);
		tau->dq->head = NULL;
		tau->dq->tail = NULL;
		tau->dq = dq_del(tau->dq, NULL);
	}

	REQUIRE(tau->dq, "dq_new() returns non-null");
	REQUIRE(tau->n2, "push_head() returns non-null");
	REQUIRE(tau->n1, "push_head() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pushhead)
{
	tau->n1 = lstnode_del(tau->n1);
	tau->n2 = lstnode_del(tau->n2);
	tau->n3 = lstnode_del(tau->n3);
	tau->n4 = lstnode_del(tau->n4);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_del(tau->dq, NULL);
}

TEST_F(add_and_remove_pushhead, pushtail_poptail)
{
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_tail(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pushtail_pophead)
{
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushhead, pushhead_pophead)
{
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_head(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pushhead_poptail)
{
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushhead, poptail_pushtail)
{
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushhead, poptail_pushhead)
{
	CHECK(dq_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushhead, pophead_pushhead)
{
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pophead_pushtail)
{
	CHECK(dq_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(lstnode_get_data(tau->dq->tail), n3d);
}

/* ###################################################################### */
/* ######################### deleting_deque ############################# */
/* ###################################################################### */

TEST(deleting_deque, delete_should_clear_all_items)
{
	deque *dq = dq_new();
	list_node *n1 = dq_push_tail(dq, n1d, NULL);
	list_node *n2 = dq_push_tail(dq, n2d, NULL);
	list_node *n3 = dq_push_tail(dq, n3d, NULL);

	if (!dq || !n1 || !n2 || !n3)
	{
		lstnode_del(n1);
		n1 = NULL;
		lstnode_del(n2);
		n2 = NULL;
		lstnode_del(n3);
		n3 = NULL;
		dq->head = NULL;
		dq->tail = NULL;
		dq = dq_del(dq, NULL);
	}

	REQUIRE((dq && n1 && n2 && n3));

	dq = dq_del(dq, NULL);
}

TEST(deleting_deque, delete_f_should_clear_all_items)
{
	deque *dq = dq_new();
	list_node *n1 = dq_push_tail(dq, n1d, dup_str);
	list_node *n2 = dq_push_tail(dq, n2d, dup_str);
	list_node *n3 = dq_push_tail(dq, n3d, dup_str);

	if (!dq || !n1 || !n2 || !n3)
	{
		free(lstnode_del(n1));
		n1 = NULL;
		free(lstnode_del(n2));
		n2 = NULL;
		free(lstnode_del(n3));
		n3 = NULL;
		dq->head = NULL;
		dq->tail = NULL;
		dq = dq_del(dq, NULL);
	}

	REQUIRE((dq && n1 && n2 && n3));

	dq = dq_del(dq, free);
}

/* ###################################################################### */
/* ############################## dqfa ################################## */
/* ###################################################################### */

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
	deque *dq = dq_from_array(arr, arr_len, sizeof(*arr), NULL, NULL);

	REQUIRE(dq, "dq_from_array() should return non-null pointer");

	CHECK(
		dq->len == arr_len, "there should be %zu items in the deque", arr_len
	);
	CHECK(*(long long int *)dq_pop_tail(dq) == arr[4]);
	CHECK(*(long long int *)dq_pop_tail(dq) == arr[3]);
	CHECK(*(long long int *)dq_pop_tail(dq) == arr[2]);
	CHECK(*(long long int *)dq_pop_tail(dq) == arr[1]);
	CHECK(*(long long int *)dq_pop_tail(dq) == arr[0]);
	CHECK(dq->len == 0);
	CHECK(dq->head == NULL);
	CHECK(dq->tail == NULL);

	dq = dq_del(dq, NULL);
}
