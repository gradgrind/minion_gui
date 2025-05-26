#ifndef WIDGET_METHODS_H
#define WIDGET_METHODS_H

#include "minion.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>
#include <string>

Fl_Color get_colour(std::string &colour);
Fl_Boxtype get_boxtype(std::string &boxtype);

//TODO--? Now a method (get_int) of MList? int_param(minion::MList* m, int i);
Fl_Color colour_param(minion::MList* m, int i);

//TODO--?
//void left_label(Fl_Widget *w, minion::MList* m);

#endif // WIDGET_METHODS_H
