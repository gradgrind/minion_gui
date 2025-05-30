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
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    //static W_Group* make(minion::MMap* parammap);
};

class W_Window : public W_Group
{
public:
    W_Window(minion::MMap* parammap);
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Window* make(minion::MMap* parammap);
};

class W_Grid : public W_Group
{
protected:
    static W_Grid* new_hvgrid(minion::MMap* parammap, bool horizontal);
    
public:
    W_Grid(minion::MMap* parammap);
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Grid* make(minion::MMap* parammap);
    static W_Grid* make_hgrid(minion::MMap* parammap)
    {
        return new_hvgrid(parammap, true);
    }
    static W_Grid* make_vgrid(minion::MMap* parammap)
    {
        return new_hvgrid(parammap, false);
    }
};

//TODO ...
void tmp_run(minion::MMap data);

#endif // LAYOUT_H
