all: hash_test

hash_test: example.c hashtable.c
	$(CC) -Wall -O2 -I.. -o $@ $^

clean:
	rm -f hash_test