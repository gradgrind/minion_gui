#include "layout.h"
#include "minion.h"
#include "widget.h"
#include "widget_methods.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <fmt/format.h>
#include <iostream>
#include <map>
using namespace std;
using namespace minion;


const map<string, Fl_Grid_Align>GRID_ALIGN{
    /** Align the widget in the middle of the cell (default). */
    {"CENTRE", FL_GRID_CENTER},
    /** Align the widget at the top of the cell. */
    {"TOP", FL_GRID_TOP},
    /** Align the widget at the bottom of the cell. */
    {"BOTTOM", FL_GRID_BOTTOM},
    /** Align the widget at the left side of the cell. */
    {"LEFT", FL_GRID_LEFT},
    /** Align the widget at the right side of the cell. */
    {"RIGHT", FL_GRID_RIGHT},
    /** Stretch the widget horizontally to fill the cell. */
    {"HORIZONTAL", FL_GRID_HORIZONTAL},
    /** Stretch the widget vertically to fill the cell. */
    {"VERTICAL", FL_GRID_VERTICAL},
    /** Stretch the widget in both directions to fill the cell. */
    {"FILL", FL_GRID_FILL},
    /** Stretch the widget proportionally. */
    {"PROPORTIONAL", FL_GRID_PROPORTIONAL},
    {"TOP_LEFT", FL_GRID_TOP_LEFT},
    {"TOP_RIGHT", FL_GRID_TOP_RIGHT},
    {"BOTTOM_LEFT", FL_GRID_BOTTOM_LEFT},
    {"BOTTOM_RIGHT", FL_GRID_BOTTOM_RIGHT}
};

void callback_no_esc_closes(
    Fl_Widget *w, void *x)
{
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape
    //TODO: message to backend?
    cout << "Closing " << Widget::get_widget_name(w) << endl;
    //TODO--
    exit(0);
}

// *** layout widgets – Fl_Group based

W_Group::W_Group(MMap* parammap) : Widget(parammap){}
//W_Group* W_Group::make(MMap* &parammap){}

void W_Group::handle_method(std::string_view method, minion::MList* &paramlist)
{
    if (method == "RESIZABLE") {
        auto rsw = Widget::get_widget(get<string>(paramlist.at(1)));
        fltk_widget()->as_group()->resizable(rsw);
    } else if (method == "fit_to_parent") {
        if (auto parent = fltk_widget()->parent()) {
            fltk_widget()->resize(0, 0, parent->w(), parent->h());
            parent->resizable(fltk_widget());
        } else {
            throw "Widget (" + string{widget_name()}  + ") method 'fit_to_parent': no parent";
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}


W_Window::W_Window(MMap* parammap) : W_Group{parammap}{}
W_Window* W_Window::make(MMap* parammap)
{
    int ww = 800;
    int wh = 600;
    parammap->get_int("WIDTH", ww);
    parammap->get_int("HEIGHT", wh);
    auto w = new Fl_Double_Window(ww, wh);
    int esc_closes{0};
    parammap->get_int("ESC_CLOSES", esc_closes);
    if (esc_closes != 0)
        w->callback(callback_no_esc_closes);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Window(parammap);
    widget->fl_widget = w;
    return widget;         
}

// Inherit handle_method from W_Group
//void W_Window::handle_method(std::string_view method, minion::MList* &paramlist);


W_Grid::W_Grid(minion::MMap* parammap) : W_Group{parammap}{}
W_Grid* W_Grid::make(minion::MMap* parammap)
{
    auto w = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid(parammap);
    widget->fl_widget = w;
    return widget;         
}
Fl_Widget *NEW_Grid(
    MMap* param)
{
    auto w = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    return w;
}

void W_Grid::handle_method(std::string_view method, minion::MList* paramlist)
{
    if (method == "GAP") {
        int szr = int_param(paramlist, 1);
        int szc = szr;
        if (paramlist->size() > 2) {
            szc = int_param(paramlist, 2);
        }
        static_cast<Fl_Grid *>(fltk_widget())->gap(szr, szc);
    } else if (method == "MARGIN") {
        int sz = int_param(paramlist, 1);
        static_cast<Fl_Grid *>(fltk_widget())->margin(sz, sz, sz, sz);
    } else {
        W_Group::handle_method(method, paramlist);
    }
}

W_Row::W_Row(minion::MMap* parammap) : W_Grid{parammap}{}
W_Row* W_Row::make(minion::MMap* &parammap)
{
    return new_hvgrid(parammap, true);

}

// Inherit handle_method from W_Grid
//void W_Row::handle_method(std::string_view method, minion::MList* &paramlist);


class W_Row : public Widget
{
public:
    W_Row(MMap* parammap) : Widget{parammap}
    {}

    static W_Row* make(MMap* &parammap);
};

class W_Column : public Widget
{
public:
    W_Column(MMap* parammap) : Widget{parammap}
    {}

    static W_Column* make(MMap* &parammap);
};

// *** End of layouts




void callback_no_esc_closes(
    Fl_Widget *w, void *x)
{
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape
    //TODO: message to backend?
    cout << "Closing " << WidgetData::get_widget_name(w) << endl;
    //TODO--
    exit(0);
}

Fl_Widget *new_hvgrid(
    MMap* &parammap,
    bool horizontal)
{
    auto w = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid(parammap);
    widget->fl_widget = w;

//TODO: Get pending items ...

    return widget;         



    auto widg = new Fl_Grid(0, 0, 0, 0);
    Fl_Group::current(0); // disable "auto-grouping"
    widg->color(0xe0ffe000);
    auto items = param.get("ITEMS");
    // The items are lists, the first element is the widget name,
    // subsequent elements are for "filling" and fixing.
    if (holds_alternative<MList*>(items)) {
        auto item_list = get<MList*>(items);
        int n_items = item_list.size();
        if (horizontal) widg->layout(1, n_items);
        else widg->layout(n_items, 1);
        for (int rc = 0; rc < n_items; ++rc) {
            auto item = get<MList*>(item_list.at(rc));
            int n_params = item.size();
            if (n_params != 0) {
                auto w = WidgetData::get_widget(get<string>(item.at(0)));
                widg->add(w);
                // now options
                for (int i = 1; i < item.size(); ++i) {
                    auto p =  get<string>(item.at(i));
                    if (p == "FIXED") {
                        if (horizontal) widg->col_weight(rc, 0);
                        else widg->row_weight(rc, 0);
                    } else {
                        try {
                            auto align = GRID_ALIGN.at(p);
                            if (horizontal) widg->widget(w, 0, rc, align);
                            else widg->widget(w, rc, 0, align);
                        } catch (out_of_range) {
                            throw "Invalid Grid flag: " + p;
                        }
                    }
                }
            }
        }
        return widg;
    }
    string s;
    minion::dump(s, items, 0);
    throw string{"Invalid ITEMS list: "} + s;    
}

Fl_Widget *NEW_Column(
    MMap* param)
{
    return new_hvgrid(param, false);
}

Fl_Widget *NEW_Row(
    MMap* param)
{
    return new_hvgrid(param, true);
}
