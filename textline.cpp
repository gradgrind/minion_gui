#include "textline.h"
#include "callback.h"
#include "widgetdata.h"
#include <iostream>
using namespace std;

TextLine::TextLine(int height) : Fl_Input(0, 0, 0, height)
{
    bg_normal = ENTRY_BG;
    bg_pending = PENDING_BG;
    when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    callback(
        [](Fl_Widget* w, void* ud) {
            auto ww = static_cast<TextLine*>(w);
            // Don't leave all text selected when return is pressed
            ww->insert_position( ww->insert_position());
            string v = ww->value();
            if (ww->set(v)) { 
                string dw{WidgetData::get_widget_name(w)};
                // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
                
                Callback1(dw, v);

                // This would remove keyboard focus from the widget
                //Fl::focus(w->parent());
            }
        });
}

int TextLine::handle(int event)
{ 
    auto result = Fl_Input::handle(event);
    if (strcmp(value(), text.c_str()) == 0) {
        if (modified) {
            modified = false;
            // Reset colour
            color(bg_normal);
            redraw();
        }
    } else {
        if (!modified) {
            modified = true;
            // Set modified/pending colour
            color(bg_pending);
            redraw();
        }
    }
    return result;
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

bool TextLine::set(std::string_view newtext)
{
    //modified = false;
    string v = value();
    bool res{false};
    if (newtext != text) {
        text = newtext;
        res = true;
    }
    if (v != text) value(text.c_str());
    return res;
}

Fl_Widget *NEW_TextLine(
    minion::MinionMap param)
{
    int h = 0;
    param.get_int("HEIGHT", h);
    return new TextLine(h);
}
