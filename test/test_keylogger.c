#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/keylogger.h"
#include "../src/server_utils.h"

/* Tests that reseting the key_package struct works when there is one key
present. */
Test(reset_structs, check_single_key) {
  key_package pack;
  pack.keys_arr_size = 0;
  pack.keys = malloc(sizeof(char));
  key_package* package = &pack;
  key_info pressed_key = {.key = "test_key", .timestamp = "test_timestamp"};
  package->keys[package->keys_arr_size] = pressed_key;
  package->keys_arr_size++;

  reset_structs(package);
  cr_assert(eq(int, 0, package->keys_arr_size));
}

/* Tests that resetting the key_package struct works when there are multiple
keys present. */
Test(rest_structs, check_multiple_keys) {
  key_package pack;
  pack.keys_arr_size = 0;
  pack.keys = malloc(sizeof(char));
  key_package* package = &pack;
  key_info pressed_key = {.key = "test_key", .timestamp = "test_timestamp"};
  package->keys[package->keys_arr_size] = pressed_key;
  package->keys_arr_size++;
  key_info pressed_key2 = {.key = "test_key2", .timestamp = "test_timestamp2"};
  package->keys[package->keys_arr_size] = pressed_key2;
  package->keys_arr_size++;

  reset_structs(package);
  cr_assert(eq(int, 0, package->keys_arr_size));
}
