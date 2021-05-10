#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashtable.h"

#define TABLE_SIZE 1024

bool test_put()
{
    bool result = false;

    hashtable_t *ht = hashtable_create(TABLE_SIZE);

    int put_val = 42;
    hashtable_put(ht, "0", &put_val, sizeof(put_val));
    int *get_val = (int *)hashtable_get(ht, "0");

    result = (*get_val == put_val);

    hashtable_destroy(ht);

    return result;
}

bool test_collision()
{
    bool result = false;

    hashtable_t *ht = hashtable_create(TABLE_SIZE);

    int put_val1 = 42;
    int put_val2 = 4242;
    hashtable_put(ht, "key 64", &put_val1, sizeof(put_val1));
    hashtable_put(ht, "key 89", &put_val2, sizeof(put_val2));
    int *get_val1 = (int *)hashtable_get(ht, "key 64");
    int *get_val2 = (int *)hashtable_get(ht, "key 89");

    result = (*get_val1 == put_val1) && (*get_val2 == put_val2);

    hashtable_destroy(ht);

    return result;
}

bool test_update()
{
    bool result = false;

    hashtable_t *ht = hashtable_create(TABLE_SIZE);

    int put_val = 0;
    hashtable_put(ht, "0", &put_val, sizeof(put_val));
    put_val = 42;
    hashtable_put(ht, "0", &put_val, sizeof(put_val));
    int *get_val = (int *)hashtable_get(ht, "0");

    result = (*get_val == 42);

    hashtable_destroy(ht);

    return result;
}

bool test_num_elements()
{
    bool result = true;

    hashtable_t *ht = hashtable_create(TABLE_SIZE);

    int put_val = 42;
    hashtable_put(ht, "0", &put_val, sizeof(put_val));
    result &= (1ul == hashtable_get_num_elements(ht));

    hashtable_put(ht, "0", &put_val, sizeof(put_val));
    result &= (1ul == hashtable_get_num_elements(ht));

    hashtable_put(ht, "1", &put_val, sizeof(put_val));
    result &= (2ul == hashtable_get_num_elements(ht));

    hashtable_destroy(ht);

    return result;
}

int main(int argc, char *argv[])
{
    int test_nr = atoi(argv[1]);
    bool result = false;

    switch (test_nr)
    {
        case 0: result = test_put()         ; break;
        case 1: result = test_collision()   ; break;
        case 2: result = test_update()      ; break;
        case 3: result = test_num_elements(); break;
        default: break;
    }

    if (result)
        return 0;
    else
        return 1;
}