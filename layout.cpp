#include "layout.h"
#include "widgetdata.h"
#include "widget_methods.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <fmt/format.h>
#include <iostream>
using namespace std;
using mlist = minion::MinionList;
using mmap = minion::MinionMap;

// *** "Group" widgets ***

void group_method(
    Fl_Widget *w, string_view c, mlist m)
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
    Fl_Widget *w, string_view c, mlist m)
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
    Fl_Widget *w, string_view c, mlist m)
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
    mmap param)
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
    mmap param)
{
    auto widg = new Fl_Flex(Fl_Flex::COLUMN);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

Fl_Widget *NEW_Hlayout(
    mmap param)
{
    auto widg = new Fl_Flex(Fl_Flex::ROW);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}

Fl_Widget *NEW_Grid(
    mmap param)
{
    auto widg = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    return widg;
}
