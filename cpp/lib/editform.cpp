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
        //col_width(0, 0);
        //col_weight(0, 0);
        gap(10, 0);
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

/*
void W_EditForm::handle_child_size(Fl_Widget* wc, int ww, int wh)
{
    auto gw = static_cast<Fl_Grid*>(fl_widget);
    gw->cell(wc)->minimum_size(ww, wh);
    gw->layout();
    get_grid_sizes(gw, children.size(), 2);
}
*/

/*    
void W_EditForm::handle_child_modified(
    Widget* wc)
{
    throw "TODO: W_EditForm::handle_child_modified";
}
*/

//TODO
void W_EditForm::dolayout()
{
    auto efw = static_cast<Fl_Grid*>(fl_widget);
    // Label alignment
    //Fl_Align align{FL_ALIGN_LEFT};
    int align = -1; // left
    {
        string align_;
        property_string("LABEL_ALIGN", align_);
        if (align_ == "CENTRE") {
            align = 0;
        } else if (align_ == "RIGHT") {
            align = 1;
        }
    }

    int label_width = 0;
    int widget_height = margin0 * 2;
    int i = 0;
    int gap;
    efw->gap(rowgap0);                               // set default row-gap
    efw->margin(margin0, margin0, margin0, margin0); // set default margin
    for (const auto& elem : children) {
        auto fwc = elem.element->fltk_widget();
        auto fwl = dynamic_cast<W_Labelled_Widget*>(fwc);
        if (elem.span == 0) {
            // If there is an external label, use it for calculating
            // left gap size.
            if (fwl && fwl->label_width > label_width)
                label_width = fwl->label_width;
            if (i != 0) {
                widget_height += rowgap0;
            }

        } else {
            //TODO: if there is an external label, use it for calculating
            // top gap size.
            if (fwl && fwl->label_height != 0) {
                if (i == 0) {
                    // Modify top margin
                    gap = fwl->label_height + v_label_gap;
                    efw->margin(-1, margin0 + gap);
                } else {
                    // Modify gap
                    gap = rowgap0 + fwl->label_height + v_label_gap;
                    efw->row_gap(i - 1, gap);
                }
                fwc->vertical_label_margin(v_label_gap);
                widget_height += gap;
            } else {
                if (i != 0) {
                    widget_height += rowgap0;
                }
            }
        }

        //TODO: Need to consider the widget's own height ...

        ++i;
    }

    /*
    {
        {
            int span = 0;
            wc->property_int("SPAN", span);

            if (span == 0) {
                fw->add(fwc);

            } else {
                //TODO
                // If there is a label, it will be placed above the widget,
                // the space above the widget (gap or margin) will need
                // to be expanded to make room for it.
                if (wc->label_width == 0) {
                    fw->add(fwc);
                } else {
                    auto gbox = new Fl_Flex(0, 0, 0, 0);
                    auto wl = new Fl_Box(0, 0, 0, 0);
                    Fl_Group::current(0); // disable "auto-grouping"
                    gbox->add(fwc);
                    wlabel = gbox;
                    // Note that if this element is removed from the form,
                    // the element's main widget (here fwc) must be removed
                    // from the Flex layout before deleting the latter,
                    // assuming that fwc is managed by its Widget object.

                    fw->add(gbox);
                    wl->copy_label(label.c_str());
                    wl->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

                    //TODO: If removal is to be a possibility, this `wlabel`
                    // contains the actual widget (`fwc`). So if the label is
                    // deleted, `fwc` must be removed from it first, as this
                    // is managed by its associated `Widget` object.
                }
            }

            children.emplace_back(form_element{wc, wlabel, span});
        }
    }
    */

    /*
    // Lay out the grid
    fw->layout(n, 2);
    fw->col_weight(0, 0);
    int label_width = 0;

    for (int i = n0; i < nc; ++i) {
        auto el = children.at(i);
        if (el.span == 0) {
            if (el.label) {
                fw->widget(el.label, i, 0);
                /* Measure width, compare with running maximum
                    int wlw = 0, wlh;
                    el.label->measure_label(wlw, wlh);
                    if (wlw > label_width) {
                        label_width = wlw;
                    }* /
            }
            fw->widget(el.element->fltk_widget(), i, 1);
            fw->row_weight(i, 0);

        } else {
            if (el.label) { // Label above element
                // Measure label dimensions
                int wlw{0}, wlh;
                auto wlbl = static_cast<Fl_Flex*>(el.label)->child(0);
                wlbl->measure_label(wlw, wlh);
                el.label->size(wlw, wlh + v_label_gap + el.element->fltk_widget()->h());
                static_cast<Fl_Flex*>(el.label)->fixed(wlbl, wlh);
                static_cast<Fl_Flex*>(el.label)->gap(v_label_gap);
                fw->widget(el.label, i, 0, 1, 2);
            } else { // No label
                fw->widget(el.element->fltk_widget(), i, 0, 1, 2);
            }
            if (el.span != 1)
                fw->row_weight(i, 1);
            else
                fw->row_weight(i, 0);
        }
    }

    get_grid_sizes(fw, nc, 2);
    //fw->col_width(0, label_width);
    */
}


void W_EditForm::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "ADD") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);

        // Add new children to list
        auto n = paramlist->size();
        if (n < 2)
            throw "No widget(s) to ADD to " + *widget_name();
        auto n0 = fw->children();
        auto nc = n0;
        for (size_t i = 1; i < n; ++i) {
            string wname;
            if (paramlist->get_string(i, wname)) {
                auto wc = get_widget(wname);
                auto fwc = wc->fltk_widget();
                // Check that it is new to the layout
                for (int i = 0; i < nc; ++i) {
                    if (fwc == fw->child(i))
                        throw "Widget " + wname + " already in layout " + *widget_name();
                }
                fw->add(fwc);
                int span = 0;
                wc->property_int("SPAN", span);
                children.emplace_back(form_element{wc, span});
                ++nc;
            }
        }
        fw->layout(nc, 2);
        fw->col_weight(0, 0);
        for (int i = n0; i < nc; ++i) {
            auto fe = children.at(i);
            auto wc = fe.element;
            if (fe.span == 0) {
                fw->widget(wc->fltk_widget(), i, 1);
                fw->row_weight(i, 0);
            } else {
                fw->widget(wc->fltk_widget(), i, 0, 1, 2);
                if (fe.span == 1)
                    fw->row_weight(i, 0);
                else
                    fw->row_weight(i, 1);
            }
        }
        dolayout();
        return;
    }

    if (method == "LABEL_POS") {
        // This can be overridden by the LABEL_POS of the widgets
        string align;
        paramlist->get_string(1, align);
        if (align == "LEFT")
            label_pos = -1;
        else if (align == "RIGHT")
            label_pos = 1;
        else if (align == "CENTRE")
            label_pos = 0;
        else
            throw "No valid LABEL_POS value for " + *widget_name();
        dolayout();
        return;
    }

    if (method == "GAP") {
        if (paramlist->get_int(1, rowgap0)) {
            colgap0 = rowgap0;
            paramlist->get_int(2, colgap0);
            //static_cast<Fl_Grid*>(fl_widget)->gap(rowgap, colgap);
            dolayout();
            return;
        }
        throw "GAP command with no value(s) on layout '" + *widget_name();
    }

    if (method == "MARGIN") {
        int s;
        if (paramlist->get_int(1, s)) {
            margin0 = s;
            //static_cast<Fl_Grid*>(fl_widget)->margin(s, s, s, s);
            dolayout();
            return;
        }
        throw "MARGIN command with no value on layout '" + *widget_name();
    }

    Widget::handle_method(method, paramlist);
}
