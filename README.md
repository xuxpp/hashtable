A chained hash table with linked lists in C. 

Default hash function: [MurmurHash](https://en.wikipedia.org/wiki/MurmurHash)

You can override the default hash function via `hashtable_set_hash_function()`.
## Basic usage
```c
#include <stdio.h>
#include "hashtable.h"

int main(int argc, char *argv[])
{
    // Create table
    hashtable_t *ht = hashtable_create(1);
    // Store value
    int val = 42;
    hashtable_put(ht, "answer_to_the_universe", &val, sizeof(val));
    // Get value
    int *rt_val = hashtable_get(ht, "answer_to_the_universe");
    printf("Answer: %d\n", *rt_val);
    // Destroy table
    hashtable_destroy(ht);
    
    return 0;
}
```
## More usage
```example.c```
