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

make sure to run with sudo permissions
`sudo ./a.out`


Note that each of the opened files corresponds to a different input event. So to get mouse data, you need to open a different file compared to if you want to get keyboard inputs. You can check the list of all events by running `sudo evtest` (note that you might have to install evtest first).

For the specific device I'm running (Ubuntu), the list is as follows:

/dev/input/event0:      Lid Switch
/dev/input/event1:      Power Button
/dev/input/event2:      Sleep Button
/dev/input/event3:      AT Translated Set 2 keyboard
/dev/input/event4:      Integrated_Webcam_HD: Integrate
/dev/input/event5:      Dell WMI hotkeys
/dev/input/event6:      DELL09C2:00 0488:121F Mouse
/dev/input/event7:      PS/2 Generic Mouse
/dev/input/event17:     Intel HID events
/dev/input/event18:     Intel HID 5 button array
/dev/input/event19:     DELL09C2:00 0488:121F Touchpad
/dev/input/event20:     DELL09C2:00 0488:121F UNKNOWN
/dev/input/event21:     Video Bus
/dev/input/event22:     Video Bus
/dev/input/event23:     DP-3
/dev/input/event24:     HDA Intel PCH Headphone Mic
/dev/input/event25:     HDA Intel PCH HDMI/DP,pcm=3
/dev/input/event26:     HDA Intel PCH HDMI/DP,pcm=7
/dev/input/event27:     HDA Intel PCH HDMI/DP,pcm=8
/dev/input/event28:     HDA Intel PCH HDMI/DP,pcm=9
/dev/input/event29:     HDA Intel PCH HDMI/DP,pcm=10
