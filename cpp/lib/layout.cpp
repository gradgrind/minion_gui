#include "layout.h"
#include "callback.h"
#include "minion.h"
#include "widget.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
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

void callback_close_window(
    Fl_Widget* w, void* ud)
{
    //(void) ud;
    if (Fl::callback_reason() == FL_REASON_CANCELLED) {
        // escape key pressed
        //TODO--
        cout << "Escape key pressed" << endl;

        int esc_quit = 0;
        static_cast<Widget*>(ud)->property_int("ESC_CLOSES", esc_quit);
        if (esc_quit == 0)
            return;
    }

    //TODO: message to backend?

    //TODO: If changed data, ask about closing
    //if (!fl_choice("Are you sure you want to quit?", "continue", "quit", NULL)) {
    //    return;
    //}

    //TODO--
    cout << "Closing " << *Widget::get_widget_name(w) << endl;
    w->hide();
}

// *** layout widgets – Fl_Group based

// The Group widget is only needed for its `handle_method`.
void W_Group::handle_method(
    string_view method, MList* paramlist)
{
    if (method == "RESIZABLE") {
        string wname;
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

W_Window* W_Window::make(
    MMap* props)
{
    int ww = 800;
    int wh = 600;
    props->get_int("WIDTH", ww);
    props->get_int("HEIGHT", wh);
    auto w = new Fl_Double_Window(ww, wh);
    w->callback(callback_close_window);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Window();
    widget->fl_widget = w;
    props->get_int("MIN_WIDTH", ww);
    props->get_int("MIN_HEIGHT", wh);
    w->size_range(ww, wh);
    return widget;
}

struct grid_item
{
    Fl_Widget* widget;
    int row = 0;
    int col = 0;
    int rspan = 1;
    int cspan = 1;
    Fl_Grid_Align align = FL_GRID_CENTER;
};

W_Grid* W_Grid::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid();
    widget->fl_widget = w;

    // Get contained widgets ...
    auto wlist0 = props->get("WIDGETS");
    if (!wlist0.is_null()) {
        auto wlist = wlist0.m_list()->get();
        auto n = wlist->size();
        if (n != 0) {
            // Need to determine the dimensions
            int maxrow = 0, maxcol = 0;
            string wname;
            string fill;
            vector<grid_item> items;
            for (size_t i = 0; i < n; ++i) {
                auto witem = wlist->get(i);
                if (!witem.is_null()) {
                    auto wl_i = witem.m_list();
                    if (wl_i) {
                        auto list_i = wl_i->get();
                        if (list_i->get_string(0, wname)) {
                            if (auto w_i = Widget::get_widget(wname)) {
                                auto wfltk = w_i->fltk_widget();
                                w->add(wfltk);
                                grid_item item{wfltk};
                                list_i->get_int(1, item.row);
                                if (item.row > maxrow)
                                    maxrow = item.row;
                                list_i->get_int(2, item.col);
                                if (item.col > maxcol)
                                    maxcol = item.col;
                                list_i->get_int(3, item.rspan);
                                list_i->get_int(4, item.cspan);
                                if (w_i->property_string("GRID_ALIGN", fill)) {
                                    try {
                                        item.align = GRID_ALIGN.at(fill);
                                    } catch (out_of_range& e) {
                                        throw string{"Invalid GRID_ALIGN: "} + fill;
                                    }
                                }
                                items.emplace_back(item);
                                continue;
                            }
                        }
                    }
                }
                throw "Layout with invalid WIDGETS list: " + *widget->widget_name();
            }

            // Place the widgets in the grid
            w->layout(maxrow + 1, maxcol + 1);
            for (const auto& item : items) {
                w->widget(item.widget, item.row, item.col, item.rspan, item.cspan, item.align);
            }

            // Row and column weights ...
            auto wmap0 = props->get("ROW_WEIGHTS");
            if (!wmap0.is_null()) {
                if (auto wmap = wmap0.m_list()) {
                    auto wl = wmap->get();
                    auto n = wl->size();
                    for (size_t i = 0; i < n; ++i) {
                        auto witem = wl->get(i);
                        if (auto wil0 = witem.m_list()) {
                            auto wil = wil0->get();
                            int ix = -1;
                            wil->get_int(0, ix);
                            if (ix >= 0 && ix <= maxrow) {
                                int weight = -1;
                                wil->get_int(1, weight);
                                if (weight >= 0) {
                                    w->row_weight(ix, weight);
                                }
                            }
                        }
                    }
                }
            }

            wmap0 = props->get("COL_WEIGHTS");
            if (!wmap0.is_null()) {
                if (auto wmap = wmap0.m_list()) {
                    auto wl = wmap->get();
                    auto n = wl->size();
                    for (size_t i = 0; i < n; ++i) {
                        auto witem = wl->get(i);
                        if (auto wil0 = witem.m_list()) {
                            auto wil = wil0->get();
                            int ix = -1;
                            wil->get_int(0, ix);
                            if (ix >= 0 && ix <= maxcol) {
                                int weight = -1;
                                wil->get_int(1, weight);
                                if (weight >= 0) {
                                    w->col_weight(ix, weight);
                                }
                            }
                        }
                    }
                }
            }

            return widget;
        }
    }
    throw "Layout with no WIDGETS list: " + *widget->widget_name();
}

void W_Grid::handle_method(
    string_view method, MList* paramlist)
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
    } else if (method == "SHOW_GRID") {
        int show_grid = 0;
        paramlist->get_int(1, show_grid);
        static_cast<Fl_Grid*>(fl_widget)->show_grid(show_grid);
        return;
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
    MMap* props, bool horizontal)
{
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid();
    widget->fl_widget = w;

    // Get contained widgets ...
    auto wlist0 = props->get("WIDGETS");
    if (!wlist0.is_null()) {
        auto wlist = wlist0.m_list()->get();
        auto n = wlist->size();
        if (n != 0) {
            if (horizontal)
                w->layout(1, n);
            else
                w->layout(n, 1);
            int xsize = 0; // find transverse size
            for (size_t i = 0; i < n; ++i) {
                string wname;
                if (wlist->get_string(i, wname)) {
                    if (auto w_i = Widget::get_widget(wname)) {
                        auto wfltk = w_i->fltk_widget();
                        w->add(wfltk);
                        auto xs = horizontal ? wfltk->h() : wfltk->w();
                        if (xs > xsize)
                            xsize = xs;
                        Fl_Grid_Align align = FL_GRID_CENTER;
                        string fill;
                        if (w_i->property_string("GRID_ALIGN", fill)) {
                            try {
                                align = GRID_ALIGN.at(fill);
                            } catch (out_of_range& e) {
                                throw string{"Invalid GRID_ALIGN: "} + fill;
                            }
                        }
                        if (horizontal)
                            w->widget(wfltk, 0, i, align);
                        else
                            w->widget(wfltk, i, 0, align);
                        int weight = 0;
                        w_i->property_int("GRID_GROW", weight);
                        if (horizontal)
                            w->col_weight(i, weight);
                        else
                            w->row_weight(i, weight);
                        continue;
                    }
                }
                throw "Layout with invalid WIDGETS list: " + *widget->widget_name();
            }
            if (horizontal)
                w->size(0, xsize);
            else
                w->size(xsize, 0);
            return widget;
        }
    }
    throw "Layout with no WIDGETS list: " + *widget->widget_name();
}
