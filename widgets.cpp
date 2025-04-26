#include "layout.h"
#include "widgets.h"
#include "minion.h"
#include "widget_methods.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Output.H>
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

TextLine::TextLine(int height) : Fl_Input(0, 0, 0, height)
{
    bg_normal = ENTRY_BG;
    bg_pending = PENDING_BG;
}

int TextLine::handle(int event)
{
    //TODO ... ???
    // if focus in: change colour, if focus out: revert colour
    if (event == FL_FOCUS) {
        cout << "FOCUS" << endl;
        color(bg_pending);
        redraw();
    } else if (event == FL_UNFOCUS) {
        cout << "UNFOCUS" << endl;
        color(bg_normal);
        redraw();
    }
    return Fl_Input::handle(event);
}


Fl_Widget *NEW_Box(
    mmap param)
{
    return new Fl_Box(0, 0, 0, 0);
}

Fl_Widget *NEW_Choice(
    mmap param)
{
    return new Fl_Choice(0, 0, 0, 0);
}

Fl_Widget *NEW_Output(
    mmap param)
{
    return new Fl_Output(0, 0, 0, 0);
}

Fl_Widget *NEW_TextLine(
    mmap param)
{
    int h = 0;
    param.get_int("HEIGHT", h);
    return new TextLine(h);
}
