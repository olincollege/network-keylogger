#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/keylogger.h"
#include "../src/server_utils.h"

// Test that reseting the key_package struct works.
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
