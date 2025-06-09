#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Widget.H>
#include <string_view>

class W_Group : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    //static W_Group* make(minion::MMap* props);
};

class W_Window : public W_Group
{
public:
    // Inherit handle_method from W_Group
    static W_Window* make(minion::MMap* props);
};

class W_Grid : public W_Group
{
protected:
    static W_Grid* new_hvgrid(minion::MMap* parammap, bool horizontal);
    
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Grid* make(minion::MMap* props);
    static W_Grid* make_hlayout(
        minion::MMap* parammap)
    {
        return new_hvgrid(parammap, true);
    }
    static W_Grid* make_vlayout(
        minion::MMap* parammap)
    {
        return new_hvgrid(parammap, false);
    }
};

class W_Stack : public W_Group
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Stack* make(minion::MMap* props);
};

#endif // LAYOUT_H
