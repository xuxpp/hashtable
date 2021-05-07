#pragma once

#include <stddef.h>

typedef struct hashtable_int_t hashtable_int_t;
typedef size_t hashtable_int_t_hash_f (int);

void *hashtable_int_get(hashtable_int_t *ht, int key);
void  hashtable_int_put(hashtable_int_t *ht, int key, void *val);

void hashtable_int_set_hash_function(hashtable_int_t *ht, hashtable_int_t_hash_f *f);

hashtable_int_t *hashtable_int_create(size_t initial_size);
void       hashtable_int_destroy(hashtable_int_t *ht);