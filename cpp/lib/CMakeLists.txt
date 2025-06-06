set(FLTK_DIR "../../fltk/build"
    CACHE FILEPATH "FLTK installation or build directory")

find_package(FLTK 1.4 CONFIG REQUIRED)

add_library(minion_gui SHARED
#  ../include/backend.h
  callback.h
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
  dialogs.cpp
  dialogs.h
)

target_include_directories(minion_gui PUBLIC ../include)

set_property(TARGET minion_gui PROPERTY CXX_STANDARD 20)
set_property(TARGET minion_gui PROPERTY POSITION_INDEPENDENT_CODE ON)
#set_property(TARGET minion_gui PROPERTY CXX_VISIBILITY_PRESET hidden)


include(GNUInstallDirs)
install(TARGETS minion_gui
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(minion_gui PRIVATE fltk::fltk)
