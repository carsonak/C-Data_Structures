#define _POSIX_C_SOURCE 200809L
#include "hashmap.h"
#include <json-c/json.h>
#define STR_ARRAY_SIZE (2048)

/**
 * struct str_list - array of strings
 * @len: number of strings in the array
 * @array: an array of strings
 */
struct str_list
{
	size_t len;
	char *array[STR_ARRAY_SIZE];
};

/**
 * parse_json_list - p
 * @json_file: path to a json file
 * @list: a struct of an array to populate with strings
 *
 * Return: number of strings parsed, -1 on error.
 */
ssize_t parse_json_list(const char *json_file, struct str_list *list)
{
	json_object *node = NULL, *root = json_object_from_file(json_file);
	size_t n = 0, i = 0;

	if (!root || !json_object_is_type(root, json_type_array))
		return (-1);

	n = json_object_array_length(root);
	list->len = 0;
	for (i = 0; i < n && i < STR_ARRAY_SIZE; i++)
	{
		node = json_object_array_get_idx(root, i);
		if (!node)
		{
			json_object_put(root);
			return (-1);
		}

		if (json_object_is_type(node, json_type_string))
		{
			list->array[list->len] = strdup(json_object_get_string(node));
			if (!list->array[list->len])
			{
				perror("Failed to copy string.");
				json_object_put(root);
				return (-1);
			}

			list->len++;
		}
	}

	json_object_put(root);
	return ((ssize_t)list->len);
}

/**
 * main - entry
 *
 * Return: 0
 */
int main(void)
{
	struct str_list keys = {STR_ARRAY_SIZE, {NULL}}, values = {STR_ARRAY_SIZE, {NULL}};
	Bucket *b = NULL;
	size_t i = 0;
	HashMap *hm = hashmap_create(1024);

	parse_json_list(
		"/home/line/Github_Repositories/World_of_C/Hash_Map/random_strings0.json",
		&keys);
	parse_json_list(
		"/home/line/Github_Repositories/World_of_C/Hash_Map/random_strings1.json",
		&values);

	for (i = 0; i < keys.len && i < values.len && i < STR_ARRAY_SIZE; i++)
		hashmap_insert(hm, keys.array[i], values.array[i]);

	for (i = 0; i < keys.len && i < values.len && i < STR_ARRAY_SIZE; i++)
	{
		b = hashmap_get(hm, (str_literal)keys.array[i]);
		printf("key[%ld]: strlen=%ld, ", i, strlen(b->key));
		printf("value[%ld]: strlen=%ld\n", i, strlen(b->value));
	}

	hashmap_delete(hm);
	for (i = 0; i < keys.len; i++)
	{
		free(keys.array[i]);
		keys.array[i] = NULL;
	}

	for (i = 0; i < values.len; i++)
	{
		free(values.array[i]);
		values.array[i] = NULL;
	}

	return (0);
}
