//TODO-- deprecated

//#include "layout.h"
//#include "widgetdata.h"
//#include "widget_methods.h"
#include <FL/Fl_Flex.H>
#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace minion;

//TODO: ???
void do_callback(
    Fl_Widget* w, void* x)
{
    auto wd{static_cast<WidgetData*>(w->user_data())};
    cout << "Callback: " << wd->widget_name() << endl;
}

void widget_method(
    Fl_Widget *w, string_view c, MList* m)
{
    int ww, wh;
    if (c == "SIZE") {
        ww = int_param(m, 1); // width
        wh = int_param(m, 2); // height
        w->size(ww, wh);
    } else if (c == "HEIGHT") {
        wh = int_param(m, 1); // height
        w->size(w->w(), wh);
    } else if (c == "WIDTH") {
        ww = int_param(m, 1); // width
        w->size(ww, w->h());
    } else if (c == "COLOUR") {
        auto clr = get_colour(get<string>(m.at(1)));
        w->color(clr);
    } else if (c == "BOXTYPE") {
        auto bxt = get_boxtype(get<string>(m.at(1)));
        w->box(bxt);
    } else if (c == "LABEL") {
        auto lbl = get<string>(m.at(1));
        w->copy_label(lbl.c_str());
    } else if (c == "CALLBACK") {
        auto cb = get<string>(m.at(1));
        w->callback(do_callback);
    } else if (c == "SHOW") {
        w->show();
    } else if (c == "FIXED") {
        auto parent = dynamic_cast<Fl_Flex *>(w->parent());
        if (parent) {
            int sz = int_param(m, 1);
            parent->fixed(w, sz);
        } else {
            throw fmt::format("Widget ({}) method FIXED: parent not VLayout/Hlayout",
                              WidgetData::get_widget_name(w));
        }
    } else if (c == "clear_visible_focus") {
        w->clear_visible_focus();
    } else if (c == "measure_label") {
        int wl, hl;
        w->measure_label(wl, hl);
        //TODO ...
        cout << "Measure " << WidgetData::get_widget_name(w) << " label: " << wl << ", " << hl
             << endl;
    } else {
        throw fmt::format("Unknown widget method: {}", c);
    }
}

//static member variable
std::unordered_map<std::string_view, Fl_Widget *> WidgetData::widget_map;

// static
MList* WidgetData::list_widgets()
{
    MList* keys;
    for (const auto &kv : widget_map) {
        keys.emplace_back(string{kv.first});
    }
    return keys;
}

// static
Fl_Widget *WidgetData::get_widget(
    string_view name)
{
    try {
        return widget_map.at(name);
    } catch (const std::out_of_range &e) {
        throw fmt::format("Unknown widget: {} ({})", name, e.what());
    }
}

// static
void WidgetData::check_new_widget_name(string_view name)
{
    if (name.empty()) {
        throw "Unnamed widget ...";
    } else if (widget_map.contains(name)) {
        throw string{"Widget name already exists: "} + string{name};
    }
}

// static
void WidgetData::add_widget(
    string_view name, Fl_Widget *w, method_handler h)
{
    //TODO--??
    check_new_widget_name(name);
    
    auto wd = new WidgetData(name, h);
    widget_map.emplace(wd->w_name, w);
    w->user_data(wd, true); // auto-free = true
}

// static
string_view WidgetData::get_widget_name(
    Fl_Widget *w)
{
    auto wd{static_cast<WidgetData *>(w->user_data())};
    return wd->w_name;
}

WidgetData::WidgetData(
    string_view wname, method_handler h)
    : Fl_Callback_User_Data()
    , w_name{wname}
    , handle_method{h}
{
    //TODO: widget type?
}

// The destructor is called when the widget to which this WidgetData is
// attached is deleted – if the widget's user-data was set with auto-free
// true. The widget reference must be removed from widget_map and the
// user-data associated with the WidgetData might need deleting.
WidgetData::~WidgetData()
{
    if (widget_map.erase(w_name) == 0) {
        cerr << fmt::format("Can't remove widget '{}', it doesn't exist", w_name) << endl;
    }
    if (auto_delete_user_data && user_data)
        delete static_cast<Fl_Callback_User_Data *>(user_data);
}

string_view WidgetData::widget_name()
{
    return w_name;
}

/*
int WidgetData::widget_type()
{
    return wtype;
}

string_view WidgetData::widget_type_name()
{
    auto i = widget_type();
    return widget_type_names[i];
}
*/
