#pragma once

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE

/**
 * Attempt to connect to a server on a given socket.
 *
 * Given an unconnected socket and a server address/port, attempt to connect to
 * a server. If successful, the socket descriptor client_socket can be used to
 * communicate with the server. If not, print an error message and exit without
 * returning from the function.
 *
 * @param client_socket The client's socket descriptor used for the connection.
 * @param server_addr The IPv4 socket address and port of the server.
 */
void try_connect(int client_socket, struct sockaddr_in server_addr);

/**
 * Get a file pointer for a given socket descriptor.
 *
 * Given a socket descriptor used by the client, create and return a file
 * pointer corresponding to that descriptor. This can be used with high-level
 * functions in stdio for convenience. In the event of an error in opening the
 * file stream, print an error message and exit the program, in which case this
 * function does not return.
 *
 * @param client_socket The socket descriptor to generate the file stream for.
 * @return A file pointer corresponding to client_socket.
 */
FILE* get_socket_file(int client_socket);

/**
 * Send and receive a line of text with the echo server.
 *
 * Given a file pointer corresponding to the client's connection with the
 * server, read a single line of text from standard input and send it to the
 * server. Then, receive the response back and print it to standard output. In
 * the event that an error occurs when attempting to send the text to the
 * server, print an error message and exit the program, in which case this
 * function does not return. Otherwise, return an integer corresponding to
 * success or a possible error in reading text from standard input or from the
 * server (which may be an end-of-file status).
 *
 * @param socket_file The file stream used in connection with the server.
 * @return 0 if the entire process succeeded, and -1 if there was an error
 * (which may be EOF).
 */
int echo(FILE* socket_file);
