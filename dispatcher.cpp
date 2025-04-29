#include "dispatcher.h"
#include "functions.h"
#include "layout.h"
#include "textline.h"
#include "widgetdata.h"
#include "widgets.h"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <fmt/format.h>
using namespace std;
using namespace minion;

void Handle_methods(
    Fl_Widget* w, MinionMap m, method_handler h)
{
    auto dolist = m.get("DO");
    if (holds_alternative<MinionList>(dolist)) {
        MinionList do_list = get<MinionList>(dolist);
        for (const auto& cmd : do_list) {
            MinionList m = get<MinionList>(cmd);
            string_view c = get<string>(m.at(0));
            h(w, c, m);
        }
    } else if (dolist.index() != 0) {
        string s;
        minion::dump(s, dolist, 0);
        throw string{"Invalid DO list: "} + s;
    }
}

void Handle_NEW(
    string_view wtype, MinionMap m)
{
    //cout << "Handle_NEW " << wtype << ":" << minion::dump_map_items(m, 1) << endl;
    string name;
    Fl_Widget* w;
    method_handler h;
    if (m.get_string("NAME", name)) {
        if (wtype == "Window") {
            w = NEW_Window(m);
            h = group_method;
        } else if (wtype == "Grid") {
            w = NEW_Grid(m);
            h = grid_method;
        } else if (wtype == "Row") {
            w = NEW_Row(m);
            h = grid_method;
        } else if (wtype == "Column") {
            w = NEW_Column(m);
            h = grid_method;
            // *** End of layouts, start of other widgets
        } else if (wtype == "PushButton") {
            w = NEW_PushButton(m);
            h = widget_method;
        } else if (wtype == "Box") {
            w = NEW_Box(m);
            h = widget_method;
        } else if (wtype == "Label") {
            w = NEW_Label(m);
            h = widget_method;
        } else if (wtype == "Choice") {
            w = NEW_Choice(m);
            h = choice_method;
        } else if (wtype == "Output") {
            w = NEW_Output(m);
            h = input_method;
        } else if (wtype == "Checkbox") {
            w = NEW_Checkbox(m);
            h = widget_method; //TODO: button_method?
        } else if (wtype == "TextLine") {
            w = NEW_TextLine(m);
            h = input_method;
        } else if (wtype == "RowTable") {
            w = NEW_RowTable(m);
            h = rowtable_method;
        } else if (wtype == "EditForm") {
            w = NEW_EditForm(m);
            h = editform_method;
        } else {
            throw fmt::format("Unknown widget type: {}", wtype);
        }
        string parent;
        if (m.get_string("PARENT", parent) && !parent.empty()) {
            
            auto p = static_cast<Fl_Group*>(WidgetData::get_widget(parent));
            p->add(w);

            // Handle fixed sizes of Flex components
            if (p->type() == Fl_Flex::VERTICAL) {
                int wh = w->h();
                if (wh != 0) {
                    static_cast<Fl_Flex*>(p)->fixed(w, wh);
                }
            } else if (p->type() == Fl_Flex::HORIZONTAL) {
                int wl = w->w();
                if (wl != 0) {
                    static_cast<Fl_Flex*>(p)->fixed(w, wl);
                }
            }
        }
        // Add a WidgetData as "user data" to the widget
        WidgetData::add_widget(name, w, h);
        // Handle methods
        Handle_methods(w, m, h);
        return;
    }
    throw fmt::format("Bad NEW command: {}", minion::dump_map_items(m, -1));
}

void GUI(
    MinionMap obj)
{
    string w;
    if (obj.get_string("NEW", w)) {
        Handle_NEW(w, obj);
    } else if (obj.get_string("WIDGET", w)) {
        // Handle methods
        auto widg = WidgetData::get_widget(w);
        auto wd{static_cast<WidgetData*>(widg->user_data())};
        Handle_methods(widg, obj, wd->handle_method);
    } else if (obj.get_string("FUNCTION", w)) {
        auto f = function_map.at(w);
        f(obj);
    } else {
        throw fmt::format("Invalid GUI parameters: {}", dump_map_items(obj, -1));
    }
}

// Pass a message to the back-end. This can be an event/callback, the
// reponse to a query, or whatever.

// There need to be two kinds of "message":
// 1) Let's call this a virtual override. It is a call to the back-end,
//    perhaps with a result (like 0 or 1 for event handlers), and is
//    blocking – so it should execute quickly. Unfortunately this seems
//    very difficult to implement, because it might also need to query
//    the front-end or perform other gui operations. Thus it entails
//    a calling back and forth between back-end and front-end.
// 2) Let's call this a trigger. It sets an operation in the back-end
//    going, but doesn't wait for it to finish. Any resulting calls to
//    the front-end could be picked up by an idle function.
// For the moment I would like to implement just normal callbacks, i.e.
// asynchronous calls. Where event handlers are necessary, I would first
// consider extending the C++ widgets.

//TODO
MinionMap message(
    MinionMap data)
{
    return data;
}

void to_back_end(
    MinionMap data)
{
    MinionMap result = message(data);
    auto dolist0 = data.get("DO");
    if (holds_alternative<MinionList>(dolist0)) {
        auto dolist = get<MinionList>(dolist0);
        for (const auto& cmd : dolist) {
            GUI(get<MinionMap>(cmd));
        }
    }
    // Any back-end function which can take more than about 100ms should
    // initiate a timeout leading to a modal "progress" dialog.
    // Any data generated while such a callback is operating (i.e. before
    // it returns a completion code) should be fetched and run by an idle
    // handler. Any data generated outside of this period is probably an
    // error – the back-end should not be doing anything then!
}
