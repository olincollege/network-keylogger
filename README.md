# Network Keylogger

## Authors: Aditi Vinod, Lily Jiang, Luke Witten, and Richard Li

A keystroke logger that runs in the background and sends information over the network.
With a virus-like functionality, the program cannot easily be noticed, identified,
and/or killed.

# Usage

## Libraries

### `libevdev`

The keystroke logging functionality requires the `libevdev` library, which can be
installed with the following command:

`sudo apt-get install libevdev-dev`

Note that compiling programs that rely on this library requires special flags:
`-I/usr/include/libevdev-1.0` and `-levdev`

And to access the keyboard events, you must have root permissions. If you're compiling
in the command line with something like GCC, you can run the executable with sudo
permissions: `sudo ./a.out`.

Alternatively, if you're compiling with CMake, you can't specify sudo permissions so
you must run the following line of code:

`sudo usermod -aG input [your username]`

and restart your device.


# Debugging

Note that in this program, the input event we harness has the path `/dev/input/event3`.
Event 3 corresponds to the built-in keyboard on the Dell Precision 3551 laptop, but it
may not be the same for other devices. If the program isn't working, try checking what
your events represent by running `sudo evtest` and changing the event path to
correspond to the keyboard.

