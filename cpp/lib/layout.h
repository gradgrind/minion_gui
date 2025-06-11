#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Widget.H>
#include <string_view>

class W_Window : public Widget
{
protected:
    Fl_Flex* container;
    static void make_window(int ww, int wh, W_Window* widget, minion::MMap* props);

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Window* make(minion::MMap* props);
};

class W_Grid : public Widget
{
protected:
    struct grid_element
    {
        int row;
        int col;
        int rspan = 1;
        int cspan = 1;
    };

    int nrows = 0;
    int ncols = 0;
    std::map<Widget*, grid_element> children;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Grid* make(minion::MMap* props);
};

class W_Layout : public Widget
{
    static W_Layout* new_hvgrid(minion::MMap* parammap, bool horizontal);

protected:
    bool horizontal;
    std::vector<Widget*> children;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Layout* make_hlayout(
        minion::MMap* props)
    {
        return new_hvgrid(props, true);
    }
    static W_Layout* make_vlayout(
        minion::MMap* props)
    {
        return new_hvgrid(props, false);
    }
};

class W_Stack : public Widget
{
    Fl_Widget* current = nullptr;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Stack* make(minion::MMap* props);
};

#endif // LAYOUT_H
