# minion_gui

WARNING: This repository is not maintained. It has been superseded by [mugui](https://github.com/gradgrind/mugui). It remains here mainly because the build scripts might be of some interest.

...

This is basically a wrapper around the FLTK library allowing communication using text messages in MINION, which is somewhat like a simplified JSON. minion_gui compiles to a dynamic library with a very simple C API. All commands to the GUI and callbacks from it are via MINION strings.

Currently the build scripts (using cmake) are only designed for Linux, but it should be possible to adapt them for other operating systems that are supported by FLTK.

A small test program which links to the library is available in cpp/test.

### Requirements

For building minion_gui you need cmake and a C++20 compiler, such as GCC or Clang on Linux, MinGW on Windows or XCode on MacOS. On Ubuntu (24.04) this should be covered by `sudo apt install build-essential cmake`. To compile FLTK, additional development packages may well be necessary.

For running programs built using minion_gui you will need some system libs which are normally available on operating systems with a graphical user interfaces:

- (Windows: no external dependencies?)
- (MacOS: no external dependencies?)
- Linux (and other Unix systems): on a fresh Linux Mint (22.1) installation no additional libraries were required.

## Building the FLTK library

See [the instructions in the `fltk` directory](fltk/README-build-fltk) FLTK has its own licence, based on the GNU Library General Public License (LGPL), but allowing static linking.

*Tip*: The [rust-fltk project](https://github.com/fltk-rs/fltk-rs) may be helpful in determining dependencies, both for building and for running. Note, however, that the fltk configuration will affect this.
