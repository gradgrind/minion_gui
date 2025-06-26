#include "callback.h"
#include "layout.h"
#include "minion.h"
#include "widget.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Wizard.H>
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

    w->hide();
}

// *** layout widgets – Fl_Group based

void W_Window::make_window(
    int ww, int wh, W_Window* widget, MMap* props)
{
    props->get_int("WIDTH", ww);
    props->get_int("HEIGHT", wh);
    auto w = new Fl_Double_Window(ww, wh);
    w->callback(callback_close_window);
    widget->container = new Fl_Flex(0, 0, 0, 0);
    //w->box(FL_EMBOSSED_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    widget->fl_widget = w;
    props->get_int("MIN_WIDTH", ww);
    props->get_int("MIN_HEIGHT", wh);
    w->size_range(ww, wh);

    widget->container->resize(0, 0, w->w(), w->h());
    w->resizable(widget->container);
}

W_Window* W_Window::make(
    MMap* props)
{
    auto widget = new W_Window();
    make_window(800, 600, widget, props);
    return widget;
}

void W_Window::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "SHOW") {
        container->layout();
        fl_widget->show();
    } else if (method == "TEXT") {
        string lbl;
        if (paramlist->get_string(1, lbl)) {
            fl_widget->copy_label(lbl.c_str());
        } else
            throw "TEXT value missing for window " + *widget_name();
    } else if (method == "SET_LAYOUT") {
        string w;
        if (paramlist->get_string(1, w)) {
            if (container->children() != 0) {
                throw "Window " + *widget_name() + " already has child: ";
            }
            container->add(get_fltk_widget(w));
        } else
            throw "SET_LAYOUT widget missing for window " + *widget_name();
    } else {
        throw string{"Unknown method on window " + *widget_name() + ": "}.append(method);
    }
}

W_Grid* W_Grid::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Grid();
    widget->fl_widget = w;
    return widget;
}

//TODO: As Fl_Grid allows gaps to be set individually, but not to read
// these gaps, this will only work if all gaps are equal.
void get_grid_sizes(Fl_Grid* gw, int nrows, int ncols)
{
    vector<int> rsize(nrows, 0);
    vector<int> csize(ncols, 0);
    for (int r = 0; r < nrows; ++r) {
        for (int c = 0; c < ncols; ++c) {
            auto cel = gw->cell(r, c);
            if (cel) {
                int w, h;
                cel->minimum_size(&w, &h);
                if (cel->colspan() == 1 && w > csize.at(c))
                    csize[c] = w;
                if (cel->rowspan() == 1 && h > rsize.at(r))
                    rsize[r] = h;
            }
        }
    }
    int l, t, r, b;
    gw->margin(&l, &t, &r, &b);
    int rg, cg;
    gw->gap(&rg, &cg);
    auto ww = Fl::box_dx(gw->box()) * 2 + l + r + cg * (ncols - 1);
    for (const auto wi : csize)
        ww += wi;
    auto wh = Fl::box_dy(gw->box()) * 2 + t + b + rg * (nrows - 1);
    for (const auto hi : rsize)
        wh += hi;
    printf("GRID SIZE: %d %d => %d %d\n", nrows, ncols, ww, wh);
    fflush(stdout);
    //TODO: Set size / minimum_size of grid in parent
    W_Group::set_child_size(gw, ww, wh);
}

void W_Grid::handle_child_size(Fl_Widget* wc, int ww, int wh)
{
    auto gw = static_cast<Fl_Grid*>(fl_widget);
    gw->cell(wc)->minimum_size(ww, wh);
    gw->layout();
    get_grid_sizes(gw, nrows, ncols);
}

/*
void W_Grid::handle_child_modified(Widget* wc)
{
    auto fc = wc->fltk_widget();
    auto c = static_cast<Fl_Grid*>(fl_widget)->cell(fc);
    c->minimum_size(fc->w(), fc->h());
    static_cast<Fl_Grid*>(fl_widget)->layout();
    get_grid_sizes(static_cast<Fl_Grid*>(fl_widget), nrows, ncols);
}
*/

