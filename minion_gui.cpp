#include "connector.h"
#include "backend.h"
#include "callback.h"
#include "dispatcher.h"
#include "minion.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <fmt/format.h>
#include <iostream>
using namespace std;
using namespace minion;

// This is used to manage the memory of a result from minion_read. It is
// freed before a call to backend(), whose result is then parsed and
// stored there.
MinionValue input_value;

// This is used for reading (deserializing) MINION messages.
InputBuffer input_buffer;

// This is used for writing (serializing) MINION messages.
DumpBuffer dump_buffer;

//TODO: In the case of an error, intermediate MValues could cause
// memory leaks. Should there be a MinionValue(&)-based builder which
// can avoid this?

void Callback(MValue m)
{
    input_value = {};
    const char* cbdata;
    {
        MinionValue mv = m;
        cbdata = dump_buffer.dump(mv);
        // TODO: Is the freeing of m correct here?
    }
    char* cbresult = backend(cbdata);
    //TODO? minion_tidy_dump();
    input_buffer.read(input_value, cbresult);
    //TODO: Handle errors (thrown MinionError)?
}

void Callback1(const char* widget, MValue data)
{
    Callback(
        {
            MPair{"CALLBACK", widget},
            MPair{"DATA", data}
        }
    );
}

void Callback2(const char* widget, MValue data, MValue data2)
{
    Callback(
        {
            MPair{"CALLBACK", widget},
            MPair{"DATA", data},
            MPair{"DATA2", data2}
        }
    );
}



//TODO: extend minion with helper methods?
//TODO ... What should the final form be?
void tmp_run(
    MValue data)
{
    auto dolist0 = data.map_search("GUI");
    if (!dolist0.is_null()) {
        if (auto dolist = dolist0.m_list()) {
            for (const auto& cmd : *dolist) {
                GUI(cmd);
            }
            return;
        }
    }
    //TODO: throw?
    cerr << "Input data not a GUI command list" << endl;
}

minion::InputBuffer minion_input; // for parsing minion

void init(char* data0) {
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    string initgui{data0};
    minion::MinionValue guidata;
    if (auto e = minion_input.read(guidata, initgui)) {
        cerr << e << endl;
        return;
    }
    tmp_run(guidata);
    return;

    /* *** This would handle a file path instead of the actual data ***
    #include "iofile.h"

    string initgui;

    if (readfile(initgui, data0)) {
        cout << "Reading " << fpath << endl;

        ...

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
