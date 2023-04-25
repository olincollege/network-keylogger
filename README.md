# Network Keylogger

Virus.

# Libraries

## libevdev and libevdev-uinput

To gain access to both libraries:

`sudo apt-get install libevdev-dev`

This installation is only available in Ubuntu.

<!-- Note that the actual line within the scripts to include the library might not
be recognized. If you are using VS Code as your text editor, follow these
steps to solve:

1. Open the command palette (Ctrl + Shift + P)
2. Go to `C/C++: Edit Configurations (UI)`
3. Scroll down to the `Include path` section, and in the text input, type the
   path of your installed library on a new line. A common path is
   `/usr/include/libevdev-1.0/`.

If you aren't using VS Code... good luck. -->

You need to compile the script with flags in order for libevdev to be recognized correctly.

gcc main.c keylogger.c -I/usr/include/libevdev-1.0 -levdev


for my personal copy/paste:
gcc tester.c -I/usr/include/libevdev-1.0 -levdev
