#include "widget_methods.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Flex.H>
#include <FL/fl_draw.H>
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 255
#include "_lib/magic_enum/magic_enum.hpp"
#include <fmt/format.h>
using namespace std;

Fl_Color get_colour(
    string& colour)
{
    if (colour.length() == 6) {
        return static_cast<Fl_Color>(stoi(colour, nullptr, 16)) * 0x100;
    }
    throw fmt::format("Invalid colour: '{}'", colour);
}

Fl_Boxtype get_boxtype(
    string& boxtype)
{
    return magic_enum::enum_cast<Fl_Boxtype>(boxtype).value();
}

int int_param(
    minion::MinionList m, int i)
{
    return stoi(get<string>(m.at(i)));
}

Fl_Color colour_param(
    minion::MinionList m, int i)
{
    return get_colour(get<string>(m.at(i)));
}

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
