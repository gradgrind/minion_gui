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
using namespace minion;

void choice_method(
    Fl_Widget *w, string_view c, MinionList m)
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
    Fl_Widget *w, string_view c, MinionList m)
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
    Fl_Widget *w, string_view c, MinionList m)
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
    MinionMap param)
{
    return new Fl_Box(0, 0, 0, 0);
}

Fl_Widget *NEW_Label(
    MinionMap param)
{
    string label{};
    if (!param.get_string("LABEL", label)) {
        param.get_string("NAME", label);
    }
    string align{};
    param.get_string("ALIGN", align);

    auto w = new Fl_Box(0, 0, 0, 0);
    w->copy_label(label.c_str());
    int lw{0}, lh;
    w->measure_label(lw, lh);
    //w->horizontal_label_margin(5);
    w->size(lw + 20, WidgetData::line_height);
    if (align == "LEFT") w->align(FL_ALIGN_INSIDE|FL_ALIGN_LEFT);
    else if (align == "RIGHT") w->align(FL_ALIGN_INSIDE|FL_ALIGN_RIGHT);
    return w;
}

Fl_Widget *NEW_Choice(
    MMap* param)
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
    MinionMap param)
{
    auto w = new Fl_Output(0, 0, 0, WidgetData::line_height);
    w->color(WidgetData::entry_bg);
    return w;
}

Fl_Widget *NEW_PushButton(
    MinionMap param)
{
    string label{};
    if (!param.get_string("LABEL", label)) {
        param.get_string("NAME", label);
    }
    auto w = new Fl_Button(0, 0, 0, 0);
    w->copy_label(label.c_str());
    int lw{0}, lh;
    w->measure_label(lw, lh);
    //TODO: margins settable?
    w->size(lw + 20, WidgetData::line_height);
    //TODO: "selection" colour
    w->color(WidgetData::entry_bg, 0xe0e0ff00);
    w->callback(
        [](Fl_Widget* w, void* ud) {
            string dw{WidgetData::get_widget_name(w)};
            // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
            auto ww = static_cast<Fl_Button*>(w);
            auto res = Callback1(dw, "");
            cout << "CALLBACK RETURNED: " << dump_map_items(res, -1) << endl;
            
            //TODO--
            auto wo = static_cast<Fl_Input *>(WidgetData::get_widget("Output_1"));
            string v{"DUMMY"};
            res.get_string("GoCallbackResult", v);
            
            cout << "CALLBACK VALUE: " << v << endl;
            wo->value(v.c_str());
        });
    return w;
}

Fl_Widget *NEW_Checkbox(
    MinionMap param)
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

Fl_Widget *NEW_List(
    MinionMap param)
{
    auto w = new Fl_Select_Browser(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    w->color(WidgetData::entry_bg);
    w->callback(
        [](Fl_Widget* w, void* ud) {
            string dw{WidgetData::get_widget_name(w)};
            // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
            auto ww = static_cast<Fl_Select_Browser*>(w);
            auto i = ww->value();
            // Callback only for actual items (1-based indexing)
            if (i > 0) {
                string itemtext{ww->text(i)};
                auto res = Callback2(
                    dw, 
                    to_string(i - 1), 
                    itemtext);
                cout << "CALLBACK RETURNED: " << dump_map_items(res, -1) << endl;
            }
        });
    return w;
}
