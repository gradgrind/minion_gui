cmake_minimum_required(VERSION 3.16)

project(minion_x
    VERSION 5.0.0
    LANGUAGES C CXX)

add_executable(minion_x
    minion.c
    minion.h
    main.c
    iofile.cpp iofile.h
    )

set_property(TARGET minion_x PROPERTY C_STANDARD 99)

include(GNUInstallDirs)
install(TARGETS minion_x
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
