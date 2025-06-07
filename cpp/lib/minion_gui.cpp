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

const map<string, new_function> new_function_map{
    //
    {"Window", W_Window::make},
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
    {"Dialog", W_Dialog::make}
    //
};

void GUI(
    MList* cmd)
{
    string s;
    if (!cmd->get_string(0, s)) {
        value_error("Invalid GUI command: ", *cmd);
    }
    if (s == "WIDGET") {
        // Handle widget methods
        string w;
        if (cmd->get_string(1, w)) {
            Widget::get_widget(w)->handle_methods(cmd, 2);
            return;
        }
        value_error("Invalid WIDGET command: ", *cmd);
    }
    if (s == "NEW") {
        // Make a new widget
        Widget::new_widget(cmd);
        return;
    }
    auto f = function_map.at(s);
    f(cmd);
}

// This is used to manage the memory of a result from minion_read. It is
// freed before a call to backend(), whose result is then parsed and
// stored there.
//TODO: What about recursive calls? ...
MValue input_value;

string dump_value(
    MValue m)
{
    minion::Writer writer(m, 0);
    return string{writer.dump()};
}

void value_error(
    string msg, MValue m)
{
    throw msg + dump_value(m);
}

void do_commands(
    MList* dolist)
{
    auto len = dolist->size();
    for (size_t i = 0; i < len; ++i) {
        if (auto command = dolist->get(i).m_list())
            GUI(command->get());
        else {
            value_error("Invalid GUI command: ", **command);
        }
    }
}

void Callback(
    MValue m)
{
    input_value = {}; // clear the result
    //const char* cbdata;
    const char* cbresult;
    {
        minion::Writer writer(m, -1);
        //printf("callback got '%s'\n", writer.dump_c());
        auto cbdata = writer.dump_c();
        cbresult = backend(cbdata);
    }

    input_value = minion::Reader::read(cbresult);

    //cbdata = dump_buffer.dump(m, -1); // compact form
    //const char* cbresult = backend(cbdata);
    //input_value = input_buffer.read(cbresult);
    if (auto dolist0 = input_value.m_list())
        do_commands(dolist0->get());
    else if (const char* e = input_value.error_message())
        throw e;
    else
        throw string{"Invalid callback result: "}.append(cbresult);
}

void Callback0(
    string& widget)
{
    MList m({widget});
    Callback(m);
}

void Callback1(
    string& widget, MValue data)
{
    MList m({widget, data});
    Callback(m);
}

void Callback2(string& widget, MValue data, MValue data2)
{
    MList m({widget, data, data2});
    Callback(m);
}

//TODO
void Init(
    const char* data0)
{
    Widget::init_settings();
    //std::cout << "C says: init '" << data0 << "'" << std::endl;

    //string initgui{data0};
    auto guidata = minion::Reader::read(data0);
    //minion::MValue guidata = minion_input.read(initgui);
    if (auto e = guidata.error_message()) {
        cerr << e << endl;
        return;
    }
    //    try {
    {
        auto dolist0 = guidata.m_list();
        if (dolist0)
            do_commands(dolist0->get());
        else
            value_error("Input data not a GUI command list: ", guidata);
        //    } catch (string& e) {
        //        cerr << "THROWN 1: " << e << endl;
        //    } catch (char const* e) {
        //        cerr << "THROWN 2: " << e << endl;
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
