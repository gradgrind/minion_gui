#include "dispatcher.h"
#include "functions.h"
#include "layout.h"
#include "minion_gui.h"
#include "textline.h"
#include "widget.h"
#include "widgets.h"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <functional>
#include <map>
#include <string_view>
using namespace std;
using namespace minion;

void Handle_methods(
    Widget* w, MMap* m)
{
    auto dolist0 = m->get("DO");
    if (!dolist0.is_null()) {
        if (auto dolist = dolist0.m_list()) {
            auto len = dolist->size();
            for (int i = 0; i < len; ++i) {
                MList* mlist = dolist->get(i).m_list();
                string_view c = mlist->get(0).m_string()->data_view();
                w->handle_method(c, mlist);
            }
            return;
        }
    }
    MValue m0{m};
    throw string{"Invalid DO list: "} + dump_buffer.dump(m0, 0);
}

using new_function = function<Widget* (MMap*)>;

const map<string, new_function> new_function_map{
    {"Window", W_Window::make},
    {"Grid", W_Grid::make},
    {"Row", W_Row::make},
    {"Column", W_Column::make},
    {"PushButton", W_PushButton::make},
    {"Box", W_Box::make},
    {"Label", W_Label::make},
    {"Choice", W_Choice::make},
    {"Output", W_Output::make},
    {"Checkbox", W_Checkbox::make},
    {"TextLine", W_TextLine::make},
    {"RowTable", W_RowTable::make},
    {"EditForm", W_EditForm::make}
};

void Handle_NEW(
    string_view wtype, MMap* m)
{
    string_view name;
    { // Check new widget name
        MString* name0;
        auto n = m->get("NAME");
        if (n.is_null() || !(name0 = n.m_string())) {
            MValue m0{m};
            throw string{"Bad NEW command: "} + dump_buffer.dump(m0, 0);
        }
        // Check name unique
        name = name0->data_view();
        Widget::check_new_widget_name(name);
    }
    new_function f;
    try {
        f = new_function_map.at(string{wtype});
    } catch (std::out_of_range) {
        throw string{"Unknown widget type: "}.append(wtype);
    }
    // Create widget
    Widget* w = f(m);
    // Add to parent, if specified
    string parent;
    if (m->get_string("PARENT", parent) && !parent.empty()) {
        auto p = static_cast<Fl_Group*>(Widget::get_fltk_widget(parent));
        p->add(w->fltk_widget());
    }
     // Handle method calls supplied with the widget creation
    Handle_methods(w, m);
}

void GUI(
    MMap* mmap)
{
    string s;
    if (mmap->get_string("NEW", s)) {
        // Make a new widget
        Handle_NEW(s, mmap);
    } else if (mmap->get_string("WIDGET", s)) {
        // Handle widget methods
        auto w = Widget::get_widget(s);
        Handle_methods(w, mmap);
    } else if (mmap->get_string("FUNCTION", s)) {
        // Some other function
        auto f = function_map.at(s);
        f(mmap);
    } else {
        // Error
        MValue m = mmap;
        dump_buffer.dump(m, 0);
        throw string{"Invalid GUI parameters: "} + dump_buffer.dump(m, 0);
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
MMap* message(
    MMap* data)
{
    return data;
}

void to_back_end(
    MMap* data)
{
    MMap* result = message(data);
    auto dolist0 = data->get("DO");
    if (MList* mlist = dolist0.m_list()) {
        size_t n = mlist->size();
        for (size_t i = 0; i < n; ++i) {
            MValue cmd = mlist->get(i);
            if (MMap* mmap = cmd.m_map())
                GUI(mmap);
        }
    }
    // Any back-end function which can take more than about 100ms should
    // initiate a timeout leading to a modal "progress" dialog.
    // Any data generated while such a callback is operating (i.e. before
    // it returns a completion code) should be fetched and run by an idle
    // handler. Any data generated outside of this period is probably an
    // error – the back-end should not be doing anything then!
}
