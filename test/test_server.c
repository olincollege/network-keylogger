#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/server.h"
#include "../src/server_utils.h"

// Check that the listener socket is successfully created.
Test(make_keylog_server, listener_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 1111);
  keylog_server* server = make_keylog_server(addr, BACKLOG_SIZE);
  cr_expect(ge(int, server->listener, 0));
  free_keylog_server(server);
}

// Check that the address and port are set correctly.
Test(make_keylog_server, addr_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  keylog_server* server = make_keylog_server(addr, BACKLOG_SIZE);
  cr_expect(eq(u32, server->addr.sin_addr.s_addr, htonl(INADDR_ANY)));
  cr_expect(eq(u16, server->addr.sin_port, htons(2222)));
  free_keylog_server(server);
}

// Check that the backlog size is set correctly.
Test(make_keylog_server, backlog_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 3333);
  keylog_server* server = make_keylog_server(addr, BACKLOG_SIZE);
  cr_expect(eq(int, server->max_backlog, BACKLOG_SIZE));
  free_keylog_server(server);
}

// Check that the socket is accepting connections once it starts listening.
Test(make_keylog_server, socket_listening) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 4444);
  keylog_server* server = make_keylog_server(addr, BACKLOG_SIZE);
  listen_for_connections(server);
  int listening = 0;
  socklen_t option_len = sizeof(int);
  cr_expect(ne(int,
               getsockopt(server->listener, SOL_SOCKET, SO_ACCEPTCONN,
                          &listening, &option_len),
               -1));
  cr_expect(ne(int, listening, 0));
  free_keylog_server(server);
}
