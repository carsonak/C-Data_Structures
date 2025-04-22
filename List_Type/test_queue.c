#include "queue.h"
#include "tau/tau.h"
#include <stdlib.h> /* free */

#define MAX_STRING_LENGTH 256U

static char n1d[] = "one", n2d[] = "two", n3d[] = "three", n4d[] = "four";

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

TEST(queue_creation, new_returns_empty_q)
{
	queue *const q = queue_new();

	REQUIRE(q, "queue_new() returns non-null");
	CHECK(queue_len(q) == 0, "length should be 0");
	CHECK(queue_peek_first(q) == NULL, "head of the queue should be empty");
	CHECK(queue_peek_last(q) == NULL, "tail of the queue should be empty");
	free(q);
}

/*######################################################################*/
/*######################################################################*/

struct adding_items
{
	single_link_node *n1, *n2, *n3;
	queue *q;
};

TEST_F_SETUP(adding_items)
{
	tau->q = queue_new();
	REQUIRE(tau->q, "queue_new() returns non-null");
}

TEST_F_TEARDOWN(adding_items)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(adding_items, enqueue_null_q_returns_null)
{
	CHECK(enqueue(NULL, NULL, NULL) == NULL, "pointer to queue required");
	CHECK(enqueue(NULL, n1d, NULL) == NULL, "pointer to queue required");
	CHECK(enqueue(NULL, NULL, dup_str) == NULL, "pointer to queue required");
	CHECK(enqueue(NULL, n1d, dup_str) == NULL, "pointer to queue required");
}

