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

void W_EditForm::handle_child_size(
    Fl_Widget* wc, int ww, int wh)
{
    auto gw = static_cast<Fl_Grid*>(fl_widget);
    gw->cell(wc)->minimum_size(ww, wh);
    gw->layout();
    get_grid_sizes(gw, children.size(), 2);
}

/*    
void W_EditForm::handle_child_modified(
    Widget* wc)
{
    throw "TODO: W_EditForm::handle_child_modified";
}
*/

void W_EditForm::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "ADD") {
        auto fw = static_cast<Fl_Grid*>(fl_widget);
        // Label alignment
        Fl_Align align{FL_ALIGN_LEFT | FL_ALIGN_INSIDE};
        {
            string align_;
            property_string("LABEL_ALIGN", align_);
            if (align_ == "CENTRE") {
                align = FL_ALIGN_CENTER;
            } else if (align_ == "RIGHT") {
                align = FL_ALIGN_RIGHT | FL_ALIGN_INSIDE;
            }
        }

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
                ++nc;

                int span = 0;
                wc->property_int("SPAN", span);
                string label;
                wc->property_string("LABEL", label);

                Fl_Widget* wlabel = nullptr;

                if (span == 0) {
                    fw->add(fwc);
                    // If there is a label, make a labelled box for the
                    // first column.
                    if (!label.empty()) {
                        wlabel = new Fl_Box(0, 0, 0, 0);
                        wlabel->copy_label(label.c_str());
                        wlabel->align(align);
                        fw->add(wlabel);
                    }

                } else {
                    // If there is a label, it will need to come first. Bundle
                    // label and widget together in a flex layout.
                    if (label.empty()) {
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
                    }*/
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
}
