#include "textline.h"
#include "widgetdata.h"
#include <iostream>
using namespace std;

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

Fl_Widget *NEW_TextLine(
    minion::MinionMap param)
{
    int h = 0;
    param.get_int("HEIGHT", h);
    return new TextLine(h);
}
