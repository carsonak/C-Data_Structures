#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>

#if __has_attribute(nonnull)
#define ATTR_NONNULL __attribute__((nonnull))
#else
#define ATTR_NONNULL
#endif

typedef const unsigned char *str_literal;

/**
 * struct Bucket - bucket of a hash table
 * @key:
 */
typedef struct Bucket
{
	size_t hash;
	char *key;
	char *value;
	struct Bucket *next;
} Bucket;

/**
 * struct HashMap - a hash table
 * @size: number of slots in the hash table
 * @array: the hash table
 */
typedef struct HashMap
{
	size_t size;
	Bucket **array;
} HashMap;

HashMap *hashmap_create(size_t size);
void hashmap_delete(HashMap *ht);
size_t get_index(str_literal key, size_t size);
Bucket *hashmap_get(const HashMap *ht, str_literal key);
void *add_bucket_head(Bucket **h, const char *key, const char *val);
int hashmap_insert(HashMap *ht, const char *key, const char *value);
void hashmap_print(const HashMap *ht);

#endif /* HASHMAP_H */
