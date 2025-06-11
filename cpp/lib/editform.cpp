#include "callback.h"
#include "layout.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/fl_draw.H>
#include <string>
using namespace std;
using namespace minion;

//TODO: How to do the callbacks? Entry name + current value?
//TODO: How to write to the entries?
//  Entry_name + new_value (ENTRY)
//  Entry_name + [new_value ...] (LIST)

class EditForm : public Fl_Grid
{
public:
    int v_title_gap{5};

    EditForm()
        : Fl_Grid(0, 0, 0, 0)
    {
        Fl_Group::current(0); // disable "auto-grouping"
        box(FL_BORDER_FRAME);
        gap(10, 5);
        margin(5, 5, 5, 5);
    }
};

// static
W_EditForm* W_EditForm::make(
    minion::MMap* props)
{
    // Now create the EditForm widget
    (void) props;
    auto efw = new EditForm();
    auto widget = new W_EditForm();
    widget->fl_widget = efw;
    //efw->color(Widget::entry_bg);
    return widget;
}

void W_EditForm::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    //TODO
    if (method == "ADD") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        if (fw->children() != 0) {
            fw->clear_layout();
        }
        auto n = paramlist->size();
        if (n < 2)
            throw "No widget(s) to ADD to " + *widget_name();
        // Add new children to list
        for (size_t i = 1; i < n; ++i) {
            string wname;
            if (paramlist->get_string(i, wname)) {
                auto wc = get_widget(wname);
                if (std::find(children.begin(), children.end(), wc) != children.end())
                    throw "Widget " + wname + " already in layout " + *widget_name();
                fw->add(wc->fltk_widget());
                children.emplace_back(wc);
            }
        }
        // Lay out the grid
        n = children.size();
        fw->layout(n, 2);
        fw->col_weight(0, 0);
        if (horizontal)
            fw->layout(1, n);
        else
            fw->layout(n, 1);
        int xsize = 0; // find transverse size
        int i = 0;     // child index
        for (const auto wc : children) {
            auto wfltk = wc->fltk_widget();
            auto xs = horizontal ? wfltk->h() : wfltk->w();
            if (xs > xsize)
                xsize = xs;
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
                fw->widget(wfltk, 0, i, align);
            else
                fw->widget(wfltk, i, 0, align);
            int weight = 0;
            wc->property_int("GRID_GROW", weight);
            if (horizontal)
                fw->col_weight(i, weight);
            else
                fw->row_weight(i, weight);
            ++i;
        }
        if (horizontal)
            fw->size(0, xsize);
        else
            fw->size(xsize, 0);
        //fw->layout(); // lay out container
        return;
    }

    if (method == "GAP") {
        int rowgap;
        if (paramlist->get_int(1, rowgap)) {
            int colgap = rowgap;
            paramlist->get_int(2, colgap);
            static_cast<Fl_Grid*>(fl_widget)->gap(rowgap, colgap);
            return;
        }
        throw "GAP command with no gap on layout '" + *widget_name();
    }

    if (method == "MARGIN") {
        int s;
        if (paramlist->get_int(1, s)) {
            static_cast<Fl_Grid*>(fl_widget)->margin(s, s, s, s);
            return;
        }
        throw "MARGIN command with no margin on layout '" + *widget_name();
    }

    Widget::handle_method(method, paramlist);

    //old:

    auto wlist0 = parammap->get("WIDGETS");
    if (auto wlist = wlist0.m_list()) {
        auto mlist = wlist->get();
        auto n = mlist->size();

        efw->layout(n, 2);
        efw->col_weight(0, 0);

        if (n != 0) {
            vector<Widget*> children;
            for (size_t i = 0; i < n; ++i) {
                string wname;
                try {
                    mlist->get_string(i, wname);
                } catch (...) {
                    string efname;
                    parammap->get_string("NAME", efname);
                    MValue m = *mlist;
                    throw string{"Invalid WIDGETS list for widget "}
                        .append(efname)
                        .append(": ")
                        .append(dump_value(m));
                }
                children.emplace_back(Widget::get_widget(wname));
            }

            int label_width{0};
            Fl_Align align{FL_ALIGN_LEFT | FL_ALIGN_INSIDE};
            string algn;
            parammap->get_string("LABEL_ALIGN", algn);
            if (algn == "CENTRE") {
                align = FL_ALIGN_CENTER;
            } else if (algn == "RIGHT") {
                align = FL_ALIGN_RIGHT | FL_ALIGN_INSIDE;
            }
            for (size_t i = 0; i < n; ++i) {
                auto w = children.at(i);
                int span = 1;
                w->property_int("SPAN", span);
                int grow = 0;
                w->property_int("GROW", grow);
                string label;
                w->property_string("LABEL", label);
                auto wx = w->fltk_widget();
                if (span == 2) {
                    // If there is a label, it will need to come first. Bundle
                    // label and widget together in a flex layout.
                    if (label.empty()) {
                        efw->add(wx);
                        efw->widget(wx, i, 0, 1, 2);
                    } else {
                        auto gbox = new Fl_Flex(0, 0, 0, 0);
                        auto wl = new Fl_Box(0, 0, 0, 0);
                        Fl_Group::current(0); // disable "auto-grouping"
                        gbox->add(wx);
                        efw->add(gbox);
                        wl->copy_label(label.c_str());
                        wl->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
                        // Measure label dimensions
                        int wlw{0}, wlh;
                        wl->measure_label(wlw, wlh);
                        gbox->size(wlw, wlh + efw->v_title_gap + wx->h());
                        gbox->fixed(wl, wlh);
                        gbox->gap(efw->v_title_gap);
                        efw->widget(gbox, i, 0, 1, 2);
                    }
                    efw->row_weight(i, grow);
                } else {
                    efw->add(wx);
                    // If there is a label, make a labelled box for the
                    // first column.
                    if (!label.empty()) {
                        auto wl = new Fl_Box(0, 0, 0, 0);
                        wl->copy_label(label.c_str());
                        wl->align(align);
                        efw->add(wl);
                        efw->widget(wl, i, 0);
                        // Measure width, compare with running maximum
                        int wlw{0}, wlh;
                        wl->measure_label(wlw, wlh);
                        if (wlw > label_width) {
                            label_width = wlw;
                        }
                    }
                    efw->widget(wx, i, 1);
                    efw->row_weight(i, 0);
                }
            }
            efw->col_width(0, label_width);
            return widget;
        }
    }
    string efname;
    parammap->get_string("NAME", efname);
    throw "EditForm missing WIDGETS list: " + efname;
}
