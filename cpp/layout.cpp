#include "layout.h"
#include "callback.h"
#include "minion.h"
#include "widget.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <fmt/format.h>
#include <iostream>
#include <map>
using namespace std;
using namespace minion;

inline const map<string, Fl_Grid_Align>
    GRID_ALIGN{/** Align the widget in the middle of the cell (default). */
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
               {"BOTTOM_RIGHT", FL_GRID_BOTTOM_RIGHT}};

void callback_no_esc_closes(
    Fl_Widget *w, void *x)
{
    (void) x;
    if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
        return; // ignore Escape
    //TODO: message to backend?
    cout << "Closing " << Widget::get_widget_name(w) << endl;
    //TODO--
    exit(0);
}

// *** layout widgets – Fl_Group based

// The Group widget is only needed for its `handle_method`.
void W_Group::handle_method(std::string_view method, minion::MList* paramlist)
{
    if (method == "RESIZABLE") {
        std::string wname;
        if (paramlist->get_string(1, wname)) {
            auto rsw = Widget::get_fltk_widget(wname);
            fl_widget->as_group()->resizable(rsw);
        } else {
            throw "Method RESIZABLE without widget";
        }
    } else if (method == "fit_to_parent") {
        if (auto parent = fltk_widget()->parent()) {
            fltk_widget()->resize(0, 0, parent->w(), parent->h());
            parent->resizable(fltk_widget());
        } else {
            throw "Widget (" + *widget_name() + ") method 'fit_to_parent': no parent";
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}


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
    auto widget = new W_Window();
    widget->fl_widget = w;
    return widget;         
}

W_Grid* W_Grid::make(minion::MMap* parammap)
{
    (void) parammap;
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid();
    widget->fl_widget = w;

//TODO: adding widgets ...

    return widget;         
}

void W_Grid::handle_method(std::string_view method, minion::MList* paramlist)
{
    if (method == "GAP") {
        int rowgap;
        if (paramlist->get_int(1, rowgap)) {
            int colgap = rowgap;
            if (paramlist->size() > 2)
                paramlist->get_int(2, colgap);
            static_cast<Fl_Grid *>(fl_widget)->gap(rowgap, colgap);
            return;   
        }
    } else if (method == "MARGIN") {
        int s;
        if (paramlist->get_int(1, s)) {
            static_cast<Fl_Grid *>(fl_widget)->margin(s, s, s, s);
            return;
        }
    } else {
        W_Group::handle_method(method, paramlist);
        return;
    }
    MValue m{*paramlist};
    throw string{"Invalid command on grid '" + *widget_name()}.append("':\n  ").append(
        dump_value(m));
}

//static
W_Grid* W_Grid::new_hvgrid(
    MMap* parammap,
    bool horizontal)
{
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid();
    widget->fl_widget = w;

    // Get contained widgets ...
    auto wlist0 = parammap->get("WIDGETS");
    auto wlist = wlist0.m_list()->get();
    auto n = wlist->size();
    if (horizontal) w->layout(1, n);
    else w->layout(n, 1);
    for (size_t i = 0; i < n; ++i) {
        auto entry0 = wlist->get(i);
        auto entry = entry0.m_list()->get();
        string wname;
        if (entry->get_string(0, wname)) {
            if (auto w_i = Widget::get_fltk_widget(wname)) {
                w->add(w_i);
                string fill;
                if (entry->get_string(1, fill)) {
                    try {
                        auto align = GRID_ALIGN.at(fill);
                        if (horizontal) w->widget(w_i, 0, i, align);
                        else w->widget(w_i, i, 0, align);
                    } catch (out_of_range& e) {
                        throw string{"Invalid Grid alignment (fill): "} + fill;
                    }
                    int weight = 1;
                    entry->get_int(2, weight);
                    if (horizontal) w->col_weight(i, weight);
                    else w->row_weight(i, weight);
                    continue;   
                }
            }
        }
        throw string{"Invalid ITEMS list: "} + dump_value(entry0);
    }
    return widget;         
}
