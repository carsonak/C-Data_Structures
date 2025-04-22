#include "hashmap.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

HashMap *hm = NULL;

/**
 * setup - initialise some variables
 */
void setup(void)
{
	hm = hashmap_create(10);
}

/**
 * teardown - clean up variables
 */
void teardown(void)
{
	hashmap_delete(hm);
	hm = NULL;
}

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_create_zero_sized_hashmap,
	 .description = "create(0)", .timeout = 0)
{
	hashmap_delete(hm);
	hm = hashmap_create(0);

	cr_assert(zero(ptr, hm->array));
}

Test(null_inputs, test_insert_nullkey_nullvalue,
	 .description = "insert(NULL, NULL)", .timeout = 0)
{
	Bucket *b = NULL;
	char a[] = {1, 0};

	hashmap_insert(hm, NULL, NULL);
	b = hashmap_get(hm, NULL);

	cr_assert(zero(ptr, b->key));
	cr_assert(zero(ptr, b->value));

	hashmap_insert(hm, a, "Hello World");
	hashmap_insert(hm, "\0", "\0");
	b = hashmap_get(hm, NULL);

	cr_assert(zero(ptr, b->key));
	cr_assert(zero(ptr, b->value));
}

Test(null_inputs, test_insert_nullvalue,
	 .description = "insert('Hello', NULL)", .timeout = 0)
{
	Bucket *b = NULL;
	char a[] = {1, 0};

	hashmap_insert(hm, "Hello", NULL);
	b = hashmap_get(hm, (str_literal) "Hello");

	cr_assert(eq(str, b->key, "Hello"));
	cr_assert(zero(ptr, b->value));

	hashmap_insert(hm, a, "Hello World");
	hashmap_insert(hm, "Hello", "\0");
	b = hashmap_get(hm, (str_literal) "Hello");

	cr_assert(eq(str, b->key, "Hello"));
	cr_assert(zero(str, b->value));
}

Test(null_inputs, test_insert_nullkey,
	 .description = "insert(NULL, 'World')", .timeout = 0)
{
	Bucket *b = NULL;
	char a[] = {1, 0};

	hashmap_insert(hm, NULL, "World");
	b = hashmap_get(hm, NULL);

	cr_assert(zero(ptr, b->key));
	cr_assert(eq(str, b->value, "World"));

	hashmap_insert(hm, a, "Hello World");
	hashmap_insert(hm, "\0", "World");
	b = hashmap_get(hm, NULL);

	cr_assert(zero(ptr, b->key));
	cr_assert(eq(str, b->value, "World"));
}

TestSuite(long_inputs, .init = setup, .fini = teardown);

Test(long_inputs, test_insert_longkey,
	 .description = "insert(longstr, 'World')", .timeout = 0)
{
	Bucket *b = NULL;
	size_t n = 0;
	char *key = NULL;
	char *mkey = NULL;
	FILE *rand_text = fopen("/home/line/Github_Repositories/World_of_C/Hash_Map/long_string.txt", "r");

	if (getline(&key, &n, rand_text) < 0)
	{
		fclose(rand_text);
		free(key);
		return;
	}

	fclose(rand_text);
	mkey = strdup(key);
	hashmap_insert(hm, mkey, "World");
	b = hashmap_get(hm, (str_literal)key);

	mkey[0] = '\0';
	free(mkey);

	cr_assert(eq(str, b->key, key));
	cr_assert(eq(str, b->value, "World"));
	free(key);
}

Test(long_inputs, test_insert_longvalue,
	 .description = "insert('Hello', longstr)", .timeout = 0)
{
	Bucket *b = NULL;
	size_t n = 0;
	char *value = NULL;
	char *cpy = NULL;
	FILE *rand_text = fopen("/home/line/Github_Repositories/World_of_C/Hash_Map/long_string.txt", "r");

	if (getline(&value, &n, rand_text) < 0)
	{
		fclose(rand_text);
		free(value);
		return;
	}

	fclose(rand_text);
	cpy = strdup(value);
	hashmap_insert(hm, "Hello", cpy);
	b = hashmap_get(hm, (str_literal) "Hello");

	cr_assert(eq(str, b->key, "Hello"));
	cr_assert(eq(str, b->value, value));

	cpy[0] = '\0';
	free(cpy);

	cr_assert(eq(str, b->key, "Hello"));
	cr_assert(eq(str, b->value, value));
	free(value);
}
