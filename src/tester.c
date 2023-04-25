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


int main(int argc, char **argv)
{
    // http://who-t.blogspot.com/2013/09/libevdev-handling-input-events.html

    // open a device, as libevdev expects a file descriptor. You should have root permissions
    struct libevdev *dev;
    int fd;
    int rc;

    // open a new device from the file descriptor and initialize
    fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
    if (fd < 0)
        fprintf(stderr, "error opening file: %d %s\n", errno, strerror(errno));
    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0)
        fprintf(stderr, "error with setting rc: %d %s\n", -rc, strerror(-rc));


    // Use the getter functions to gather info about the device
    printf("Device: %s\n", libevdev_get_name(dev));
    printf("vendor: %x product: %x\n",
        libevdev_get_id_vendor(dev),
        libevdev_get_id_product(dev));

    // check for E_REL first, then the actual axes. Allows us to skip libevdev_has_event_type()
    if (libevdev_has_event_type(dev, EV_REL) &&
        libevdev_has_event_code(dev, EV_REL, REL_X) &&
        libevdev_has_event_code(dev, EV_REL, REL_Y) &&
        libevdev_has_event_code(dev, EV_KEY, BTN_LEFT) &&
        libevdev_has_event_code(dev, EV_KEY, BTN_MIDDLE) &&
        libevdev_has_event_code(dev, EV_KEY, BTN_RIGHT)) {
        printf("We have a mouse\n");
    }

    // get the next event and process it
    struct input_event ev;

    rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    if (rc < 0) {
        if (rc != -EAGAIN)
            fprintf(stderr, "error: %d %s\n", -rc, strerror(-rc));
    else if (rc == LIBEVDEV_READ_STATUS_SYNC)
        handle_syn_dropped(dev);
    else if (rc == LIBEVDEV_READ_STATUS_SUCCESS)
        printf("We have an event!\n%d (%s) %d (%s) value %d\n",
            ev.type, libevdev_event_type_get_name(ev.type),
            ev.code, libevdev_event_code_get_name(ev.type, ev.code),
            ev.value);
    }


    // Clean up
    libevdev_free(dev);
    close(fd);


	return 0;
}