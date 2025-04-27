#include "textline.h"
#include "callback.h"
#include "widgetdata.h"
using namespace std;

TextLine::TextLine() : Fl_Input(0, 0, 0, WidgetData::line_height)
{
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
            color(WidgetData::entry_bg);
            redraw();
        }
    } else {
        if (!modified) {
            modified = true;
            // Set modified/pending colour
            color(WidgetData::pending_bg);
            redraw();
        }
    }
    return result;
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
    return new TextLine();
}
