# minion_gui

This is basically a wrapper around the FLTK library allowing communication using text messages in MINION, which is somewhat like a simplified JSON.

## Building the FLTK library

See [the instructions in the `fltk` directory](fltk/README-build-fltk) FLTK has its own licence, based on the GNU Library General Public License (LGPL), but allowing static linking.

### Requirements

For building minion_gui you need cmake and a C++20 compiler, such as GCC or Clang on Linux, MinGW on Windows or XCode on MacOS. On Ubuntu (24.04) this should be covered by `sudo apt install build-essential cmake`.

For running programs built using minion_gui you will need some system libs which are normally available on operating systems with a graphical user interfaces:

- Windows: no external dependencies
- MacOS: no external dependencies
- Linux (and other Unix systems): something like this ...
    - x11
    - xinerama
    - xfixes
    - xcursor
    - xkbcommon
    - pango
    - pangocairo
    - gobject
    - cairo
    - wayland-client
    - wayland-cursor
    - dbus
    - OpenGL?


*Tip*: The [rust-fltk project](https://github.com/fltk-rs/fltk-rs) may be helpful in determining dependencies, both for building and for running. Note, however, that the fltk configuration will affect this.

There is a small test programm in the cpp directory.
