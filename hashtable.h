#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct hashtable_t hashtable_t;
typedef uint32_t hashtable_hash_f (const void *key, size_t len);

void        *hashtable_get    (hashtable_t *ht, const void *key);
bool         hashtable_put    (hashtable_t *ht, const void *key, const void *val); // True: collision
hashtable_t *hashtable_create (size_t initial_size, size_t key_len);
void         hashtable_destroy(hashtable_t *ht);

size_t hashtable_resize           (hashtable_t *ht, size_t new_size);
void   hashtable_set_hash_function(hashtable_t *ht, hashtable_hash_f *f);

size_t hashtable_get_num_elements (hashtable_t *ht);
size_t hashtable_get_table_size   (hashtable_t *ht);
size_t hashtable_get_num_collision(hashtable_t *ht);

