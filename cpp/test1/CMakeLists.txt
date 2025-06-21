add_executable(minion_gui_test1
    main.cpp
#    ../include/backend.h
)

target_include_directories(minion_gui_test1
  PUBLIC ../include
  PRIVATE ../lib)

set_property(TARGET minion_gui_test1 PROPERTY CXX_STANDARD 20)
set_property(TARGET minion_gui_test1 PROPERTY BUILD_WITH_INSTALL_RPATH ON)
set_property(TARGET minion_gui_test1 PROPERTY INSTALL_RPATH "$ORIGIN/../lib")

target_link_libraries(minion_gui_test1 PRIVATE minion_gui)
