cmake_minimum_required(VERSION 3.16)

project(minion_gui
    VERSION 1.0.0
    LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(minion_gui
    examples/main.cpp
    backend.cpp backend.h
    callback.h connector.h
    dispatcher.cpp dispatcher.h
    editform.cpp editform.h
    functions.cpp functions.h
    iofile.cpp iofile.h
    layout.cpp layout.h
    minion.cpp minion.h
    minion_gui.cpp
    rowtable.cpp rowtable.h
    support_functions.cpp support_functions.h
    textline.cpp
    widget.cpp widget.h
    widgets.cpp widgets.h
    )

set_property(TARGET minion_gui PROPERTY CXX_STANDARD 20)
