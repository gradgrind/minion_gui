package minion_gui

import "fmt"

// #include <stdlib.h>
// #include "connector.h"
import "C"
import "unsafe"

var result string
var resultptr *C.char
var do_callback func(string)string

// The //export allows C to call the Go func

//export goCallback
func goCallback(callback *C.cchar_t) *C.char {
    cb := C.GoString(callback)
	fmt.Printf("goCallback got '%s'\n", cb)
	C.free(unsafe.Pointer(resultptr))
	result = handleCallback(cb)
	//result = do_callback(cb)
	resultptr = C.CString(result)
	return resultptr
}

//TODO--
func handleCallback(data string) string {
	fmt.Printf("handleCallback got '%s'\n", data)
	return "handleCallback result"
}

func MinionGui(initdata string, callback func(string)string) {
	do_callback = callback
	fmt.Printf("Go says: calling C init ...\n")
	
	result = "data/course_editor.minion"
	resultptr = C.CString(result)
	
	C.init(resultptr)
	fmt.Printf("Go says: Finished\n")

	C.free(unsafe.Pointer(resultptr))
}
