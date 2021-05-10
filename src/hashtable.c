#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

typedef struct element_t element_t;
struct element_t
{
    char      *key;
    void      *val;
    size_t     val_len;
    element_t *next;
};

struct hashtable_t
{
    size_t            num_elements;
    size_t            table_size;
    size_t            num_collision;
    hashtable_hash_f *hash_f;
    element_t        *data;
};

uint32_t murmur_hash(const char *key, size_t len)
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

static uint32_t default_hash_func(const char *key, size_t len)
{
    return murmur_hash(key, len);
}

static size_t hash_idx(hashtable_t *ht, const char *key)
{
    return ht->hash_f(key, strlen(key)) & (ht->table_size - 1);
}

static void free_list(element_t *element, size_t len)
{
    element_t *e = element;
    while (e)
    {
        element_t *next = e->next;
        free(e->val);
        free(e->key);
        free(e);
        e = next;
    }
}

static void free_data(element_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        element_t *e = &data[i];
        if (e->key)
            if (e->next)
                free_list(e->next, len);
        free(e->val);
        free(e->key);
    }
    free(data);
}

static size_t nearest_power_of_2(size_t size)
{
    for (size_t i = 0; i < 32; i++)
    {
        size_t tmp = (1 << i);
        if (tmp >= size)
            return tmp;
    }

    return 0;
}

static bool key_match(const char *k1, const char *k2)
{
    return strcmp((void *)k1, (void *)k2) == 0;
}

void *hashtable_get(hashtable_t *ht, const char *key)
{
    element_t *e = &ht->data[hash_idx(ht, key)];

    while (e)
        if (strcmp(e->key, key) == 0)
            return e->val;
        else
            e = e->next;

    return NULL;
}

bool hashtable_put(hashtable_t *ht, const char *key, const void *val, size_t val_len)
{
    element_t *e = &ht->data[hash_idx(ht, key)];
    bool collision = false;

    while (e && e->key)
    {
        if (key_match(e->key, key))
        {
            // Update value
            free(e->val);
            e->val = calloc(1, val_len);
            memcpy(e->val, val, val_len);
            return false;
        }
        else
        {
            collision = true;
            if (!e->next) // End of the list
            {
                e->next = calloc(1, sizeof(*e->next));
                e = e->next;
                break;
            }
            else
            {
                e = e->next;
            }
        }
    }

    e->key = calloc(1, strlen(key)+1); // +1 -> '\0'
    e->val = calloc(1, val_len);
    strcpy(e->key, key);
    memcpy(e->val, val, val_len);
    ht->num_elements++;

    if (collision)
        ht->num_collision++;

    return collision;
}

size_t hashtable_resize(hashtable_t *ht, size_t size)
{
    size_t     size_new = nearest_power_of_2(size); // Table size == power of 2 to avoid modulo
    size_t     size_old = ht->table_size;
    element_t *data_old = ht->data;
    element_t *data_new = calloc(size_new, sizeof(*data_new));

    ht->table_size    = size_new;
    ht->data          = data_new;
    ht->num_elements  = 0;
    ht->num_collision = 0;

    /* Put old data to new data */
    for (size_t i = 0; i < size_old; i++)
    {
        element_t *e = &data_old[i];
        if (e->key)
        {
            hashtable_put(ht, e->key, e->val, e->val_len);
            while (e->next)
            {
                e = e->next;
                hashtable_put(ht, e->key, e->val, e->val_len);
            }
        }
    }
    free_data(data_old, size_old);

    return ht->num_collision;
}

size_t hashtable_get_num_elements (hashtable_t *ht){ return ht->num_elements; }
size_t hashtable_get_table_size   (hashtable_t *ht){ return ht->table_size;   }
size_t hashtable_get_num_collision(hashtable_t *ht){ return ht->num_collision;}

void hashtable_set_hash_function(hashtable_t *ht, hashtable_hash_f *f)
{
    ht->hash_f = f;
}

hashtable_t *hashtable_create(size_t initial_size)
{
    hashtable_t *ht = calloc(1, sizeof(*ht));
    ht->hash_f      = default_hash_func;
    ht->table_size  = nearest_power_of_2(initial_size); 
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
    free_data(ht->data, ht->table_size);
    free(ht);
}
