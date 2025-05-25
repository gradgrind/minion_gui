#include "connector.h"
#include "backend.h"
#include "callback.h"
#include "dispatcher.h"
#include "minion_gui.h"
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
        new MMap{
            MPair{"CALLBACK", new MString{widget}},
            MPair{"DATA", data}
        }
    );
}

void Callback2(const char* widget, MValue data, MValue data2)
{
    Callback(
        new MMap{
            MPair{"CALLBACK", new MString{widget}},
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
    if (MMap* m0 = data.m_map()) {
        MValue dolist0 = m0->get("GUI");
        if (!dolist0.is_null()) {
            if (MList* dolist = dolist0.m_list()) {
                auto len = dolist->size();
                for (int i = 0; i < len; ++i) {
                    GUI(dolist->get(i));
                }
                return;
            }
        }
    }
    throw string{"Input data not a GUI command list: "}
        + dump_buffer.dump(data, 0);
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
