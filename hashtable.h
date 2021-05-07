#pragma once

#include <stddef.h>

typedef struct hashtable_t hashtable_t;
typedef size_t hashtable_hash_f (void *key, size_t len);

void *hashtable_get(hashtable_t *ht, void *key);
void  hashtable_put(hashtable_t *ht, void * key, void *val);

void hashtable_set_hash_function(hashtable_t *ht, hashtable_hash_f *f);

hashtable_t *hashtable_create(size_t initial_size, size_t key_len);
void       hashtable_destroy(hashtable_t *ht);