#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/keylogger.h"
#include "../src/server_utils.h"

/* Tests that reseting the key_package struct works when there is one key
present. */
Test(reset_structs, check_single_key) {
  key_info keys[] = {{"key", "timestamp"}};
  key_package pack = {.keys = keys, .keys_arr_size = 1};
  key_package* package = &pack;

  package->keys_arr_size = 1;

  reset_structs(package);
  cr_assert(eq(int, 0, package->keys_arr_size));
}

/* Tests that resetting the key_package struct works when there are multiple
keys present. */
Test(rest_structs, check_multiple_keys) {
  key_info keys[] = {{"key", "timestamp"}, {"key2", "timestamp2"}};
  key_package pack = {.keys = keys, .keys_arr_size = 2};
  key_package* package = &pack;

  reset_structs(package);
  cr_assert(eq(int, 0, package->keys_arr_size));
}

Test(keys_to_file, writes_keys_to_file) {
  // Create a temporary file to write the keys to
  FILE* key_file = tmpfile();
  cr_assert_not_null(key_file, "Failed to create temporary file");

  // Create a key package to write to the file
  key_info keys[] = {{"key", "timestamp"}, {"key2", "timestamp2"}};
  key_package package = {.keys = keys, .keys_arr_size = 2};

  // Call the function to write the keys to the file
  keys_to_file(key_file, package);

  // Rewind the file pointer to read the contents
  rewind(key_file);

  // Read the contents of the file into a buffer
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, key_file);
  cr_assert_gt(read_size, 0, "Failed to read contents of file");

  // Check that the contents of the file match the expected output
  char* expected_output =
      "Key:key Timestamp: timestamp\tKey:key2 Timestamp: timestamp2\t\n";
  cr_assert_str_eq(buffer, expected_output, "Output does not match expected");
}
