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
#include <unistd.h>

void begin_keylogger(void) {
  // IMPLEMENTATION HERE
}

void end_keylogger(void) {
  // IMPLEMENTATION HERE
}

int keylogger_test_1(void) {
  // int err;
  // struct libevdev *dev;
  // struct libevdev_uinput *uidev;

  // dev = libevdev_new();
  // libevdev_set_name(dev, "fake keyboard device");

  // libevdev_enable_event_type(dev, EV_KEY);
  // libevdev_enable_event_code(dev, EV_KEY, KEY_A, NULL);

  // err = libevdev_uinput_create_from_device(dev,
  //     LIBEVDEV_UINPUT_OPEN_MANAGED,
  //     &uidev);

  // if (err != 0)
  //   printf("err = %d\n", err);
  //   return err;

  // libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 1);
  // libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
  // libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 0);
  // libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

  // libevdev_uinput_destroy(uidev);
  
  int err;
  int fd, new_fd, uifd;
  struct libevdev *dev;
  struct libevdev_uinput *uidev;
  struct input_event ev[2];
  fd = open("/dev/input/event0", O_RDONLY);
  printf("1.1\n");
  if (fd < 0) {
      printf("error at 1\n");
      return err;
  }
  err = libevdev_new_from_fd(fd, &dev);
  if (err != 0) {
      printf("error at 2\n");
      return err;
  }
  uifd = open("/dev/uinput", O_RDWR);
  if (uifd < 0) {
      printf("error at 3\n");
      return -errno;
  }
  err = libevdev_uinput_create_from_device(dev, uifd, &uidev);
  if (err != 0) {
      printf("error at 4\n");
      return err;
  }
  // post a REL_X event
  err = libevdev_uinput_write_event(uidev, EV_REL, REL_X, -1);
  if (err != 0) {
      printf("error at 5\n");
      return err;
  }
  libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
  if (err != 0) {
      printf("error at 6\n");
      return err;
  }
  libevdev_uinput_destroy(uidev);
  libevdev_free(dev);
  close(uifd);
  close(fd);
  printf("Complete\n");
  return 0;
}

int main(void) {
  printf("1\n");
  int x = keylogger_test_1();
  printf("returns: %d\n", x);
  printf("2\n");
  return 0;
}
