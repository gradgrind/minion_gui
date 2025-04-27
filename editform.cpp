#include "editform.h"
#include "callback.h"
#include "textline.h"
#include "widgetdata.h"
#include "widget_methods.h"
#include "widgets.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Grid.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <string>
#include <FL/fl_draw.H>
#include <iostream>
using namespace std;
using namespace minion;

//TODO: How to do the callbacks? Entry name + current value?
//TODO: How to write to the entries?
//  Entry_name + new_value (ENTRY)
//  Entry_name + [new_value ...] (LIST)

void editform_method(
    Fl_Widget* w, string_view c, MinionList m)
{
    //auto efw = static_cast<EditForm*>(w);
    widget_method(w, c, m);
}

EditForm::EditForm()
    : Fl_Grid(0, 0, 0, 0)
{
    Fl_Group::current(0); // disable "auto-grouping"
    box(FL_BORDER_FRAME);
    gap(10, 5);
    margin(5, 5, 5, 5);
}

Fl_Widget* NEW_EditForm(
    MinionMap param)
{
    auto itemlist = param.get("ITEMS");
    if (holds_alternative<minion::MinionList>(itemlist)) {
        auto do_list = get<minion::MinionList>(itemlist);
        int n_entries = do_list.size();
        if (n_entries != 0) {
            auto efw = new EditForm();
            auto entry_bg{ENTRY_BG};
            string clr{};
            if (param.get_string("ENTRY_BG", clr))
                entry_bg = get_colour(clr);
            efw->layout(n_entries, 2);
            efw->col_weight(0, 0);
        
            Fl_Widget* e1;
            method_handler h;
            int label_width{0};
            for (int n_entry = 0; n_entry < n_entries; ++n_entry) {
                const auto& cmd = do_list.at(n_entry);
                const auto m = get<minion::MinionList>(cmd);
                const auto c = get<string>(m.at(0));
                string name{};
                string label{};
                if (m.size() > 2) {
                    name = get<string>(m.at(1));
                    label = get<string>(m.at(2));
                }
                bool measure_label{false};
                if (c == "SEPARATOR") {
                    e1 = new Fl_Box(FL_BORDER_FRAME, 0, 0, 0, 1, "");
                    h = widget_method;
                    efw->add(e1);
                    efw->widget(e1, n_entry, 0, 1, 2);
                    efw->row_weight(n_entry, 0);

                } else if (c == "TEXT") {
                    measure_label = true;
                    e1 = new TextLine(efw->entry_height);
                    h = input_method;
                    efw->add(e1);
                    efw->widget(e1, n_entry, 1);
                    efw->row_weight(n_entry, 0);
                    e1->copy_label(label.c_str());
                    e1->align(FL_ALIGN_LEFT);
                    e1->color(entry_bg);
                    
                } else if (c == "EDITOR") {
                    measure_label = true;
                    e1 = new Fl_Output(0, 0, 0, efw->entry_height);
                    h = input_method;
                    efw->add(e1);
                    efw->widget(e1, n_entry, 1);
                    efw->row_weight(n_entry, 0);
                    e1->copy_label(label.c_str());
                    e1->align(FL_ALIGN_LEFT);
                    e1->color(entry_bg);
                    e1->clear_visible_focus();
                    e1->callback(
                        [](Fl_Widget* w, void* ud) {
                            string dw{WidgetData::get_widget_name(w)};
                            // or string dw{static_cast<WidgetData*>(ud)->get_widget_name(w)};
                            auto res = Callback1(dw, static_cast<Fl_Output*>(w)->value());
                            cout << "CALLBACK RETURNED: " << dump_map_items(res, -1) << endl;
                        });
        
                } else if (c == "CHOICE") {
                    measure_label = true;
                    e1 = NEW_Choice({});
                    e1->size(0, efw->entry_height);
                    h = choice_method;
                    efw->add(e1);
                    efw->widget(e1, n_entry, 1);
                    efw->row_weight(n_entry, 0);
                    e1->copy_label(label.c_str());
                    e1->align(FL_ALIGN_LEFT);
                    e1->color(entry_bg);
                    e1->clear_visible_focus();

                } else if (c == "CHECKBOX") {
                    e1 = new Fl_Round_Button(0, 0, 0, efw->entry_height);
                    h = widget_method; //???
                    efw->add(e1);
                    efw->widget(e1, n_entry, 0, 1, 2);
                    efw->row_weight(n_entry, 0);
                    e1->copy_label(label.c_str());
                    e1->color(entry_bg);
                    e1->clear_visible_focus();

                } else if (c == "LIST") {
                    auto e0 = new Fl_Select_Browser(0, 0, 0, 0);
                    Fl_Group::current(0); // disable "auto-grouping"
                    e1 = e0;
                    h = list_method;
                    efw->add(e1);
                    e1->copy_label(label.c_str());
                    e1->align(FL_ALIGN_TOP_LEFT);
                    e1->color(entry_bg);
                    e1->clear_visible_focus();
            
                    //TODO
                    // Clicking somewhere withour an entry seems to select -1!
                    e1->callback([](Fl_Widget* w, void* ud) {
                        cout << "Chosen: " << static_cast<Fl_Select_Browser*>(w)->value() - 1 << endl;
                    });
            
                    //TODO--
                    e0->add("First item");
                    e0->add("Second item");
                    e0->add("Third item");
            
                    //e1->vertical_label_margin(5);

                    efw->widget(e1, n_entry, 0, 1, 2);
                    auto padabove = efw->list_title_space;           
                    if (padabove != 0) {
                        if (n_entry == 0) {
                            efw->margin(-1, padabove);
                        } else {
                            efw->row_gap(n_entry - 1, padabove);
                        }
                    }
            
                } else {
                    //TODO
                    throw string{"Invalid EditForm item: "} + c;
                }

                if (measure_label) {
                    int wl{0}, hl;
                    e1->measure_label(wl, hl);
                    if (wl > label_width) {
                        label_width = wl;
                    }
                }
        
                if (!name.empty()) {
                    // Add a WidgetData as "user data" to the widget
                    WidgetData::add_widget(name, e1, h);
                }
            }
            efw->col_gap(0, label_width + 15);
            return efw;
        }
    }
    string efname;
    param.get_string("NAME", efname);
    throw string{"EditForm missing ITEMS list: "} + efname;
}
