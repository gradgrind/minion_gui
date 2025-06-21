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

add_subdirectory(lib)
#add_subdirectory(test)
add_subdirectory(test1)