//TODO: What about changing min col size when a text changes?
void W_Grid::handle_method(
    string_view method, MList* paramlist)
{
    if (method == "RC") { //TODO: May be unnecessary?
        // ADD automatically expands the layout if necessary.
        //TODO: Shrinking is not currently supported.
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        if (fw->children() != 0) {
            fw->clear_layout();
        }
        int r, c;
        if (paramlist->get_int(1, r) && paramlist->get_int(2, c)) {
            fw->layout(r, c);
            nrows = r;
            ncols = c;
            return;
        }
        throw "RC command needs two values (rows and columns), grid: " + *widget_name();
    }

    if (method == "ADD") {
        // Add new children to map
        auto flgrid = static_cast<Fl_Grid*>(fl_widget);
        auto n = paramlist->size();
        for (size_t i = 1; i < n; ++i) {
            auto w_i = paramlist->get(i);
            auto wlistp = w_i.m_list();
            if (wlistp) {
                auto wlist = wlistp->get();
                string wname;
                if (wlist->get_string(0, wname)) {
                    auto wchild = get_widget(wname);
                    auto flchild = wchild->fltk_widget();
                    if (flgrid->cell(flchild))
                        throw "Widget " + wname + " already in layout " + *widget_name();
                    int row, col, rspan = 1, cspan = 1;
                    if (wlist->get_int(1, row) && wlist->get_int(2, col)) {
                        wlist->get_int(3, rspan);
                        wlist->get_int(4, cspan);
                        flgrid->add(flchild);

                        Fl_Grid_Align align = FL_GRID_CENTER;
                        string fill;
                        if (wchild->property_string("GRID_ALIGN", fill)) {
                            try {
                                align = GRID_ALIGN.at(fill);
                            } catch (out_of_range& e) {
                                throw string{"Invalid GRID_ALIGN: "} + fill;
                            }
                        }

                        // Place the child widget ...
                        //  ... first check rows and cols (with spans)
                        if (row >= 0 && col >= 0 && rspan > 0 && cspan > 0) {
                            if (col + cspan > ncols) {
                                ncols = col + cspan;
                            }
                            if (row + rspan > nrows) {
                                nrows = row + rspan;
                            }
                            flgrid->layout(nrows, ncols);
                            flgrid->widget(flchild, row, col, rspan, cspan, align);
                        } else {
                            throw "Widget " + *wchild->widget_name()
                                + ", Grid placement invalid: \n " + to_string(row) + "+"
                                + to_string(rspan) + " / " + to_string(col) + "+" + to_string(cspan)
                                + "\n Layout: " + to_string(nrows) + " / " + to_string(ncols);
                        }
                        continue;
                    }
                    throw "Grid ADD command needs two values (row and column), grid: "
                        + *widget_name();
                }
            }
            throw "Invalid grid ADD command on '" + *widget_name() + "':\n  " + dump_value(w_i);
        }
        //fw->layout();
        return;
    }

    // Set row weights
    if (method == "ROW_WEIGHTS") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        auto n = paramlist->size();
        for (size_t i = 1; i < n; ++i) {
            auto m = paramlist->get(i);
            if (auto wlp = m.m_list()) {
                auto wl = wlp->get();
                int row, wt;
                if (wl->get_int(0, row) && wl->get_int(1, wt)) {
                    fw->row_weight(row, wt);
                    continue;
                }
            }
            throw "Invalid ROW_WEIGHTS value: " + dump_value(m);
        }

        //fw->layout();
        return;
    }

    // Set column weights
    if (method == "COL_WEIGHTS") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        auto n = paramlist->size();
        for (size_t i = 1; i < n; ++i) {
            auto m = paramlist->get(i);
            if (auto wlp = m.m_list()) {
                auto wl = wlp->get();
                int col, wt;
                if (wl->get_int(0, col) && wl->get_int(1, wt)) {
                    fw->col_weight(col, wt);
                    continue;
                }
            }
            throw "Invalid COL_WEIGHTS value: " + dump_value(m);
        }

        //fw->layout();
        return;
    }

    if (method == "GAP") {
        int rowgap;
        if (paramlist->get_int(1, rowgap)) {
            auto fw = static_cast<Fl_Grid*>(fl_widget);
            int colgap = rowgap;
            paramlist->get_int(2, colgap);
            fw->gap(rowgap, colgap);
            //fw->layout();
            return;
        }
        throw "GAP command with no gap on layout '" + *widget_name();
    }

    if (method == "MARGIN") {
        int s;
        if (paramlist->get_int(1, s)) {
            auto fw = static_cast<Fl_Grid*>(fl_widget);
            fw->margin(s, s, s, s);
            //fw->layout();
            return;
        }
        throw "MARGIN command with no margin on layout '" + *widget_name();
    }

    if (method == "SHOW_GRID") {
        int show_grid = 0;
        paramlist->get_int(1, show_grid);
        static_cast<Fl_Grid*>(fl_widget)->show_grid(show_grid);
        return;
    }

    Widget::handle_method(method, paramlist);
    return;
}

