#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

#define TEST_SIZE 12345
#define KEY_LEN   20

typedef struct {
    int a;
    int b;
} dummy_data_t;

int main(int argc, char *argv[])
{
    dummy_data_t *data = calloc(TEST_SIZE, sizeof(*data));
    char        **keys = calloc(TEST_SIZE, sizeof(*keys));

    for (size_t i = 0; i < TEST_SIZE; i++)
        keys[i] = calloc(1, KEY_LEN * sizeof(*keys[i]));

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        snprintf(keys[i], KEY_LEN, "key %ld", i);
        data[i].a = i;
        data[i].b = i+1;
    }

    hashtable_t *ht = hashtable_create(TEST_SIZE, KEY_LEN);

    for (size_t i = 0; i < TEST_SIZE; i++)
        hashtable_put(ht, keys[i], &data[i]);

    for (size_t i = 0; i < 10; i++)
    {
        dummy_data_t *data = hashtable_get(ht, keys[i]);
        printf("%s: dummy_a: %d, dummy_b %d\n", keys[i], data->a, data->b);
    }

    printf("Number of elements  : %ld\n", hashtable_get_num_elements(ht));
    printf("Table size          : %ld\n", hashtable_get_table_size(ht));
    printf("Number of collisions: %ld\n", hashtable_get_num_collision(ht));

    hashtable_resize(ht, TEST_SIZE * 3);
    printf("\nResizing...\n");
    printf("Number of elements  : %ld\n", hashtable_get_num_elements(ht));
    printf("Table size          : %ld\n", hashtable_get_table_size(ht));
    printf("Number of collisions: %ld\n", hashtable_get_num_collision(ht));

    hashtable_destroy(ht);
    for (size_t i = 0; i < TEST_SIZE; i++)
        free(keys[i]);
    free(keys);
    free(data);

    return 0;
}