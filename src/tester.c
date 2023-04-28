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

#include <poll.h>



// If the device sends events faster than can be read, the kernel buffers will
// fill up and the kernel will skip events. This function resyncs the device
// and updates the internal state before reading events again
void handle_syn_dropped(struct libevdev *dev) {
    struct input_event ev;
    int rc = LIBEVDEV_READ_STATUS_SYNC;

    while (rc == LIBEVDEV_READ_STATUS_SYNC) {
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_SYNC, &ev);
        if (rc < 0) {
            if (rc != -EAGAIN)
                fprintf(stderr, "error %d (%s)\n", -rc, strerror(-rc));
            return;
        }

        printf("State change since SYN_DROPPED for %s %s value %d\n",
                libevdev_event_type_get_name(ev.type),
                libevdev_event_code_get_name(ev.type, ev.code),
                ev.value);
    }
}


int main(int argc, char **argv) {
    // open a file to write to. If it doesn't exist yet, this creates it
    FILE* data_storage = fopen("data.txt","w");
    if (data_storage == NULL) {
        printf("Error with opening the text file!");
        exit(1);
    }



    // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html

    // open a device, as libevdev expects a file descriptor. You should have root permissions
    struct libevdev *dev;
    struct libevdev *keyboard_dev;
    int fd;    // note that this will be for event0, which is the lid switch
    int rc;

    // open a new device from the file descriptor and initialize
    fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
    if (fd < 0) {
        printf("yuck %d\n", EXIT_FAILURE);
        fprintf(stderr, "error opening file: %d %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }
    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0) {
        fprintf(stderr, "error with setting rc: %d %s\n", -rc, strerror(-rc));
        fclose(data_storage);
        return EXIT_FAILURE;
    }


    // Use the getter functions to gather info about the device
    printf("Device: %s | vendor: %x | product: %x\n",
        libevdev_get_name(dev),
        libevdev_get_id_vendor(dev),
        libevdev_get_id_product(dev));

    // store data into text file
    if (data_storage != NULL) {
        fprintf(data_storage, "Device: %s | vendor: %x | product: %x\n",
            libevdev_get_name(dev),
            libevdev_get_id_vendor(dev),
            libevdev_get_id_product(dev));
    }

    // get keyboard inputs event file
    int keyboard_fd = open("/dev/input/event3", O_RDONLY|O_NONBLOCK);
    if (keyboard_fd < 0) {
        fprintf(stderr, "error opening event3 file: %d %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }
    rc = libevdev_new_from_fd(keyboard_fd, &keyboard_dev);
    if (rc < 0) {
        fprintf(stderr, "error with setting rc: %d %s\n", -rc, strerror(-rc));
        fclose(data_storage);
        return EXIT_FAILURE;
    }

    printf("does keyboard have key events? (1 = yes): %d\n", libevdev_has_event_type(keyboard_dev, EV_KEY));

    while (1) {
        struct input_event ev;

        // other options: LIBEVDEV_READ_FLAG_NORMAL
        rc = libevdev_next_event(keyboard_dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
        if (rc < 0) {
            // note that this section runs when no event is occurring, NOT necessarily when there is an error
            // printf("value of rc: %d\n", rc);
            if (rc != -EAGAIN)
                printf("1 error: %d %s\n", -rc, strerror(-rc));
                // exit(0);
        } else if (rc == LIBEVDEV_READ_STATUS_SYNC) {
            printf("uhhh x2\n");
            handle_syn_dropped(dev);
        } else if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
            // handle event here
            printf("We have an event!\n%d (%s) %d (%s) value %d\n",
                ev.type, libevdev_event_type_get_name(ev.type),
                ev.code, libevdev_event_code_get_name(ev.type, ev.code),
                ev.value);
            // exit(1);
        } else {
            printf("unsure \n");
        }
    }

    // Clean up
    libevdev_free(dev);
    close(fd);
    close(keyboard_fd);
    fclose(data_storage);

	return 0;
}