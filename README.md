# Network Keylogger

## Authors: Aditi Vinod, Lily Jiang, Luke Witten, and Richard Li

A keystroke logger that runs in the background and sends information over a tcp socket network.
With a virus-like functionality, the program cannot easily be noticed, identified,
and/or killed.

# Usage

## Libraries
The `libevdev` library was used in C17 in order to collect event information from a device.

### `libevdev`

The keystroke logging functionality requires the `libevdev` library, which can be
installed with the following command:

`sudo apt-get install libevdev-dev`

Note that compiling programs that rely on this library requires special flags:
`-I/usr/include/libevdev-1.0` and `-levdev`

Root permissions are necessary to access the keyboard events that the keylogger 
utilizes. When compiling in the commandline, for example, when using GCC, the 
executable must be run with sudo permissions: `sudo ./a.out`.

However, when compiling iwth CMake, since sudo permissions cannot be specified,
the user must be added to the input group by running the following in the terminal:
`sudo usermod -aG input [your username]`.
Restart the device afterwards for the requried change to take place.

## Setting Up the Server
In order to run the local server that can receive information from the keylogger
and client, run `run_server.c` in a terminal window.

## Creating a Client
To start logging keys from a client, run `main.c`. This should set up the client
side of the server and start the keylogger. If the keylogger is being run on the
same device as the server, a different terminal must be used.


# Debugging

Note that in this program, the input event harnessed has the path `/dev/input/event3`.
Event 3 corresponds to the built-in keyboard on the Dell Precision 3551 laptop, but it
may not be the same for other devices. If the program isn't working, try checking the
events' representations by running `sudo evtest` and changing the event path to
correspond to the keyboard.

