#include "backend.h"
#include <iostream>

// _cgo_export.h is auto-generated and has Go //export funcs
#include "_cgo_export.h"

std::string backend(const std::string data) {
    std::cout << "C callback got '" << data << "'" << std::endl;
    
    char *result = goCallback(data.c_str());
    return std::string{result};
  }
  
  