//static
W_Layout* W_Layout::new_hvgrid(
    MMap* props, bool horizontal)
{
    (void) props;
    auto w = new Fl_Grid(0, 0, 0, 0);
    w->box(FL_NO_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Layout();
    widget->fl_widget = w;
    widget->horizontal = horizontal;
    return widget;
}

void W_Layout::set_transverse_size()
{
    // Take the padding and box type into account
    int boxsize = padding * 2;
    if (horizontal) {
        boxsize += Fl::box_dh(fl_widget->box());
    } else {
        boxsize += Fl::box_dw(fl_widget->box());
    }
    int xsize = 0;
    for (const auto wc : children) {
        auto wfltk = wc->fltk_widget();
        auto xs = horizontal ? wfltk->h() : wfltk->w();
        if (xs > xsize)
            xsize = xs;
    }
    if (horizontal)
        static_cast<Fl_Grid*>(fl_widget)->size(0, xsize + boxsize);
    else
        static_cast<Fl_Grid*>(fl_widget)->size(xsize + boxsize, 0);
}

/*
void W_Layout::handle_child_modified(
    Widget* wc)
{
    //TODO: I think this needs handling specially for the 1-d grid ...
    auto fc = wc->fltk_widget();
    int ww = fl_widget->w(), hh = fl_widget->h();
    printf("PARENT-SIZE %d %d\n", ww, hh);
    auto c = static_cast<Fl_Grid*>(fl_widget)->cell(fc);
    ww = fc->w(), hh = fc->h();
    c->minimum_size(ww, hh);
    printf("CHILD-SIZE %d %d\n", ww, hh);

    auto gw = static_cast<Fl_Grid*>(fl_widget);
    gw->layout();
    printf("PARENT-RESIZE %d %d\n", fl_widget->w(), fl_widget->h());
    fflush(stdout);
    //child_size_modified(this);
    if (horizontal)
        get_grid_sizes(gw, 1, gw->children());
    else
        get_grid_sizes(gw, gw->children(), 1);
}
*/

void W_Layout::handle_child_size(Fl_Widget* wc, int ww, int wh)
{
    auto gw = static_cast<Fl_Grid*>(fl_widget);
    gw->cell(wc)->minimum_size(ww, wh);
    gw->layout();
    if (horizontal)
        get_grid_sizes(gw, 1, gw->children());
    else
        get_grid_sizes(gw, gw->children(), 1);
}

void W_Layout::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "ADD") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        auto n = paramlist->size();
        if (n < 2)
            throw "No widget(s) to ADD to " + *widget_name();
        auto n0 = children.size();
        // Add new children to list
        for (size_t i = 1; i < n; ++i) {
            string wname;
            if (paramlist->get_string(i, wname)) {
                auto wc = get_widget(wname);
                if (std::find(children.begin(), children.end(), wc) != children.end())
                    throw "Widget " + wname + " already in layout " + *widget_name();
                fw->add(wc->fltk_widget());
                children.emplace_back(wc);
            } else {
                MValue m0{*paramlist};
                throw "Invalid ADD to layout " + *widget_name() + ": " + dump_value(m0);
            }
        }
        // Lay out the grid
        n = children.size();
        if (horizontal)
            fw->layout(1, n);
        else
            fw->layout(n, 1);
        for (size_t i = n0; i < n; ++i) {
            auto wc = children.at(i);
            Fl_Grid_Align align = FL_GRID_CENTER;
            string fill;
            if (wc->property_string("GRID_ALIGN", fill)) {
                try {
                    align = GRID_ALIGN.at(fill);
                } catch (out_of_range& e) {
                    throw string{"Invalid GRID_ALIGN: "} + fill;
                }
            }
            if (horizontal)
                fw->widget(wc->fltk_widget(), 0, i, align);
            else
                fw->widget(wc->fltk_widget(), i, 0, align);
            int weight = 0;
            wc->property_int("GRID_GROW", weight);
            if (horizontal)
                fw->col_weight(i, weight);
            else
                fw->row_weight(i, weight);
        }

        if (horizontal)
            get_grid_sizes(fw, 1, n);
        else
            get_grid_sizes(fw, n, 1);

        /*string autosize;
        if (property_string("AUTOSIZE", autosize) && !autosize.empty())
            set_transverse_size();
        //fw->layout(); // lay out container
        */

        return;
    }

    if (method == "GAP") {
        int gap;
        if (paramlist->get_int(1, gap)) {
            if (horizontal)
                static_cast<Fl_Grid*>(fl_widget)->gap(0, gap);
            else
                static_cast<Fl_Grid*>(fl_widget)->gap(gap, 0);
            return;
        }
        throw "GAP command with no gap on layout '" + *widget_name();
    }

    if (method == "MARGIN") {
        if (paramlist->get_int(1, padding)) {
            static_cast<Fl_Grid*>(fl_widget)->margin(padding, padding, padding, padding);
            string autosize;
            if (property_string("AUTOSIZE", autosize) && !autosize.empty())
                set_transverse_size();
            return;
        }
        throw "MARGIN command with no margin on layout '" + *widget_name();
    }

    Widget::handle_method(method, paramlist);
}

//TODO?
void W_Stack::handle_child_size(Fl_Widget* wc, int ww, int wh)
{
    (void) wc;
    (void) ww;
    (void) wh;
    return;
}

/*    
void W_Stack::handle_child_modified(
    Widget* wc)
{
    return;
    throw "TODO: W_Stack::handle_child_modified";
}
*/

void W_Stack::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "ADD") {
        string wname;
        if (paramlist->get_string(1, wname)) {
            fl_widget->as_group()->add(get_fltk_widget(wname));
        }
    } else if (method == "SELECT") {
        string wname;
        if (paramlist->get_string(1, wname)) {
            auto subw = Widget::get_fltk_widget(wname);
            if (current)
                current->hide();
            subw->show();
            subw->take_focus();
            current = subw;
            static_cast<Fl_Flex*>(fl_widget)->layout();
        } else {
            throw "Method SELECT without widget";
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}

//static method
W_Stack* W_Stack::make(
    minion::MMap* props)
{
    (void) props;
    auto w = new Fl_Flex(0, 0, 0, 0);
    w->box(FL_EMBOSSED_BOX);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Stack();
    widget->fl_widget = w;

    return widget;
}
