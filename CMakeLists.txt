cmake_minimum_required(VERSION 3.16)
project(hash_test)

include_directories(src)

add_executable(app example.c src/hashtable.c)
add_executable(test_bin tests/hashtable_test.c src/hashtable.c)

enable_testing()
add_test(test_put          test_bin 0)
add_test(test_collision    test_bin 1)
add_test(test_update       test_bin 2)
add_test(test_num_elements test_bin 3)
