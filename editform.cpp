#include "editform.h"
#include "backend.h"
#include "layout.h"
#include "minion.h"
#include "widget_methods.h"
#include "widgets.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Grid.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <string>
#include <vector>
#include <FL/fl_draw.H>
#include <iostream>
#include <ostream>
using namespace std;
using namespace minion;

//TODO: Do I really need the EditFormEntry struct as it is?
// How dynamic should the widget be? Should I be able to add entries
// later? Remove them?

// I could have the entries as a constructor parameter and use the single
// layout function if there is to be no change later.

//TODO: How to do the callbacks? Entry name + current value?
//TODO: How to write to the entries?
//  Entry_name + new_value (ENTRY)
//  Entry_name + [new_value ...] (LIST)

struct EditFormEntryData : public Fl_Callback_User_Data
{
    std::string w_name;

    EditFormEntryData(
        std::string_view n)
        : Fl_Callback_User_Data()
        , w_name{n}
    {}
};

void editform_method(
    Fl_Widget* w, string_view c, MinionList m)
{
    auto efw = static_cast<EditForm*>(w);
    int entry_n = efw->entries.size();

    /* TODO ...

    if (c == "ENTRY") {
        string name = get<string>(m.at(1));
        string label = get<string>(m.at(2));
        auto e1 = new Fl_Output(0, 0, 0, efw->entry_height);
        efw->entry_map.emplace(name, e1);
        efw->add(e1);
        e1->copy_label(label.c_str());
        e1->align(FL_ALIGN_LEFT);
        e1->color(efw->entry_bg);

        //TODO
        e1->callback(
            [](Fl_Widget* w, void* ud) {
                auto dw = static_cast<EditFormEntryData*>(ud)->w_name;
                cout << "Activated: " << dw << endl;
                auto res = backend("EditForm: " + dw);
                cout << "CALLBACK RETURNED: " << res << endl;
            },
            new EditFormEntryData(name),
            true);

        e1->clear_visible_focus(); // no cursor, but text cannot be copied
        e1->horizontal_label_margin(5);

        //??
        EditFormEntry efe{
            .widget = e1,
            .name = name,
        };
        efw->entries.emplace_back(efe);

        efw->layout(entry_n + 1, 2);
        efw->widget(e1, entry_n, 1);
        efw->row_weight(entry_n, 0);
        if (entry_n == 0)
            efw->col_weight(0, 0);

        int wl, hl;
        e1->measure_label(wl, hl);
        if (wl > efw->label_width) {
            efw->label_width = wl;
            efw->col_gap(0, efw->label_width + 15);
        }

    } else if (c == "SEPARATOR") {
        auto e1 = new Fl_Box(FL_BORDER_FRAME, 0, 0, 0, 1, "");
        efw->add(e1);
        efw->entries.emplace_back(EditFormEntry{.widget = e1, .spanning = true});
        efw->layout(entry_n + 1, 2);
        efw->widget(e1, entry_n, 0, 1, 2);
        if (entry_n == 0)
            efw->col_weight(0, 0);
        efw->row_weight(entry_n, 0);

    } else if (c == "LIST") {
        string name = get<string>(m.at(1));
        string label = get<string>(m.at(2));
        //TODO
        auto e1 = new Fl_Select_Browser(0, 0, 0, 0);
        e1->end();
        efw->entry_map.emplace(name, e1);
        efw->add(e1);
        e1->copy_label(label.c_str());
        e1->align(FL_ALIGN_TOP_LEFT);
        e1->color(efw->entry_bg);

        //TODO
        e1->callback([](Fl_Widget* w, void* a) {
            cout << "Chosen: " << static_cast<Fl_Select_Browser*>(w)->value() - 1 << endl;
        });

        //TODO--
        e1->add("First item");
        e1->add("Second item");
        e1->add("Third item");

        //e1->vertical_label_margin(5);

        auto efe = EditFormEntry{.widget = e1,
                                 .name = name,
                                 .padabove = efw->list_title_space,
                                 .spanning = true,
                                 .growable = true};

        efw->entries.emplace_back(efe);

        efw->layout(entry_n + 1, 2);
        efw->widget(e1, entry_n, 0, 1, 2);
        if (entry_n == 0)
            efw->col_weight(0, 0);
        if (efe.padabove != 0) {
            if (entry_n == 0) {
                efw->margin(-1, efe.padabove);
            } else {
                efw->row_gap(entry_n - 1, efe.padabove);
            }
        }

    //} else if (c == "SET") {
    if (c == "SET") {
        string name = get<string>(m.at(1));
        auto e0 = efw->entry_map.at(name);
        if (auto e1 = dynamic_cast<Fl_Output*>(e0)) {
            auto v = get<string>(m.at(2)).c_str();
            e1->value(v);
            // Setting the tooltip allows overlong texts to be readable
            e1->tooltip(v);
        } else if (auto e1 = dynamic_cast<Fl_Select_Browser*>(e0)) {
            e1->clear();
            for (int i = 2; i < m.size(); ++i) {
                //TODO? add can have a second argument (void *), which can
                // refer to data ...
                e1->add(get<string>(m.at(i)).c_str());
            }
        }

        //TODO?
    } else {
        widget_method(w, c, m);
    }
    */
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

Fl_Widget* container;
Fl_Widget* widget;
std::string name;
bool spanning;
bool growable;

/* Currently unused ...

void EditForm::add_separator()
{
    auto e1 = new Fl_Box(FL_BORDER_FRAME, 0, 0, 0, 1, "");
    entries.emplace_back(EditFormEntry{.widget = e1, .spanning = true});
}

void EditForm::add_list(
    const char* name, const char* label)
{
    auto e1 = new Fl_Select_Browser(0, 0, 0, 0, label);
    e1->end();
    e1->align(FL_ALIGN_TOP_LEFT);
    e1->color(fl_rgb_color(255, 255, 200));

    //TODO
    e1->callback([](Fl_Widget* w, void* a) {
        std::cout << "Chosen: " << ((Fl_Select_Browser*) w)->value()
                  << std::endl;
    });

    //TODO--
    e1->add("First item");
    e1->add("Second item");
    e1->add("Third item");

    //e1->vertical_label_margin(5);

    entries.emplace_back(EditFormEntry{.widget = e1,
                                       .name = name,
                                       .padabove = 30,
                                       .spanning = true,
                                       .growable = true});
}

void EditForm::do_layout()
{
    int labwidth = 0; // for measuring the max. label width
    int wl, hl;
    int n_entries = entries.size();
    layout(n_entries, 2);
    for (int i = 0; i < n_entries; ++i) {
        auto e = entries[i];
        add(e.widget);
        if (e.spanning) {
            widget(e.widget, i, 0, 1, 2);
        } else {
            widget(e.widget, i, 1);
        }
        if (!e.growable) {
            row_weight(i, 0);
        }
        if (e.padabove != 0) {
            if (i == 0) {
                margin(-1, e.padabove);
            } else {
                row_gap(i - 1, e.padabove);
            }
        }

        wl = 0;
        e.widget->measure_label(wl, hl);
        if (wl > labwidth)
            labwidth = wl;
    }
    col_weight(0, 0);
    col_gap(0, labwidth + 15);
}
*/

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

                    //TODO
                    // when(FL_WHEN_RELEASE) is too unpredictable?
                    // With FL_WHEN_ENTER_KEY it may not be clear whether
                    // ENTER has been pressed or not (the cursor is still
                    // there.)
                    // A refocus after RETURN can perhaps help? But if 
                    // when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY) is used, a
                    // return causes two callbacks ...
                    // Can I change colour on focus?
                    e1->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
                    //e1->when(FL_WHEN_ENTER_KEY);
                    //e1->when(FL_WHEN_ENTER_KEY_CHANGED );
                    // ... then change the when() on first call?
                    e1->callback(
                        [](Fl_Widget* w, void* ud) {

                            //w->color(0xffe0e0);
                            
                            auto dw = WidgetData::get_widget_name(w);
                            // or: auto dw = static_cast<WidgetData*>(ud)->get_widget_name(w);
                            
                            // set a flag in widget-data?

                            cout << "Activated: " << dw << ": " << Fl_Callback_Reason() << endl;
                            cout << "  ::: " << static_cast<Fl_Input*>(w)->value() << endl; 
                            //auto res = backend(string{"EditForm: "} + string{dw});
                            //cout << "CALLBACK RETURNED: " << res << endl;
                            Fl::focus(w->parent());
                        });

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

                    //TODO
                    e1->callback(
                        [](Fl_Widget* w, void* ud) {
                            auto dw = WidgetData::get_widget_name(w);
                            // or: auto dw = static_cast<WidgetData*>(ud)->get_widget_name(w);
                            cout << "Activated: " << dw << endl;
                            auto res = backend(string{"EditForm: "} + string{dw});
                            cout << "CALLBACK RETURNED: " << res << endl;
                        });

                } else if (c == "CHOICE") {
                    measure_label = true;
                    e1 = new Fl_Choice(0, 0, 0, efw->entry_height);
                    h = input_method;
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
