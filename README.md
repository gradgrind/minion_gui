# minion_gui

This is basically a wrapper around the FLTK library allowing communication using text messages in MINION, which is somewhat like a simplified JSON.

## Building the FLTK library

The build script is derived from that used in [go-fltk](https://pkg.go.dev/github.com/pwiecz/go-fltk), which is MIT-licenced. FLTK has its own licence, based on the GNU Library General Public License (LGPL) – see the lib directory.

### Requirements

For building minion_gui, besides the Golang compiler, you also need a C++20 compiler, such as GCC or Clang on Linux, MinGW on Windows and XCode on MacOS.

To build the FLTK libraries for your platform it might be enough to call `go generate` from the root of the minion_gui source tree. On Linux, additional development libraries might well need to be installed.

If the build procedure doesn't work for you, you can modify `fltk-build.go` yourself.

For running programs built using minion_gui you will need some system libs which are normally available on operating systems with a graphical user interfaces:

- Windows: no external dependencies
- MacOS: no external dependencies
- Linux (and other Unix systems - not tested):
    - X11
    - Xrender
    - Xcursor
    - Xfixes
    - Xext
    - Xft
    - Xinerama
    - XKBCommon
    - Wayland
    - libdecor
    - DBus
    - OpenGL

*Tip*: The [rust-fltk project](https://github.com/fltk-rs/fltk-rs) may be helpful in determining dependencies, both for building and for running. Note, however, that the configuration in `fltk-build.go` will affect the libraries needed.

At present, also the "fmt" library is used, so this may also need to be installed.

## Usage

TODO
