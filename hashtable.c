#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

typedef struct element_t element_t;
struct element_t
{
    bool           in_use;
    void          *key;
    void          *val;
    element_t *next;
};

struct hashtable_t
{
    size_t            num_elements;
    size_t            table_size;
    size_t            key_len;
    size_t            num_collision;
    hashtable_hash_f *hash_f;
    element_t    *data;
};

uint32_t murmur_hash(const void *key, size_t len)
{
    uint32_t h = 3323198485ul;
    const uint8_t *byte = key;

    for (size_t i = 0; i < len; i++)
    {
        h ^= byte[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }

    return h;
}

static uint32_t default_hash_func(const void *key, size_t len)
{
    return murmur_hash(key, len);
}

static size_t hash_idx(hashtable_t *ht, const void *key)
{
    return ht->hash_f(key, ht->key_len) & (ht->table_size - 1);
}

static void clear_list(element_t *e, size_t len)
{
    element_t *curr = e;
    while (curr)
    {
        element_t *next = curr->next;
        free(curr->key);
        free(curr);
        curr = next;
    }
}

void *hashtable_get(hashtable_t *ht, const void *key)
{
    element_t *curr = &ht->data[hash_idx(ht, key)];

    while (curr)
        if (memcmp(curr->key, key, ht->key_len) == 0)
            return curr->val;
        else
            curr = curr->next;

    return NULL;
}

void hashtable_put(hashtable_t *ht, const void *key, const void *val)
{
    element_t *curr = &ht->data[hash_idx(ht, key)];

    if (curr->in_use)
    {
        ht->num_collision++;
        while (curr->next) curr = curr->next;
        curr->next = calloc(1, sizeof(*curr));
        curr = curr->next;
        curr->key = calloc(1, ht->key_len);
    }

    memcpy(curr->key, key, ht->key_len);
    curr->val = (void *)val;
    curr->in_use = true;
    ht->num_elements++;
}

size_t hashtable_get_num_collision(hashtable_t *ht)
{
    return ht->num_collision;
}

void hashtable_set_hash_function(hashtable_t *ht, hashtable_hash_f *f)
{
    ht->hash_f = f;
}

hashtable_t *hashtable_create(size_t table_size, size_t key_len)
{
    hashtable_t *ht = calloc(1, sizeof(*ht));
    ht->hash_f      = default_hash_func;
    ht->key_len     = key_len;
    for (size_t i = 0; i < 32; i++)
    {
        size_t tmp = (1 << i);
        if (tmp >= table_size)
        {
            /* Table size == power of 2 to avoid modulo */
            ht->table_size = tmp;
            break;
        }
    }
    if (!ht->table_size)
    {
        fprintf(stderr, "table_size too big\n");
        hashtable_destroy(ht);
        return NULL;
    }
    ht->data = calloc(ht->table_size, sizeof(*ht->data));
    for (size_t i = 0; i < ht->table_size; i++)
        ht->data[i].key = calloc(1, ht->key_len);

    return ht;
}

void hashtable_destroy(hashtable_t *ht)
{
    for (size_t i = 0; i < ht->table_size; i++)
    {
        element_t *e = &ht->data[i];
        if (e->in_use)
            if (e->next)
                clear_list(e->next, ht->key_len);
        free(e->key);
    }
    free(ht->data);
    free(ht);
}
