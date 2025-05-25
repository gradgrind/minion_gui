#include "widget.h"
#include "functions.h"
#include "widget_methods.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <FL/Fl_Group.H>
using namespace std;
using namespace minion;

//TODO: get missing methods from widgetdata.cpp?

// static
void Widget::check_new_widget_name(string_view name)
{
    if (name.empty()) {
        throw "A new widget must have a name ...";
    } else if (widget_map.contains(name)) {
        throw string{"Widget name already exists: "}.append(name);
    }
}

//TODO??? Is this a duplicate? Is it needed?
void handle_methods(
    Widget* w, MMap* mmap)
{
    auto dolist = mmap.get("DO");
    if (holds_alternative<MList*>(dolist)) {
        MList* do_list = get<MList*>(dolist);
        for (const auto& cmd : do_list) {
            if (holds_alternative<MList*>(cmd)) {
                MList* mlist = get<MList*>(cmd);
                if (mlist.empty()) goto fail;
                string c;
                try {
                    c = get<string>(mlist.at(0));
                } catch (std::bad_variant_access) {
                    goto fail;
                }
                w->handle_method(c, mlist);
            }
            else goto fail;
        }
        return;
    } else if (dolist.index() == 0) return;
fail:    
    string s;
    minion::dump(s, dolist, 0);
    throw "Invalid DO list: " + s;
}

void process_command(
    MMap* parammap)
{
    //TODO: widget type?
    string param0;
    if (parammap.get_string("NEW", param0)) {
        function<Widget*(MMap*&)> wmake;
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


void Widget::handle_method(std::string_view method, minion::MList* paramlist)
{
    auto w = fltk_widget();
    int ww, wh;
    if (method == "SIZE") {
        ww = int_param(paramlist, 1); // width
        wh = int_param(paramlist, 2); // height
        w->size(ww, wh);
    } else if (method == "HEIGHT") {
        wh = int_param(paramlist, 1); // height
        w->size(w->w(), wh);
    } else if (method == "WIDTH") {
        ww = int_param(paramlist, 1); // width
        w->size(ww, w->h());
    } else if (method == "COLOUR") {
        auto clr = get_colour(get<string>(paramlist.at(1)));
        w->color(clr);
    } else if (method == "BOXTYPE") {
        auto bxt = get_boxtype(get<string>(paramlist.at(1)));
        w->box(bxt);
    } else if (method == "LABEL") {
        //TODO: Do I really want this for all widgets?
        auto lbl = get<string>(paramlist.at(1));
        w->copy_label(lbl.c_str());
    //} else if (method == "CALLBACK") {
    //    auto cb = get<string>(paramlist.at(1));
    //    w->callback(do_callback);
    } else if (method == "SHOW") {
        w->show();
    } else if (method == "clear_visible_focus") {
        w->clear_visible_focus();
    } else if (method == "measure_label") {
        int wl, hl;
        w->measure_label(wl, hl);
        //TODO ...
        cout << "Measure " << widget_name() << " label: " << wl << ", " << hl
             << endl;
    } else {
        throw "Unknown widget method: " + string{method};
    }
}
