#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

#define TEST_SIZE 12345

typedef struct {
    int a;
    int b;
} dummy_data_t;

int main(int argc, char *argv[])
{
    dummy_data_t *data = malloc(TEST_SIZE * sizeof(*data));
    int          *keys = malloc(TEST_SIZE * sizeof(*keys));

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        keys[i]   = i;
        data[i].a = i;
        data[i].b = i+1;
    }

    hashtable_int_t *ht = hashtable_int_create(TEST_SIZE);

    for (size_t i = 0; i < TEST_SIZE; i++)
        hashtable_int_put(ht, keys[i], &data[i]);

    for (size_t i = 0; i < 10; i++)
    {
        dummy_data_t *data = hashtable_int_get(ht, i);
        printf("%ld: dummy_a: %d, dummy_b %d\n", i, data->a, data->b);
    }

    return 0;
}