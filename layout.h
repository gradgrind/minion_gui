#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Widget.H>
#include <string_view>

class W_Group : public Widget
{
public:
    W_Group(minion::MinionMap parammap);
    virtual void handle_method(std::string_view method, minion::MinionList &paramlist);
    //static W_Group* make(minion::MinionMap &parammap);
};

class W_Window : public W_Group
{
public:
    W_Window(minion::MinionMap parammap);
    virtual void handle_method(std::string_view method, minion::MinionList &paramlist);
    static W_Window* make(minion::MinionMap &parammap);
};

class W_Grid : public W_Group
{
public:
    W_Grid(minion::MinionMap parammap);
    virtual void handle_method(std::string_view method, minion::MinionList &paramlist);
    static W_Grid* make(minion::MinionMap &parammap);
};

class W_Row : public W_Grid
{
public:
    W_Row(minion::MinionMap parammap);
    virtual void handle_method(std::string_view method, minion::MinionList &paramlist);
    static W_Row* make(minion::MinionMap &parammap);
};

class W_Column : public W_Grid
{
public:
    W_Column(minion::MinionMap parammap);
    virtual void handle_method(std::string_view method, minion::MinionList &paramlist);
    static W_Window* make(minion::MinionMap &parammap);
};

//TODO ...
void tmp_run(minion::MinionMap data);

Fl_Widget *NEW_Window(minion::MinionMap param);
Fl_Widget *NEW_Grid(minion::MinionMap param);
Fl_Widget *NEW_Row(minion::MinionMap param);
Fl_Widget *NEW_Column(minion::MinionMap param);

void grid_method(Fl_Widget *w, std::string_view c, minion::MinionList m);
void group_method(Fl_Widget *w, std::string_view c, minion::MinionList m);

#endif // LAYOUT_H
