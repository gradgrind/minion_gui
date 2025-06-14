#ifndef LAYOUT_H
#define LAYOUT_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Widget.H>
#include <string_view>

class W_Group : public Widget
{
protected:
    virtual void handle_child_modified(Widget* wc) = 0;

public:
    static void child_size_modified(
        Widget* wc)
    {
        /*printf("§ %s %d %d\n",
               wc->widget_name()->c_str(),
               wc->fltk_widget()->w(),
               wc->fltk_widget()->h());
        fflush(stdout);*/
        if (auto p = wc->fltk_widget()->parent()) {
            auto wp{static_cast<W_Group*>(p->user_data())};
            wp->handle_child_modified(wc);
        }
    }
};

class W_Window : public Widget
{
protected:
    Fl_Flex* container;
    static void make_window(int ww, int wh, W_Window* widget, minion::MMap* props);

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Window* make(minion::MMap* props);
};

class W_Grid : public W_Group
{
    void handle_child_modified(Widget* wc) override;

protected:
    int nrows = 0;
    int ncols = 0;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Grid* make(minion::MMap* props);
};

class W_Layout : public W_Group
{
    static W_Layout* new_hvgrid(minion::MMap* parammap, bool horizontal);

    bool horizontal = false;
    std::vector<Widget*> children;
    int padding = 0;
    void set_transverse_size();

    void handle_child_modified(Widget* wc) override;

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

class W_Stack : public W_Group
{
    Fl_Widget* current = nullptr;

    void handle_child_modified(Widget* wc) override;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Stack* make(minion::MMap* props);
};

class W_EditForm : public W_Group
{
    struct form_element
    {
        Widget* element;
        Fl_Widget* label;
        int span = 0; // 0: right column, otherwise both columns, 2: "grow"
    };

    int v_label_gap = 5;
    std::vector<form_element> children;

    void handle_child_modified(Widget* wc) override;

public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_EditForm* make(minion::MMap* props);
};

#endif // LAYOUT_H
