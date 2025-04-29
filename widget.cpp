#include "widget.h"
#include "functions.h"
#include <FL/Fl_Window.H>
#include <functional>
#include <stdexcept>
#include <FL/Fl_Group.H>
using namespace std;
using namespace minion;

//TODO: more sub-classes, and their method handlers ...

class W_Box : public Widget
{
public:
    W_Box(MinionMap parammap) : Widget{parammap}
    {}

    static W_Box* make(MinionMap &parammap);
};

class W_Label : public Widget
{
public:
    W_Label(MinionMap parammap) : Widget{parammap}
    {}

    static W_Label* make(MinionMap &parammap);
};

const map<string_view, function<Widget*(MinionMap&)>>widget_type_map{
    {"Box", W_Box::make},
    {"Label", W_Label::make}
};

void handle_methods(
    Widget* w, MinionMap mmap)
{
    auto dolist = mmap.get("DO");
    if (holds_alternative<MinionList>(dolist)) {
        MinionList do_list = get<MinionList>(dolist);
        for (const auto& cmd : do_list) {
            //TODO error check
            MinionList mlist = get<MinionList>(cmd);
            w->handle_method(mlist);
        }
    } else if (dolist.index() != 0) {
        string s;
        minion::dump(s, dolist, 0);
        throw "Invalid DO list: " + s;
    }
}

void process_command(
    MinionMap parammap)
{
    //TODO: widget type?
    string param0;
    if (parammap.get_string("NEW", param0)) {
        function<Widget*(MinionMap&)> wmake;
        try {
            wmake = widget_type_map.at(param0);
        } catch (std::out_of_range) {
            throw "Unknown widget type: " + param0;
        }

        string wname;
        if (parammap.get_string("NAME", wname) && !wname.empty()) {
            if (Widget::get_widget_data(wname)) {
                throw "NEW, widget (" + param0 + ": " + wname + ") redefined";
            }
            auto w = wmake(parammap);

            string parent;
            if (parammap.get_string("PARENT", parent) && !parent.empty()) {
                
                auto p = static_cast<Fl_Group*>(Widget::get_widget(parent));
                p->add(w->fltk_widget());
            } else {
                // Only permissable for Fl_Window and derived types
                if (w->fltk_widget()->type() < FL_WINDOW) {
                    throw "NEW, non-Window widget (" + param0 + ": " + wname + ") without parent";
                }
            }

            //TODO: Add the Widget as "user data" to the widget.
            // was WidgetData::add_widget(name, w, h);

            // Handle methods
            handle_methods(w, parammap);

        } else throw "NEW, widget (" + param0 + ") with no NAME";

    } else if (parammap.get_string("WIDGET", param0)) {
        if (auto w = Widget::get_widget_data(param0)) {
            // Handle methods
            handle_methods(w, parammap);
        } else {
            throw "Unknown widget: " + param0;
        }
    } else if (parammap.get_string("FUNCTION", param0)) {
        auto f = function_map.at(param0);
        f(parammap);
    } else {
        throw "Invalid command: " + dump_map_items(parammap, -1);
    }
}
