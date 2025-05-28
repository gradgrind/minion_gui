#include "connector.h"
#include "backend.h"
#include "callback.h"
#include "dispatcher.h"
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
MValue input_value;

// This is used for reading (deserializing) MINION messages.
InputBuffer input_buffer;

// This is used for writing (serializing) MINION messages.
DumpBuffer dump_buffer;
const char* dump_value(MValue m) {
    return dump_buffer.dump(m, 0);
}

void Callback(MValue m)
{
    input_value = {}; // clear the result
    const char* cbdata;
        cbdata = dump_value(m);
    char* cbresult = backend(cbdata);
    //TODO? minion_tidy_dump();
    input_value = input_buffer.read(cbresult);
    if (const char* e = input_value.error_message())
        throw e;
}

void Callback1(string& widget, MValue data)
{
    MMap m{{{"CALLBACK", widget}, {"DATA", data}}};
    Callback(m);
}

void Callback2(string& widget, MValue data, MValue data2)
{
    MMap m{{{"CALLBACK", widget}, {"DATA", data}, {"DATA2", data2}}};
    Callback(m);
}

//TODO ... What should the final form be?
void tmp_run(
    MValue data)
{
    auto mp = data.m_map();
    if (mp) {
        MMap* m0 = mp->get();
        MValue dolist0 = m0->get("GUI");
        if (!dolist0.is_null()) {
            auto lp = dolist0.m_list();
            if (lp) {
                MList* dolist =lp->get();
                auto len = dolist->size();
                for (int i = 0; i < len; ++i) {
                    GUI(dolist->get(i));
                }
                return;
            }
        }
    }
    throw string{"Input data not a GUI command list: "}
        + dump_value(data);
}

minion::InputBuffer minion_input; // for parsing minion

void init(char* data0) {
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    string initgui{data0};
    minion::MValue guidata = minion_input.read(initgui);
    if (auto e = guidata.error_message()) {
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
