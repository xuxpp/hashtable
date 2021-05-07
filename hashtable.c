#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"

typedef struct element_int_t element_int_t;
struct element_int_t
{
    bool       in_use;
    int        key;
    void      *val;
    element_int_t *next;
};

struct hashtable_int_t
{
    size_t                  num_element_ints;
    size_t                  table_size;
    hashtable_int_t_hash_f *hash_f;
    element_int_t          *data;
};

static size_t default_hash_func(int key)
{
    return key;
}

static size_t hash_idx(hashtable_int_t *ht, int key)
{
    return ht->hash_f(key) & (ht->table_size - 1);
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

void *hashtable_int_get(hashtable_int_t *ht, int key)
{
    element_int_t *curr = &ht->data[hash_idx(ht, key)];

    while (curr)
        if (curr->key == key)
            return curr->val;
        else
            curr = curr->next;

    return NULL;
}

void hashtable_int_put(hashtable_int_t *ht, int key, void *val)
{
    element_int_t *curr = &ht->data[hash_idx(ht, key)];

    if (curr->in_use)
    {
        while (curr->next) curr = curr->next;
        curr->next = calloc(1, sizeof(*curr));
        curr = curr->next;
    }

    curr->key = key;
    curr->val = val;
    curr->in_use = true;
    ht->num_element_ints++;
}

void hashtable_int_set_hash_function(hashtable_int_t *ht, hashtable_int_t_hash_f *f)
{
    ht->hash_f = f;
}

hashtable_int_t *hashtable_int_create(size_t table_size)
{
    hashtable_int_t *ht = calloc(1, sizeof(*ht));
    ht->hash_f = default_hash_func;
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
        hashtable_int_destroy(ht);
        return NULL;
    }
    ht->data = calloc(ht->table_size, sizeof(*ht->data));

    return ht;
}

void hashtable_int_destroy(hashtable_int_t *ht)
{
    for (size_t i = 0; i < ht->table_size; i++)
        if (ht->data[i].in_use)
            if (ht->data[i].next)
                clear_list(ht->data[i].next);
    if (ht->data)
        free(ht->data);
    free(ht);
}
