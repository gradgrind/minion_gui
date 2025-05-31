#include "widget.h"
#include "callback.h"
#include "support_functions.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <FL/Fl_Group.H>
using namespace std;
using namespace minion;

// static member
std::unordered_map<std::string_view, Widget*> Widget::widget_map;

// static
void Widget::new_widget(
    string_view wtype, MMap* m)
{
    string name;
    // Check new widget name
    if (!m->get_string("NAME", name)) {
        MValue m0{*m};
        throw string{"Bad NEW command: "} + dump_value(m0);
    }
    // Check name unique
    if (name.empty()) {
        throw "A new widget must have a name ...";
    } else if (widget_map.contains(name)) {
        throw string{"Widget name already exists: "}.append(name);
    }
    new_function f;
    try {
        f = new_function_map.at(string{wtype});
    } catch (std::out_of_range& e) {
        throw string{"Unknown widget type: "}.append(wtype);
    }
    // Create widget
    Widget* w = f(m);
    w->w_name = std::move(name);
    widget_map.emplace(w->w_name, w);
    w->fltk_widget()->user_data(w, true); // auto-free = true
    {
        auto props = m->get("PROPERTIES");
        if (props.type() == minion::T_Map)
            w->properties = *props.m_map();
    }
    // Add to parent, if specified
    string parent;
    if (m->get_string("PARENT", parent) && !parent.empty()) {
        auto p = Widget::get_fltk_widget(parent)->as_group();
        if (!p)
            throw string{"Invalid parent widget: "} + parent;
        p->add(w->fltk_widget());
    }
     // Handle method calls supplied with the widget creation
    w->handle_methods(m);
}

// static
Widget* Widget::get_widget(
    string_view name)
{
    try {
        return widget_map.at(name);
    } catch (const out_of_range& e) {
        throw string{"Unknown widget: "}.append(name);
    }
}

// The destructor will be called when the associated fltk_widget is destroyed,
// thanks to that widget's user_data setting.
Widget::~Widget() {
    // Delete any associated "user_data"
    if (auto_delete_user_data && user_data) {
        delete static_cast<Fl_Callback_User_Data*>(user_data);
    }
    // Remove from widget map
    widget_map.erase(w_name);
}

void Widget::handle_methods(
    MMap* m)
{
    auto dolist0 = m->get("DO");
    if (!dolist0.is_null()) {
        if (auto dolist = dolist0.m_list()) {
            auto len = (*dolist)->size();
            for (size_t i = 0; i < len; ++i) {
                auto n = (*dolist)->get(i);
                auto mlist = n.m_list();
                string c;
                if ((*mlist)->get_string(0, c)) {
                    handle_method(c, mlist->get());
                } else {
                    throw string{"Invalid DO method on widget "} + w_name + ": " + dump_value(n);
                }
            }
            return;
        }
        MValue m0{*m};
        throw string{"Invalid DO list on widget "} + w_name + ": " + dump_value(m0);
    }
}

void Widget::handle_method(std::string_view method, minion::MList* paramlist)
{
    //auto w = fl_widget;
    int ww, wh;
    if (method == "SIZE") {
        paramlist->get_int(1, ww); // width
        paramlist->get_int(2, wh); // height
        fl_widget->size(ww, wh);
    } else if (method == "HEIGHT") {
        paramlist->get_int(1, wh); // height
        fl_widget->size(fl_widget->w(), wh);
    } else if (method == "WIDTH") {
        paramlist->get_int(1, ww); // width
        fl_widget->size(ww, fl_widget->h());
    } else if (method == "COLOUR") {
        string clr;
        if (paramlist->get_string(1, clr)) {
            auto c = get_colour(clr);
            fl_widget->color(c);
        }
    } else if (method == "BOXTYPE") {
        string btype;
        if (paramlist->get_string(1, btype)) {
            auto bxt = get_boxtype(btype);
            fl_widget->box(bxt);
        } else
            throw "BOXTYPE value missing for widget " + w_name;
    } else if (method == "TEXT") {
        //TODO: Do I really want this for all widgets?
        string lbl;
        if (paramlist->get_string(1, lbl)) {
            fl_widget->copy_label(lbl.c_str());
        } else
            throw "TEXT value missing for widget " + w_name;
    //} else if (method == "CALLBACK") {
    //    auto cb = get<string>(paramlist.at(1));
    //    fl_widget->callback(do_callback);
    } else if (method == "SHOW") {
        fl_widget->show();
    } else if (method == "clear_visible_focus") {
        fl_widget->clear_visible_focus();
    } else if (method == "MeasureLabel") {
        int wl, hl;
        fl_widget->measure_label(wl, hl);
        //TODO ...
        cout << "Measure " << widget_name() << " label: " << wl << ", " << hl
             << endl;
    } else {
        throw string{"Unknown method on widget " + w_name + ": "}.append(method);
    }
}
