#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct hashtable_t hashtable_t;
typedef uint32_t hashtable_hash_f (const void *key, size_t len);

void *hashtable_get(hashtable_t *ht, const void *key);
void  hashtable_put(hashtable_t *ht, const void *key, const void *val);

size_t hashtable_get_num_collision(hashtable_t *ht);
void hashtable_set_hash_function(hashtable_t *ht, hashtable_hash_f *f);

hashtable_t *hashtable_create(size_t initial_size, size_t key_len);
void         hashtable_destroy(hashtable_t *ht);