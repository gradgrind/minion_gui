#include "widgets.h"
#include "callback.h"
#include "widgetdata.h"
#include "widget_methods.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <iostream>
using namespace std;
using mmap = minion::MinionMap;
using mlist = minion::MinionList;

void choice_method(
    Fl_Widget *w, string_view c, mlist m)
{
    if (c == "ADD") {
        for (int i = 1; i < m.size(); ++i) {
            //TODO: Do I need to store the string somewhere, or is that
            // handled by the widget?
            static_cast<Fl_Choice *>(w)->add(get<string>(m.at(i)).c_str());
        }
    } else if (c == "LABEL") {
        left_label(w, m);
    } else {
        widget_method(w, c, m);
    }
}

void input_method(
    Fl_Widget *w, string_view c, mlist m)
{
    if (c == "VALUE") {
        //TODO: Do I need to store the string somewhere, or is that
        // handled by the widget?
        static_cast<Fl_Input *>(w)->value(get<string>(m.at(1)).c_str());
    } else if (c == "LABEL") {
        left_label(w, m);
    } else {
        widget_method(w, c, m);
    }
}

void list_method(
    Fl_Widget *w, string_view c, mlist m)
{
    if (c == "SET") {
        auto e1 = static_cast<Fl_Select_Browser*>(w);
        e1->clear();
        for (int i = 1; i < m.size(); ++i) {
            //TODO? add can have a second argument (void *), which can
            // refer to data ...
            e1->add(get<string>(m.at(i)).c_str());
        }
    } else {
        widget_method(w, c, m);
    }
}

// *** Non-layout widgets ***

Fl_Widget *NEW_Box(
    mmap param)
{
    return new Fl_Box(0, 0, 0, 0);
}

Fl_Widget *NEW_Choice(
    mmap param)
{
    auto w = new Fl_Choice(0, 0, 0, WidgetData::line_height);
    w->callback(
        [](Fl_Widget* w, void* ud) {
            string dw{WidgetData::get_widget_name(w)};
            // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
            auto ww = static_cast<Fl_Choice*>(w);
            auto res = Callback2(dw, to_string(ww->value()), ww->text());
            cout << "CALLBACK RETURNED: " << dump_map_items(res, -1) << endl;
        });
    return w;
}

Fl_Widget *NEW_Output(
    mmap param)
{
    return new Fl_Output(0, 0, 0, WidgetData::line_height);
}

Fl_Widget *NEW_Checkbox(
    mmap param)
{
    auto w = new Fl_Round_Button(0, 0, 0, WidgetData::line_height);
    w->callback(
        [](Fl_Widget* w, void* ud) {
            string dw{WidgetData::get_widget_name(w)};
            // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
            auto ww = static_cast<Fl_Round_Button*>(w);
            string val{};
            if (ww->value() != 0) val = "1";
            auto res = Callback1(dw, val);
            cout << "CALLBACK RETURNED: " << dump_map_items(res, -1) << endl;
        });
    return w;
}
