# minion_gui

The basic idea is to control an FLTK-based GUI via a simple interface using MINION messages.

[MINION](minion.md) is a simple structured notation with some resemblance to JSON. Its primary design aims were to be easier to type manually than JSON and to avoid some of JSON's "woolly" areas (number type and precisiom, unicode escapes as UTF-16). It only supports the data types string, map/object and list/array. Quotation of strings is often unnecessary, especially for simple object keys. Comments and multiline strings are supported in a fairly clear way.

It is not intended to be a comprehensive wrapper around FLTK, but to provide a convenient declarative interface to some of the widgets. Also some custom, higher-level, widgets are defined for convenience.
As far as layouts are concerned, the use of the flex and grid widgets is preferred.

The interface to Go is intentionally simple, consisting, in essence of only a single function, `MinionGui`. This passes an initial MINION message to the wrapper describing the GUI widgets and starting the event loop. Subsequently, the GUI takes over as "front-end", calling back into the Go code when needed for handling GUI events (button clicks, etc.). To manage these callbacks, `MinionGui` takes a second argument which is a Go function to handle them. This function takes a single MINION string - from the front-end – as argument and returns a single MINION string – to the front-end.

`MinionGui` returns only when the front-end (GUI) is closed.

Callbacks happen within the main thread, so the front-end is blocked while they are being processed. Thus they should return quickly.

## Longer-running callbacks

To perform more extensive back-end processing, a separate thread should be used. The front-end must be aware that a longer-running back-end process is under way, because it is designed around a single callback, multiple concurrent callbacks are not supported. To handle this, a special flag is returned by a callback when it returns without having completed. The front-end then sends further "follow-up" callbacks to poll the progress of the back-end process. If the back-end process exceeds a certain time-limit, the front-end displays a "dialog" pop-up which can show progress reports (if these are sent by the back-end) and allow the cancellation of the back-end process, if desired – and if supported by the back-end.

*TODO*: Include a helper function to manage such longer-running callbacks?

## Details

### minion_gui.go

`MinionGui` saves the `callback` function in the variable `do_callback`, where it can be used in actual callbacks by the function `goCallback`, which is exported via cgo to the C++ front-end. The string argument `initdata` is MINION data describing the initial GUI configuration. This is passed to the C++ function `init` in `minion_gui.cpp` after creating a new C-string from it. The same static variable, `resultptr`, is used for this C-string as for the C-string returned in callbacks by `goCallback`. That means the C-string from `initdata` is freed by the first callback. On return from `init` the C-string is also freed. At this point the front-end has been closed, so the program is exiting. Nevertheless, for the sake of tidiness, the C-string is freed here, whether it be the last callback result or that from `initdata` (if there was no callback).

`goCallback` receives and returns C-strings. To connect to the Go callback function passed to `MinionGui` these must be converted to or from Go strings. The received C-string is managed by the C++ side (function `backend` in `minion_gui.cpp`) and is valid for the duration of the `goCallback` call. The returned C-string is created by `goCallback` and must remain valid until the front-end has dealt with it. The C++ function `backend` copies it into an `std::string` immediately on return from `goCallback`, but to be safe, no immediate attempt is made to free the C-string on the Go side. It is freed only when `goCallback` is called again – which should be good enough, given that only one callback is permitted at any time.
