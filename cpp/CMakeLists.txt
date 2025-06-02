cmake_minimum_required(VERSION 3.16)

project(minion_gui
    VERSION 1.0.0
    LANGUAGES CXX)

#set(FLTK_DIR "../fltk/build"
#    CACHE FILEPATH "FLTK installation or build directory")

#find_package(FLTK 1.4 CONFIG REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_subdirectory(minion_gui)
#add_subdirectory(test)

#add_executable(minion_gui_test
#    test/main.cpp
#    backend.h
#    callback.h
#    editform.cpp editform.h
#    functions.cpp functions.h
#    iofile.cpp iofile.h
#    layout.cpp layout.h
#    minion.cpp minion.h
#    minion_gui.cpp
#    rowtable.cpp rowtable.h
#    support_functions.cpp support_functions.h
#    textline.cpp
#    widget.cpp widget.h
#    widgets.cpp widgets.h
#    )

#set_property(TARGET minion_gui_test PROPERTY CXX_STANDARD 20)
#set_property(TARGET minion_gui PROPERTY CXX_STANDARD 20)
#set_property(TARGET minion_gui PROPERTY POSITION_INDEPENDENT_CODE ON)
#set_property(TARGET minion_gui PROPERTY CXX_VISIBILITY_PRESET hidden)

#include(GNUInstallDirs)
#install(TARGETS minion_gui_test
#    #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
#install(TARGETS minion_gui
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
#target_link_libraries(minion_gui_test PRIVATE fltk::fltk)
#target_link_libraries(minion_gui PRIVATE fltk::fltk)
