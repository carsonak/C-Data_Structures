#include "stack.h"
#include "tau/tau.h"
#include <string.h> /* memcpy */
#include <stdlib.h> /* free */

#define MAX_STRING_LENGTH ((unsigned int)256)

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

TEST(stack_creation, new_returns_empty_stack)
{
	stack *s = stk_new();

	REQUIRE(s, "new returns non-null");
	CHECK(stk_len(s) == 0, "there should be 0 items in the stack");
	CHECK(stk_peek(s) == NULL, "there should be no item at the top of the stack");

	free(s);
}

/*######################################################################*/
/*######################################################################*/

struct adding_items
{
	stack *s;
	single_link_node *n1, *n2, *n3;
};

TEST_F_SETUP(adding_items)
{
	tau->s = stk_new();
	REQUIRE(tau->s, "failed to create stack");
}

TEST_F_TEARDOWN(adding_items)
{
	free(tau->s);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(adding_items, push_invalid_args)
{
	CHECK(stk_push(NULL, NULL, NULL) == NULL, "pointer to stack is required");
	CHECK(stk_push(NULL, n1d, NULL) == NULL, "pointer to stack is required");
	CHECK(stk_push(NULL, NULL, dup_str) == NULL, "pointer to stack is required");
	CHECK(stk_push(NULL, n1d, dup_str) == NULL, "pointer to stack is required");
}

TEST_F(adding_items, push_one_faildup)
{
	REQUIRE(stk_push(tau->s, n1d, fail_dup) == NULL, "push() returns NULL on failure");
	CHECK(stk_len(tau->s) == 0, "number of items in the stack should be unchanged");
	CHECK(stk_peek(tau->s) == NULL, "top of the stack should be unchanged");
}

TEST_F(adding_items, push_2_fail_on_2)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	REQUIRE(tau->n1, "failed to push to stack");

	CHECK(stk_push(tau->s, n2d, fail_dup) == NULL, "push() returns NULL on failure");
	CHECK(stk_len(tau->s) == 1, "number of items in the stack should be unchanged");
	CHECK(stk_peek(tau->s) == n1d, "top of the stack should be unchanged");
}

TEST_F(adding_items, push_one)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	REQUIRE(tau->n1, "push() returns pointer to the added node");

	CHECK(stk_len(tau->s) == 1, "there should be only one item in the stack");
	CHECK(stk_peek(tau->s) == n1d, "top should point to the new node");
}

TEST_F(adding_items, push_one_dupfunc)
{
	tau->n1 = stk_push(tau->s, n1d, dup_str);
	REQUIRE(tau->n1, "push() returns pointer to the added node");

	CHECK(stk_len(tau->s) == 1, "there should be only one item in the stack");
	char *const d = stk_peek(tau->s);
	CHECK_STREQ(d, n1d, "top should point to the new node");
	free(d);
}

TEST_F(adding_items, push_three)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	tau->n2 = stk_push(tau->s, n2d, NULL);
	tau->n3 = stk_push(tau->s, n3d, NULL);
	REQUIRE(tau->n1, "push() returns pointer to the added node");
	REQUIRE(tau->n2, "push() returns pointer to the added node");
	REQUIRE(tau->n3, "push() returns pointer to the added node");

	CHECK(stk_len(tau->s) == 3, "there should be only 3 items in the stack");
	CHECK(stk_peek(tau->s) == n3d, "top should point to the n3");
}

/*######################################################################*/
/*######################################################################*/

struct push_and_pop
{
	stack *s;
	single_link_node *n1, *n2, *n3;
};

TEST_F_SETUP(push_and_pop)
{
	tau->s = stk_new();
	REQUIRE(tau->s, "failed to create stack");
}

TEST_F_TEARDOWN(push_and_pop)
{
	free(tau->s);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(push_and_pop, pop_null)
{
	CHECK(stk_pop(NULL) == NULL, "pointer to stack is required");
}

TEST_F(push_and_pop, pop_empty)
{
	CHECK(stk_pop(tau->s) == NULL, "nothing should happen");
}

TEST_F(push_and_pop, push_1_pop_1)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	REQUIRE(tau->n1, "failed to push node to stack");

	CHECK(stk_len(tau->s) == 1, "there should be 1 item in the stack");
	CHECK(stk_pop(tau->s) == n1d, "data at top should be n1d");
	tau->n1 = NULL;
	CHECK(stk_len(tau->s) == 0, "there should be 0 items in the stack");
}

