#include "editform.h"
#include "callback.h"
#include "widgets.h"
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <string>
#include <FL/fl_draw.H>
using namespace std;
using namespace minion;

//TODO: How to do the callbacks? Entry name + current value?
//TODO: How to write to the entries?
//  Entry_name + new_value (ENTRY)
//  Entry_name + [new_value ...] (LIST)

EditForm::EditForm()
    : Fl_Grid(0, 0, 0, 0)
{
    Fl_Group::current(0); // disable "auto-grouping"
    box(FL_BORDER_FRAME);
    gap(10, 5);
    margin(5, 5, 5, 5);
}

// static
W_EditForm* W_EditForm::make(minion::MMap* parammap)
{
    auto wlist0 = parammap->get("WIDGETS");
    if (auto wlist = wlist0.m_list()) {
        auto mlist = wlist->get();
        auto n = mlist->size();
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
            // Now create the EditForm widget
            auto efw = new EditForm();
            efw->layout(n, 2);
            efw->col_weight(0, 0);
            auto widget = new W_EditForm();
            widget->fl_widget = efw;
            efw->color(Widget::entry_bg);

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
                        auto wl = new Fl_Box (0, 0, 0, 0);
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
                        auto wl = new Fl_Box (0, 0, 0, 0);
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
