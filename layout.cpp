#include "layout.h"
#include "widgetdata.h"
#include "widget_methods.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <fmt/format.h>
#include <iostream>
using namespace std;
using namespace minion;

// *** "Group" widgets ***

void group_method(
    Fl_Widget *w, string_view c, MinionList m)
{
    if (c == "RESIZABLE") {
        auto rsw = WidgetData::get_widget(get<string>(m.at(1)));
        w->as_group()->resizable(rsw);
    } else if (c == "fit_to_parent") {
        if (auto parent = w->parent()) {
            w->resize(0, 0, parent->w(), parent->h());
            parent->resizable(w);
        } else {
            throw fmt::format("Widget ({}) method fit_to_parent: no parent",
                              WidgetData::get_widget_name(w));
        }
    } else {
        widget_method(w, c, m);
    }
}

void flex_method(
    Fl_Widget *w, string_view c, MinionList m)
{
    if (c == "MARGIN") {
        int sz = int_param(m, 1);
        static_cast<Fl_Flex *>(w)->margin(sz);
    } else if (c == "GAP") {
        int sz = int_param(m, 1);
        static_cast<Fl_Flex *>(w)->gap(sz);
    } else {
        group_method(w, c, m);
    }
}

void grid_method(
    Fl_Widget *w, string_view c, MinionList m)
{
    if (c == "GAP") {
        int szr = int_param(m, 1);
        int szc = szr;
        if (m.size() > 2) {
            szc = int_param(m, 2);
        }
        static_cast<Fl_Grid *>(w)->gap(szr, szc);
    } else if (c == "MARGIN") {
        int sz = int_param(m, 1);
        static_cast<Fl_Grid *>(w)->margin(sz, sz, sz, sz);
    } else {
        group_method(w, c, m);
    }
}

void callback_no_esc_closes(
    Fl_Widget *w, void *x)
{
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape
    //TODO: message to backend?
    cout << "Closing " << WidgetData::get_widget_name(w) << endl;
    //TODO--
    exit(0);
}

Fl_Widget *NEW_Window(
    MinionMap param)
{
    int w = 800;
    int h = 600;
    param.get_int("WIDTH", w);
    param.get_int("HEIGHT", h);
    auto widg = new Fl_Double_Window(w, h);
    int esc_closes{0};
    param.get_int("ESC_CLOSES", esc_closes);
    if (!esc_closes)
        widg->callback(callback_no_esc_closes);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

Fl_Widget *NEW_Vlayout(
    MinionMap param)
{
    auto widg = new Fl_Flex(Fl_Flex::COLUMN);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

Fl_Widget *NEW_Hlayout(
    MinionMap param)
{
    auto widg = new Fl_Flex(Fl_Flex::ROW);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

Fl_Widget *NEW_Grid(
    MinionMap param)
{
    auto widg = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

//TODO: A problem with this approach is that some widgets need more info
// to define them that must be passed as parameters. Indeed this one does!
// Perhaps I should have unified widget descriptions – as maps ...
// To avoid nesting I could define them before the grid with no parent?
Fl_Widget *NEW_VGrid(
    MinionMap param)
{
    auto widg = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    auto items = param.get("ITEMS");
    //TODO: The items are lists, the first element is the widget name,
    // subsequent elements are for "filling" and fixing.
    if (holds_alternative<MinionList>(items)) {
        auto item_list = get<MinionList>(items);
        int n_items = item_list.size();
        widg->layout(n_items, 1);
        for (int i = 0; i < n_items; ++i) {
            auto item = get<MinionList>(item_list.at(i));
            int n_params = item.size();
            if (n_params != 0) {
                auto wname = get<string>(item.at(0));
                // now options ???
                for (int i = 1; i < item.size(); ++i) {
                    auto p =  get<string>(item.at(i));
                //TODO
                
                }
                

                return widg;
            }
        }
    }
    string s;
    minion::dump(s, items, 0);
    throw string{"Invalid ITEMS list: "} + s;    
}
