#include "connector.h"
#include "backend.h"

// _cgo_export.h is auto-generated and has Go //export funcs
#include "_cgo_export.h"

void init(cchar_t* data0) {
    SetCallbackFunction(goCallback);
    Init(data0);
}

/*
char* backend(const char* data) {

    SetCallbackFunction(goCallback);
    
    // The argument, data, must be valid for the duration of this function.
    //std::cout << "C -> Go: '" << data << "'" << std::endl;
    
    char* ret = goCallback(data);
    //std::cout << "Go -> C: '" << ret << "'" << std::endl;
    return ret;
    // The memory referenced by the return pointer is available until the
    // next call to backend().
}
*/  
  