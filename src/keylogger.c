/**
 * Local keylogging
 */

#include "keylogger.h"

// for keylogging
#include <stdio.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

void begin_keylogger(void) {
  // IMPLEMENTATION HERE
}

void end_keylogger(void) {
  // IMPLEMENTATION HERE
}

void keylogger_test_1(void) {
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
  //   printf("err != 0\n");
  //   return err;

  // libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 1);
  // libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
  // libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 0);
  // libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

  // libevdev_uinput_destroy(uidev);
  // printf("Complete\n");
}

int main(void) {
  keylogger_test_1();
  return 0;
}