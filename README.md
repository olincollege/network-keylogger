# Network Keylogger

## Lily Jiang, Richard Li, Aditi Vinod, Luke Witten

Virus.

# Libraries

## libevdev and libevdev-uinput

To gain access to both libraries:

`sudo apt-get install libevdev-dev`

This installation is only available in Ubuntu.

You need to compile the script with special flags in order for libevdev to be recognized correctly.

`-I/usr/include/libevdev-1.0` and `-levdev`

And make sure to run with sudo permissions
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

The program also _should_ account for 1 external keyboard, as long as it is plugged into one of the 3 USB ports of the Olin class of 2025 laptops. This external keyboard is read through a different event than the default laptop keyboard.

It also doesn't account for if the user connects/disconnects external keyboards while the program is running. It only checks once when the program begins.

All of this ^ is what is desired. But unfortunately I am struggling with getting it to recognize an external keyboard so...


Currently, you exit out of the loop by simply pressing `c`.