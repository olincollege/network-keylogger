/**
 * Script created to run/test the keylogger (prior to unit tests).
 */

#pragma once
#include "client.h"
#include "keylogger.h"

int main(void) {
  key_package pack;
  pack.keys_arr_size = 0;
  log_device(&pack);
  log_keys(&pack);
  print_logged_keys(pack);
  return 0;
}
