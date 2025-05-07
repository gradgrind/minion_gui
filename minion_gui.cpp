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

// This is used to manage the memory of a result from minion_read. It is
// freed before a call to backend(), whose result is then parsed and
// stored there.
// TODO: it should also be cleared on program exit.
minion_doc input_doc;

minion_value Callback(minion_value m)
{
    minion_free(input_doc);
    const char* cbdata{minion_dump(m, -1)};
    minion_free_item(m); // TODO: Is this correct here?
    char* cbresult = backend(cbdata);
    minion_tidy_dump();
    minion_doc mdoc = minion_read(cbresult);
    char* merr = minion_error(mdoc);
    if (merr) {
        //TODO: I might not want to crash ... Is there any less dramatic way
        // of handling this?
        cerr << "[BUG] backend() received invalid data:\n" << cbresult << endl;
        cerr << merr << endl;
        exit(1);
    }
    input_doc = mdoc;
    return mdoc.minion_item;;
}

minion_value Callback1(const char* widget, minion_value data)
{
    minion_value m = new_minion_map({
        {"CALLBACK", new_minion_string(widget)},
        {"DATA", data}});
    auto res = Callback(m);
}

minion_value Callback2(const char* widget, minion_value data, minion_value data2)
{
    minion_value m = new_minion_map({
        {"CALLBACK", new_minion_string(widget)},
        {"DATA", data},
        {"DATA2", data2}});
    return Callback(m);
}

//TODO ...
void tmp_run(
    minion_value data)
{
    auto dolist0 = data.get("GUI");
    if (holds_alternative<MinionList>(dolist0)) {
        auto dolist = get<MinionList>(dolist0);
        for (const auto& cmd : dolist) {
            GUI(get<minion_value>(cmd));
        }
    } else {
        cerr << "Input data not a GUI command list" << endl;
    }
}

void init(char* data0) {
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    string initgui{data0};
    minion::minion_value guidata;
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
        minion::minion_value guidata;

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
