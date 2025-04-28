#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include <FL/Fl_Widget.H>
#include <string_view>

//TODO ...
void tmp_run(minion::MinionMap data);

Fl_Widget *NEW_Window(minion::MinionMap param);
Fl_Widget *NEW_Vlayout(minion::MinionMap param);
Fl_Widget *NEW_Hlayout(minion::MinionMap param);
Fl_Widget *NEW_Grid(minion::MinionMap param);
Fl_Widget *NEW_Hgrid(minion::MinionMap param);
Fl_Widget *NEW_Vgrid(minion::MinionMap param);

void grid_method(Fl_Widget *w, std::string_view c, minion::MinionList m);
void flex_method(Fl_Widget *w, std::string_view c, minion::MinionList m);
void group_method(Fl_Widget *w, std::string_view c, minion::MinionList m);

#endif // LAYOUT_H