TEST_F(adding_items, enqueue_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = enqueue(tau->q, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");

	CHECK(queue_len(tau->q) == 1, "there should only be 1 item in the queue");
	CHECK(
		queue_peek_first(tau->q) == NULL, "head should point to the added node"
	);
	CHECK(
		queue_peek_last(tau->q) == NULL, "tail should point to the added node"
	);
}

TEST_F(adding_items, enqueue_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = enqueue(tau->q, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");

	CHECK(queue_len(tau->q) == 1, "there should only be 1 item in the queue");
	CHECK_STREQ((char *)queue_peek_first(tau->q), n1d, "head should be n1");
	CHECK_STREQ((char *)queue_peek_last(tau->q), n1d, "tail should be n2");

	free(queue_peek_first(tau->q));
}

TEST_F(adding_items, enqueue_3_nodes)
{
	tau->n1 = enqueue(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");

	CHECK(queue_len(tau->q) == 1, "there should only be 1 item in the queue");
	CHECK(
		queue_peek_first(tau->q) == n1d, "head should point to the added node"
	);
	CHECK(
		queue_peek_last(tau->q) == n1d, "tail should point to the added node"
	);

	tau->n2 = enqueue(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "enqueue() should return non-null pointer");

	CHECK(queue_len(tau->q) == 2, "there should only be 2 items in the queue");
	CHECK(queue_peek_first(tau->q) == n1d, "head should be n1");
	CHECK(queue_peek_last(tau->q) == n2d, "tail should be n2");

	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "enqueue() should return non-null pointer");

	CHECK(queue_len(tau->q) == 3, "there should only be 3 items in the queue");
	CHECK(queue_peek_first(tau->q) == n1d, "head should be n1");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be n3");
}

TEST_F(adding_items, enqueue_q_d_failfunc_returns_NULL)
{
	tau->n1 = enqueue(tau->q, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "enqueue() should return NULL on failure");
	CHECK(queue_peek_first(tau->q) == NULL, "head should be unchanged");
	CHECK(queue_peek_last(tau->q) == NULL, "tail should be unchanged");
}

TEST_F(adding_items, enqueue_2_nodes_fail_on_2nd)
{
	tau->n1 = enqueue(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");
	tau->n2 = enqueue(tau->q, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "enqueue() should return NULL on failure");
	CHECK(queue_len(tau->q) == 1, "there should only be 1 item in the queue");
	CHECK(queue_peek_first(tau->q) == n1d, "head should be unchanged");
	CHECK(queue_peek_last(tau->q) == n1d, "tail should be unchanged");
}

/*######################################################################*/
/*######################################################################*/

struct removing_items
{
	single_link_node *n1, *n2, *n3;
	queue *q;
};

TEST_F_SETUP(removing_items)
{
	tau->q = queue_new();
	REQUIRE(tau->q, "queue_new() returns non-null");
}

TEST_F_TEARDOWN(removing_items)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
}

TEST(removing_items, dequeue_null_should_return_NULL)
{
	CHECK(dequeue(NULL) == NULL, "pointer to queue is required");
}

TEST_F(removing_items, dequeue_empty_q_changes_nothing)
{
	CHECK(dequeue(tau->q) == NULL, "nothing should be done");

	CHECK(queue_len(tau->q) == 0, "queue should have 0 items");
	CHECK(queue_peek_first(tau->q) == NULL, "head should be unchanged");
	CHECK(queue_peek_last(tau->q) == NULL, "tail should be unchanged");
}

TEST_F(removing_items, dequeue_1_node_from_1)
{
	tau->n1 = enqueue(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");

	CHECK_STREQ(
		(char *)dequeue(tau->q), n1d, "first node should have been removed"
	);
	tau->n1 = NULL;
	CHECK(queue_len(tau->q) == 0, "queue should have 0 items");
	CHECK(queue_peek_first(tau->q) == NULL, "head should be NULL");
	CHECK(queue_peek_last(tau->q) == NULL, "tail should be NULL");
}

TEST_F(removing_items, dequeue_1_node_from_2)
{
	tau->n1 = enqueue(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");
	tau->n2 = enqueue(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "enqueue() should return non-null pointer");

	CHECK_STREQ(
		(char *)dequeue(tau->q), n1d, "first node should have been removed"
	);
	tau->n1 = NULL;
	CHECK(queue_len(tau->q) == 1, "queue should have 1 item");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be n2");
	CHECK(queue_peek_last(tau->q) == n2d, "tail should be unchanged");
}

TEST_F(removing_items, dequeue_1_node_from_3)
{
	tau->n1 = enqueue(tau->q, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "enqueue() should return non-null pointer");
	tau->n2 = enqueue(tau->q, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "enqueue() should return non-null pointer");
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "enqueue() should return non-null pointer");

	CHECK_STREQ(
		(char *)dequeue(tau->q), n1d, "first node should have been removed"
	);
	tau->n1 = NULL;
	CHECK(queue_len(tau->q) == 2, "queue should have 2 items");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be n2");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be unchanged");
}

/*######################################################################*/
/*######################################################################*/

struct add_and_remove
{
	single_link_node *n1, *n2, *n3, *n4;
	queue *q;
};

TEST_F_SETUP(add_and_remove)
{
	tau->q = queue_new();
	tau->n1 = enqueue(tau->q, n1d, NULL);
	tau->n2 = enqueue(tau->q, n2d, NULL);
	if (!tau->q || !tau->n1 || !tau->n2)
	{
		free(tau->n1);
		free(tau->n2);
		free(tau->q);
	}

	REQUIRE(tau->q, "queue_new() returns non-null");
	REQUIRE(tau->n1, "enqueue() returns non-null");
	REQUIRE(tau->n2, "enqueue() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove)
{
	free(tau->q);
	free(tau->n1);
	free(tau->n2);
	free(tau->n3);
	free(tau->n4);
}

TEST_F(add_and_remove, add_remove)
{
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;

	CHECK(queue_len(tau->q) == 2, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be updated to n2");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove, remove_add)
{
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(queue_len(tau->q) == 2, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be updated to n2");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove, add_add_remove)
{
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	tau->n4 = enqueue(tau->q, n4d, NULL);
	REQUIRE(tau->n4, "n4 is added");
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;

	CHECK(queue_len(tau->q) == 3, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be updated to n2");
	CHECK(queue_peek_last(tau->q) == n4d, "tail should be updated to n4.");
}

TEST_F(add_and_remove, remove_remove_add)
{
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;
	CHECK(dequeue(tau->q) == n2d, "n2 is removed from the head");
	tau->n2 = NULL;
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");

	CHECK(queue_len(tau->q) == 1, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n3d, "head should be updated to n3");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove, add_remove_remove)
{
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;
	CHECK(dequeue(tau->q) == n2d, "n2 is removed from the head");
	tau->n2 = NULL;

	CHECK(queue_len(tau->q) == 1, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n3d, "head should be updated to n3");
	CHECK(queue_peek_last(tau->q) == n3d, "tail should be updated to n3.");
}

TEST_F(add_and_remove, remove_add_add)
{
	CHECK(dequeue(tau->q) == n1d, "n1 is removed from the head");
	tau->n1 = NULL;
	tau->n3 = enqueue(tau->q, n3d, NULL);
	REQUIRE(tau->n3, "n3 is added");
	tau->n4 = enqueue(tau->q, n4d, NULL);
	REQUIRE(tau->n4, "n4 is added");

	CHECK(queue_len(tau->q) == 3, "number of items is unchanged");
	CHECK(queue_peek_first(tau->q) == n2d, "head should be updated to n2");
	CHECK(queue_peek_last(tau->q) == n4d, "tail should be updated to n4.");
}

/*######################################################################*/
/*######################################################################*/

TEST(deleting_queue, delete_should_clear_all_items)
{
	queue *q = queue_new();
	single_link_node *n1 = enqueue(q, n1d, NULL);
	single_link_node *n2 = enqueue(q, n2d, NULL);
	single_link_node *n3 = enqueue(q, n3d, NULL);

	if (!q || !n1 || !n2 || !n3)
	{
		free(n1);
		free(n2);
		free(n3);
		free(q);
	}

	REQUIRE((q && n1 && n2 && n3), "failed to create queue");

	q = queue_delete(q, NULL);
}

TEST(deleting_queue, delete_f_should_clear_all_items)
{
	queue *q = queue_new();
	single_link_node *n1 = enqueue(q, n1d, dup_str);
	single_link_node *n2 = enqueue(q, n2d, dup_str);
	single_link_node *n3 = enqueue(q, n3d, dup_str);

	if (!q || !n1 || !n2 || !n3)
	{
		free(queue_peek_first(q));
		free(n1);
		/*WARNING: data in n2 might not be freed.*/
		free(n2);
		free(queue_peek_last(q));
		free(n3);
		free(q);
	}

	REQUIRE((q && n1 && n2 && n3), "failed to create queue");

	q = queue_delete(q, free);
}

/*######################################################################*/
/*######################################################################*/

TEST(qfa, qfa_invalid_args)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));

	CHECK(
		queue_from_array(NULL, 0, 0, NULL, NULL) == NULL,
		"data_array, len and type_size are required"
	);

	CHECK(
		queue_from_array(NULL, arr_len, 0, NULL, NULL) == NULL,
		"data_array and type_size are required"
	);
	CHECK(
		queue_from_array(NULL, 0, sizeof(*arr), NULL, NULL) == NULL,
		"data_array and len are required"
	);
	CHECK(
		queue_from_array(NULL, arr_len, sizeof(*arr), NULL, NULL) == NULL,
		"data_array is required"
	);

	CHECK(
		queue_from_array(arr, 0, 0, NULL, NULL) == NULL,
		"len and type_size are required"
	);
	CHECK(
		queue_from_array(arr, arr_len, 0, NULL, NULL) == NULL,
		"type_size is required"
	);
	CHECK(
		queue_from_array(arr, 0, sizeof(*arr), NULL, NULL) == NULL,
		"len is required"
	);

	CHECK(
		queue_from_array(arr, arr_len, sizeof(*arr), dup_llint, NULL) == NULL,
		"a dup function should always be accompanied by a delete function"
	);
}

TEST(qfa, queue_from_array)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));
	queue *q = queue_from_array(arr, arr_len, sizeof(*arr), NULL, NULL);

	REQUIRE(q, "queue_from_array() should return non-null pointer");

	CHECK(
		queue_len(q) == arr_len, "there should be %zu items in the queue",
		arr_len
	);
	CHECK(*(long long int *)dequeue(q) == arr[0]);
	CHECK(*(long long int *)dequeue(q) == arr[1]);
	CHECK(*(long long int *)dequeue(q) == arr[2]);
	CHECK(*(long long int *)dequeue(q) == arr[3]);
	CHECK(*(long long int *)dequeue(q) == arr[4]);
	CHECK(queue_len(q) == 0, "there should now be 0 items in the queue");
	CHECK(queue_peek_first(q) == NULL, "head should be NULL");
	CHECK(queue_peek_last(q) == NULL, "tail should be NULL");

	q = queue_delete(q, NULL);
}
