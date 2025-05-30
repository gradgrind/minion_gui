cmake_minimum_required(VERSION 3.16)

project(minion_gui
    VERSION 1.0.0
    LANGUAGES CXX)

set(FLTK_DIR "../fltk/build"
    CACHE FILEPATH "FLTK installation or build directory")

find_package(FLTK 1.4 CONFIG REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(minion_gui
    main.cpp
    callback.h
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

include(GNUInstallDirs)
install(TARGETS minion_gui
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(minion_gui PRIVATE fltk::fltk)
