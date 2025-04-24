#include "connector.h"
#include "iofile.h"
#include "layout.h" //TODO: less ...
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <chrono>
#include <fmt/format.h>
#include <iostream>
using namespace std;

// *** The code for connecting to Go ***

// _cgo_export.h is auto-generated and has Go //export funcs
#include "_cgo_export.h"

std::string backend(const std::string data) {
  std::cout << "C callback got '" << data << "'" << std::endl;
  
  char *result = goCallback(data.c_str());
  return std::string{result};
}

void init(char* data0) {
  std::cout << "C says: init '" << data0 << "'" << std::endl;
  //printf("C says: init '%s'\n", data0);
  
  // Call actual start-up function
  //start(data0);
  
  std::string result2 = backend("C++ callback");
  std::cout << "C++ callback returned '" << result2 << "'" << std::endl;
  //printf("C callback returned '%s'\n", result);

	//TODO++ The real start!
	//initialize(data0);
}

// *** Initialization of the GUI ***

//TODO: Window might get a special callback ...
void main_callback(
    Fl_Widget *, void *)
{
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape

    //TODO: If changed data, ask about closing
    if (fl_choice("Are you sure you want to quit?", "continue", "quit", NULL))
        exit(0);
}

bool read_file(
    string &gui, string &fpath)
{
    //string fpath{"data/gui.minion"};
    //string fpath{"data/course_editor.minion"};
    if (readfile(gui, fpath)) {
        cout << "Reading " << fpath << endl;
    } else {
        cerr << "Error opening file: " << fpath << endl;
        return false;
    }
    return true;
}

int initialize(
    string guipath)
{
    string gui;
    if (read_file(gui, guipath)) {
        minion::MinionMap guidata;

        try {
            try {
                guidata = minion::read_minion(gui);
            } catch (minion::MinionException &e) {
                cerr << e.what() << endl;
                return 1;
            }

            tmp_run(guidata);
            return 0;

        } catch (const std::exception &ex) {
            cerr << "EXCEPTION: " << ex.what() << endl;
        } catch (const std::string &ex) {
            cerr << "ERROR: " << ex << endl;
        }
    }
    return 1;
}
