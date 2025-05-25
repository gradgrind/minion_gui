#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Widget.H>
#include <string_view>

class W_Group : public Widget
{
public:
    W_Group(minion::MMap* parammap);
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    //static W_Group* make(minion::MMap* parammap);
};

class W_Window : public W_Group
{
public:
    W_Window(minion::MMap* parammap);
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_Window* make(minion::MMap* parammap);
};

class W_Grid : public W_Group
{
public:
    W_Grid(minion::MMap* parammap);
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_Grid* make(minion::MMap* parammap);
};

class W_Row : public W_Grid
{
public:
    W_Row(minion::MMap* parammap);
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_Row* make(minion::MMap* parammap);
};

class W_Column : public W_Grid
{
public:
    W_Column(minion::MMap* parammap);
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_Window* make(minion::MMap* parammap);
};

//TODO ...
void tmp_run(minion::MMap data);

Fl_Widget *NEW_Window(minion::MMap* param);
Fl_Widget *NEW_Grid(minion::MMap* param);
Fl_Widget *NEW_Row(minion::MMap* param);
Fl_Widget *NEW_Column(minion::MMap* param);

void grid_method(Fl_Widget *w, std::string_view c, minion::MList m);
void group_method(Fl_Widget *w, std::string_view c, minion::MList m);

#endif // LAYOUT_H
