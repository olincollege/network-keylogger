// tests for server utils

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../src/server_utils.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

// Check that the descriptor returned is a valid socket descriptor value
// (nonnegative integer).
Test(open_tcp_socket, valid_descriptor_value) {
  int socket_descriptor = open_tcp_socket();
  cr_assert(ge(int, socket_descriptor, 0));
  close_tcp_socket(socket_descriptor);
}

// Check that the socket is actually a TCP socket.
Test(open_tcp_socket, valid_socket) {
  int socket_descriptor = open_tcp_socket();
  int socket_type = 0;
  socklen_t option_len = sizeof(int);
  cr_expect(ne(int,
               getsockopt(socket_descriptor, SOL_SOCKET, SO_TYPE,
                          (void*)&socket_type, &option_len),
               -1));
  cr_expect(eq(int, socket_type, SOCK_STREAM));
  close_tcp_socket(socket_descriptor);
}

// Check that getting socket options after closing it results in an error.
Test(close_tcp_socket, invalid_after_close) {
  int socket_descriptor = open_tcp_socket();
  close_tcp_socket(socket_descriptor);
  int debug = 0;
  socklen_t debug_len = sizeof(int);
  cr_expect(eq(int,
               getsockopt(socket_descriptor, SOL_SOCKET, SO_DEBUG,
                          (void*)&debug, &debug_len),
               -1));
}

// Check that the socket address is created with the correct address family.
Test(socket_address, correct_proto_family) {
  struct sockaddr_in addr = socket_address(0, 0);
  cr_assert(eq(int, addr.sin_family, AF_INET));
}

// Check that a socket address created with address and port 0 has those values.
Test(socket_address, zero_addr_and_port) {
  struct sockaddr_in addr = socket_address(0, 0);
  cr_assert(zero(u32, addr.sin_addr.s_addr));
  cr_assert(zero(u16, addr.sin_port));
}

// Check a socket address converts its address bytes from host to network order.
Test(socket_address, addr_byte_order) {
  // IPv4 addresses are 4 bytes, so create a sample value here.
  in_addr_t ip_addr = 0x12345678;
  struct sockaddr_in addr = socket_address(ip_addr, 0);
  cr_assert(eq(u32, addr.sin_addr.s_addr, htonl(ip_addr)));
}

// Check a socket address converts its port bytes from host to netowrk oder.
Test(socket_address, port_byte_order) {
  // Port numbers are 2 bytes, so create a sample value here.
  in_port_t port = 0x1234;
  struct sockaddr_in addr = socket_address(0, port);
  cr_assert(eq(u16, addr.sin_port, htons(port)));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
