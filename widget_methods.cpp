#include "widget_methods.h"
//#include <FL/Fl_Box.H>
//#include <FL/Fl_Flex.H>
//#include <FL/fl_draw.H>
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 255
#include "_lib/magic_enum/magic_enum.hpp"
using namespace std;


// Read a string with 6 characters as a hex integer, returning the value
// multiplied by 0x100 to make an FLTK colour value.
Fl_Color get_colour(
    const string& colour)
{
    if (colour.size() == 6) {
        unsigned int i;
        try {
            size_t index;
            i = stoul(colour, &index, 16);
            if (index == colour.size()) return i * 0x100;
        } catch (...) {}
    }
    string msg{"Invalid FLTK colour: '" + colour + "'"};
    throw msg; 
}

Fl_Boxtype get_boxtype(
    string& boxtype)
{
    return magic_enum::enum_cast<Fl_Boxtype>(boxtype).value();
}

//TODO--
void left_label(
    Fl_Widget *w, minion::MinionList m)
{
    // In an Hlayout an empty box will be inserted as the the widget
    // itself allocates no space for the label.
    auto lbl = get<string>(m.at(1));
    w->copy_label(lbl.c_str());
    auto pw = dynamic_cast<Fl_Flex *>(w->parent());
    if (pw && pw->horizontal()) {
        auto padbox = new Fl_Box(0, 0, 0, 0);
        pw->insert(*padbox, w);
        int wl{0}, hl{0};
        w->measure_label(wl, hl);
        pw->fixed(padbox, wl + int_param(m, 2));
    }
}
