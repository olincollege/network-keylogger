#include "server.h"

// Convenient error + exit program
void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}

void open_server(void) {
  // SOCK_STREAM because we want accuracy > speed
  int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_desc == -1) {
    error_and_exit("Error with my socks!");
  }
  // Return the socket descriptor
  return sock_desc;
}
void close_server(int socket_descriptor) {
  // Replace this with your implementation.
  if (close(socket_descriptor) == -1) {
    error_and_exit("Couldn't close the server.");
  }
}

void transmit_data(void) {
  // IMPLEMENTATION HERE
}

void clear_data_queue(void) {
  // IMPLEMENTATION HERE
}

struct sockaddr_in socket_address(in_addr_t addr, in_port_t port) {
  // Replace this with your implementation.
  struct sockaddr_in foo;

  foo.sin_family = AF_INET;
  foo.sin_addr.s_addr = htonl(addr);
  foo.sin_port = htons(port);

  return foo;
}