TEST_F(push_and_pop, push_2_pop_2)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	REQUIRE(tau->n1, "failed to push node to stack");
	tau->n2 = stk_push(tau->s, n2d, NULL);
	REQUIRE(tau->n2, "failed to push node to stack");

	CHECK(stk_len(tau->s) == 2, "there should be 2 items in the stack");
	CHECK(stk_pop(tau->s) == n2d, "data at top should be n2d");
	tau->n2 = NULL;
	CHECK(stk_len(tau->s) == 1, "there should be 1 item in the stack");
	CHECK(stk_pop(tau->s) == n1d, "data at top should be n1d");
	tau->n1 = NULL;
	CHECK(stk_len(tau->s) == 0, "there should be 0 items in the stack");
}

TEST_F(push_and_pop, push_3_pop_3)
{
	tau->n1 = stk_push(tau->s, n1d, NULL);
	REQUIRE(tau->n1, "failed to push node to stack");
	tau->n2 = stk_push(tau->s, n2d, NULL);
	REQUIRE(tau->n2, "failed to push node to stack");
	tau->n3 = stk_push(tau->s, n3d, NULL);
	REQUIRE(tau->n3, "failed to push node to stack");

	CHECK(stk_len(tau->s) == 3, "there should be 3 items in the stack");
	CHECK(stk_pop(tau->s) == n3d, "data at top should be n3d");
	tau->n3 = NULL;
	CHECK(stk_len(tau->s) == 2, "there should be 2 items in the stack");
	CHECK(stk_pop(tau->s) == n2d, "data at top should be n2d");
	tau->n2 = NULL;
	CHECK(stk_len(tau->s) == 1, "there should be 1 item in the stack");
	CHECK(stk_pop(tau->s) == n1d, "data at top should be n1d");
	tau->n1 = NULL;
	CHECK(stk_len(tau->s) == 0, "there should be 0 items in the stack");
}

/*######################################################################*/
/*######################################################################*/

struct stack_deletion
{
	stack *s;
	single_link_node *n1, *n2, *n3;
};

TEST_F_SETUP(stack_deletion)
{
	tau->s = stk_new();
	tau->n1 = stk_push(tau->s, n1d, NULL);
	tau->n2 = stk_push(tau->s, n2d, NULL);
	tau->n3 = stk_push(tau->s, n3d, NULL);
	if (!tau->s || !tau->n1 || !tau->n2 || !tau->n3)
	{
		free(tau->s);
		free(tau->n1);
		free(tau->n2);
		free(tau->n3);
	}

	REQUIRE(tau->s, "failed to create stack");
	REQUIRE(tau->n1, "failed to push node to stack");
	REQUIRE(tau->n2, "failed to push node to stack");
	REQUIRE(tau->n3, "failed to push node to stack");
}

TEST_F_TEARDOWN(stack_deletion)
{
	free(tau->s);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(stack_deletion, delete_null)
{
	REQUIRE(stk_delete(NULL, NULL) == NULL, "pointer to stack is required");
	REQUIRE(stk_delete(NULL, free) == NULL, "pointer to stack is required");
}

TEST(stack_deletion, delete_empty)
{
	stack *s = stk_new();

	REQUIRE(s, "failed to create stack");
	CHECK(stk_delete(s, NULL) == NULL, "delete should handle empty stack");

	s = stk_new();

	REQUIRE(s, "failed to create stack");
	CHECK(stk_delete(s, free) == NULL, "delete should handle empty stack");
}

TEST_F(stack_deletion, delete_3)
{
	CHECK(stk_delete(tau->s, NULL) == NULL, "delete should handle non-empty stack");
	*tau = (struct stack_deletion){0};
}

TEST(stack_deletion, delete_3_dupfunc)
{
	stack *s = stk_new();
	single_link_node *n1 = stk_push(s, n1d, dup_str);
	single_link_node *n2 = stk_push(s, n2d, dup_str);
	single_link_node *n3 = stk_push(s, n3d, dup_str);
	if (!s || !n1 || !n2 || !n3)
	{
		/*WARNING: Wil not be able to free data in the nodes.*/
		free(s);
		free(n1);
		free(n2);
		free(n3);
	}

	REQUIRE(s, "failed to create stack");
	REQUIRE(n1, "failed to push node to stack");
	REQUIRE(n2, "failed to push node to stack");
	REQUIRE(n3, "failed to push node to stack");

	CHECK(stk_delete(s, free) == NULL, "delete should handle non-empty stack");
}
