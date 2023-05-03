/**
 * Server-client connection creation utilities
 *
 */

#include "server_utils.h"

#include <stdint.h>  // uint16_t
#include <stdio.h>   // perror
#include <stdlib.h>  // exit, EXIT_FAILURE
#include <unistd.h>  // close

const uint16_t PORT = 1525;

void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}

int open_tcp_socket(void) {
  int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_desc == -1) {
    error_and_exit("Error with my socks!");
  }
  // Return the socket descriptor
  return sock_desc;
}

void close_tcp_socket(int socket_) {
  // Replace this with your implementation.
  if (close(socket_) == -1) {
    error_and_exit("Couldn't close.");
  }
}

struct sockaddr_in socket_address(in_addr_t addr, in_port_t port) {
  // Replace this with your implementation.
  struct sockaddr_in foo;

  foo.sin_family = AF_INET;
  foo.sin_addr.s_addr = htonl(addr);
  foo.sin_port = htons(port);

  return foo;
}
