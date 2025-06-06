#include "backend.h"
#include "callback.h"
#include "dialogs.h"
#include "functions.h"
#include "layout.h"
#include "widget.h"
#include "widgets.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <map>
#include <string_view>
using namespace std;
using namespace minion;

const char* (*backend)(const char*);

void SetCallbackFunction(
    const char* (*backend_pointer)(const char*) )
{
    backend = backend_pointer;
}

const map<string, new_function> new_function_map{{"Window", W_Window::make},
                                                 {"Grid", W_Grid::make},
                                                 {"Hlayout", W_Grid::make_hlayout},
                                                 {"Vlayout", W_Grid::make_vlayout},
                                                 {"PushButton", W_PushButton::make},
                                                 {"Box", W_Box::make},
                                                 {"Hline", W_Hline::make},
                                                 {"Vline", W_Vline::make},
                                                 {"Label", W_Label::make},
                                                 {"Choice", W_Choice::make},
                                                 {"Output", W_Output::make},
                                                 {"PopupEditor", W_PopupEditor::make},
                                                 {"Checkbox", W_Checkbox::make},
                                                 {"List", W_List::make},
                                                 {"TextLine", W_TextLine::make},
                                                 {"RowTable", W_RowTable::make},
                                                 {"EditForm", W_EditForm::make},
                                                 {"Dialog", W_Dialog::make}};

void GUI(
    MMap* mmap)
{
    string s;
    if (mmap->get_string("NEW", s)) {
        // Make a new widget
        Widget::new_widget(s, mmap);
    } else if (mmap->get_string("WIDGET", s)) {
        // Handle widget methods
        auto w = Widget::get_widget(s);
        w->handle_methods(mmap);
    } else if (mmap->get_string("FUNCTION", s)) {
        // Some other function
        auto f = function_map.at(s);
        f(mmap);
    } else {
        value_error("Invalid GUI parameters: ", *mmap);
    }
}

// Pass a message to the back-end. This can be an event/callback, the
// reponse to a query, or whatever.

// There need to be two kinds of "message":
// 1) Let's call this a virtual override. It is a call to the back-end,
//    perhaps with a result (like 0 or 1 for event handlers), and is
//    blocking – so it should execute quickly. Unfortunately this seems
//    very difficult to implement, because it might also need to query
//    the front-end or perform other gui operations. Thus it entails
//    a calling back and forth between back-end and front-end.
// 2) Let's call this a trigger. It sets an operation in the back-end
//    going, but doesn't wait for it to finish. Any resulting calls to
//    the front-end could be picked up by an idle function.
// For the moment I would like to implement just normal callbacks, i.e.
// asynchronous calls. Where event handlers are necessary, I would first
// consider extending the C++ widgets.

//TODO
MMap* message(
    MMap* data)
{
    return data;
}

void to_back_end(
    MMap* data)
{
    //MMap* result = message(data);
    auto dolist0 = data->get("DO");
    if (MList* mlist = dolist0.m_list()->get()) {
        size_t n = mlist->size();
        for (size_t i = 0; i < n; ++i) {
            MValue cmd = mlist->get(i);
            if (MMap* mmap = cmd.m_map()->get())
                GUI(mmap);
        }
    }
    // Any back-end function which can take more than about 100ms should
    // initiate a timeout leading to a modal "progress" dialog.
    // Any data generated while such a callback is operating (i.e. before
    // it returns a completion code) should be fetched and run by an idle
    // handler. Any data generated outside of this period is probably an
    // error – the back-end should not be doing anything then!
}


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
void value_error(
    string msg, MValue m)
{
    throw msg + dump_value(m);
}

bool has_GUI(
    MMap* m0)
{
    MValue dolist0 = m0->get("GUI");
    if (!dolist0.is_null()) {
        auto lp = dolist0.m_list();
        if (lp) {
            MList* dolist = lp->get();
            auto len = dolist->size();
            for (size_t i = 0; i < len; ++i) {
                if (auto mm = dolist->get(i).m_map())
                    GUI(mm->get());
                else {
                    value_error("Invalid GUI command: ", **mm);
                }
            }
            return true;
        }
        value_error("GUI expects a list of commands: ", *m0);
    }
    return false;
}

void Callback(
    MValue m)
{
    input_value = {}; // clear the result
    const char* cbdata;
    cbdata = dump_buffer.dump(m); // compact form
    const char* cbresult = backend(cbdata);
    input_value = input_buffer.read(cbresult);
    if (const char* e = input_value.error_message())
        throw e;
    if (auto mm = input_value.m_map()) {
        auto mmap = mm->get();
        if (!has_GUI(mmap))
            GUI(mmap);
    } else {
        throw string{"Invalid callback result: "}.append(cbresult);
    }
}

void Callback0(
    string& widget)
{
    MMap m{{{"CALLBACK", widget}}};
    Callback(m);
}

void Callback1(
    string& widget, MValue data)
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
        if (has_GUI(m0))
            return;
    }
    value_error("Input data not a GUI command list: ", data);
}

minion::InputBuffer minion_input; // for parsing minion

//TODO
void Init(
    const char* data0)
{
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    //TODO?: something like: Fl::background(250, 250, 200);

    string initgui{data0};
    minion::MValue guidata = minion_input.read(initgui);
    if (auto e = guidata.error_message()) {
        cerr << e << endl;
        return;
    }
    try {
        tmp_run(guidata);
    } catch (string& e) {
        cerr << "THROWN: " << e << endl;
    } catch (char const* e) {
        cerr << "THROWN: " << e << endl;
    }
    auto e = Widget::clear();
    if (!e.empty())
        cerr << e << endl;
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
