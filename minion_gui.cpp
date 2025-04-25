#include "connector.h"
#include "layout.h" //TODO: less ...
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
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
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    string initgui{data0};
    minion::MinionMap guidata;
    try {
        try {
            guidata = minion::read_minion(initgui);
        } catch (minion::MinionException &e) {
            cerr << e.what() << endl;
            return;
        }

        tmp_run(guidata);
        return;

    } catch (const std::exception &ex) {
        cerr << "EXCEPTION: " << ex.what() << endl;
    } catch (const std::string &ex) {
        cerr << "ERROR: " << ex << endl;
    }

    /* *** This would handle a file path instead of the actual data ***
    #include "iofile.h"

    string initgui;

    if (readfile(initgui, guipath)) {
        cout << "Reading " << fpath << endl;
        minion::MinionMap guidata;

        try {
            try {
                guidata = minion::read_minion(gui);
            } catch (minion::MinionException &e) {
                cerr << e.what() << endl;
                return;
            }

            tmp_run(guidata);
            return;

        } catch (const std::exception &ex) {
            cerr << "EXCEPTION: " << ex.what() << endl;
        } catch (const std::string &ex) {
            cerr << "ERROR: " << ex << endl;
        }
    } else {
        cerr << "Error opening file: " << guipath << endl;
    }
    */
}

//TODO: Window might get a special callback ... what to do with this?
void main_callback(
    Fl_Widget *, void *)
{
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape

    //TODO: If changed data, ask about closing
    if (fl_choice("Are you sure you want to quit?", "continue", "quit", NULL))
        exit(0);
}
