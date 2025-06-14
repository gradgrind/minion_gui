package mugui

import (
	"fmt"
	"unsafe"
)

/*
#cgo CFLAGS: -I.
#cgo LDFLAGS: -L./lib -lminion_gui -Wl,-rpath=${SRCDIR}/lib
// Or this for a "standalone" app with the library relative to the executable:
//#cgo LDFLAGS: -L./lib -lminion_gui -Wl,-rpath=$ORIGIN/lib
#include <stdlib.h>
#include "connector.h"
*/
import "C"

var result string
var resultptr *C.char
var do_callback func(string) string

// The //export allows C to call the Go func.
// The C-strings passed to the C++ code are managed here. The same variable,
// `resultptr` is used for the initial message and the callback results. This
// simplifies the freeing.

//export goCallback
func goCallback(callback *C.cchar_t) *C.cchar_t {
	// The C-string `callback` is managed on the C++ side
	cb := C.GoString(callback)
	fmt.Printf("goCallback got '%s'\n", cb)
	C.free(unsafe.Pointer(resultptr))
	result = do_callback(cb)
	resultptr = C.CString(result)
	return resultptr
}

func MinionGui(initdata string, callback func(string) string) {
	do_callback = callback
	result = initdata
	resultptr = C.CString(result)
	C.init(resultptr)
	fmt.Printf("Go says: Finished\n")
	C.free(unsafe.Pointer(resultptr))
}
