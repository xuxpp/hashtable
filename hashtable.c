#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

typedef struct element_int_t element_int_t;
struct element_int_t
{
    bool           in_use;
    void          *key;
    void          *val;
    element_int_t *next;
};

struct hashtable_t
{
    size_t            num_element_ints;
    size_t            table_size;
    size_t            key_len;
    size_t            num_collision;
    hashtable_hash_f *hash_f;
    element_int_t    *data;
};

static size_t default_hash_func(void *key, size_t len)
{
    return (size_t)key;
}

static size_t hash_idx(hashtable_t *ht, void *key)
{
    return ht->hash_f(key, ht->key_len) & (ht->table_size - 1);
}

static void clear_list(element_int_t *e)
{
    element_int_t *curr = e;
    while (curr)
    {
        element_int_t *next = curr->next;
        free(curr);
        curr = next;
    }
}

void *hashtable_get(hashtable_t *ht, void *key)
{
    element_int_t *curr = &ht->data[hash_idx(ht, key)];

    while (curr)
        if (memcmp(curr->key, key, ht->key_len) == 0)
            return curr->val;
        else
            curr = curr->next;

    return NULL;
}

void hashtable_put(hashtable_t *ht, void *key, void *val)
{
    element_int_t *curr = &ht->data[hash_idx(ht, key)];

    if (curr->in_use)
    {
        ht->num_collision++;
        while (curr->next) curr = curr->next;
        curr->next = calloc(1, sizeof(*curr));
        curr = curr->next;
    }

    curr->key = key;
    curr->val = val;
    curr->in_use = true;
    ht->num_element_ints++;
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

    return ht;
}

void hashtable_destroy(hashtable_t *ht)
{
    for (size_t i = 0; i < ht->table_size; i++)
        if (ht->data[i].in_use)
            if (ht->data[i].next)
                clear_list(ht->data[i].next);
    if (ht->data)
        free(ht->data);
    free(ht);
}
