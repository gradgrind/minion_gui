#include "widgets.h"
#include "callback.h"
#include "support_functions.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Grid.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Select_Browser.H>
using namespace std;
using namespace minion;

// *** Non-layout widgets ***

//static
W_Box* W_Box::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Box(0, 0, 0, 0);
    auto widget = new W_Box();
    widget->fl_widget = w;
    return widget;
}

//static
W_Hline* W_Hline::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Box(FL_BORDER_FRAME, 0, 0, 0, 1, "");
    auto widget = new W_Hline();
    widget->fl_widget = w;
    return widget;
}

//static
W_Vline* W_Vline::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Box(FL_BORDER_FRAME, 0, 0, 1, 0, "");
    auto widget = new W_Vline();
    widget->fl_widget = w;
    return widget;
}

//static
W_Label* W_Label::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Box(0, 0, 0, 0);
    auto widget = new W_Label();
    widget->fl_widget = w;
    return widget;
}

void W_Label::handle_method(
    string_view method, MList* paramlist)
{
    string label;
    if (method == "TEXT") {
        if (paramlist->get_string(1, label)) {
            fl_widget->copy_label(label.c_str());
            int lw{0}, lh;
            fl_widget->measure_label(lw, lh);
            //w->horizontal_label_margin(5);
            fl_widget->size(lw + 20, Widget::line_height);
            string align;
            property_string("LABEL_ALIGN", align);
            if (align == "LEFT")
                fl_widget->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
            else if (align == "RIGHT")
                fl_widget->align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
            if (auto g = dynamic_cast<Fl_Grid*>(fl_widget->parent())) {
                g->layout();
            }
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}

//static
W_Choice* W_Choice::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Choice(0, 0, 0, Widget::line_height);
    w->callback([](Fl_Widget* w, void* ud) {
        (void) ud;
        string* dw{Widget::get_widget_name(w)};
        auto ww = static_cast<Fl_Choice*>(w);
        Callback2(*dw, to_string(ww->value()), ww->text());
    });
    auto widget = new W_Choice();
    widget->fl_widget = w;
    return widget;
}

void W_Choice::handle_method(
    string_view method, MList* paramlist)
{
    string item;
    if (method == "ADD") {
        int n = paramlist->size();
        for (int i = 1; i < n; ++i) {
            paramlist->get_string(i, item);
            static_cast<Fl_Choice*>(fl_widget)->add(item.c_str());
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}

//static
W_Output* W_Output::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Output(0, 0, 0, Widget::line_height);
    auto widget = new W_Output();
    widget->fl_widget = w;
    w->selection_color(Widget::selection_bg);
    return widget;
}

void W_Output::handle_method(
    string_view method, MList* paramlist)
{
    string item;
    if (method == "VALUE") {
        string val;
        if (paramlist->get_string(1, val)) {
            static_cast<Fl_Output*>(fl_widget)->value(val.c_str());
        } else
            throw "Output widget VALUE method: value missing";
    } else {
        Widget::handle_method(method, paramlist);
    }
}

// static
W_PopupEditor* W_PopupEditor::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Output(0, 0, 0, Widget::line_height);
    auto widget = new W_PopupEditor();
    widget->fl_widget = w;
    w->color(Widget::entry_bg);
    w->callback([](Fl_Widget* w, void* ud) {
        (void) ud;
        string* dw{Widget::get_widget_name(w)};
        // or string dw{static_cast<Widget*>(ud)->widget_name()};
        Callback1(*dw, static_cast<Fl_Output*>(w)->value());
    });
    return widget;
}

//static
W_PushButton* W_PushButton::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Button(0, 0, 0, 0);
    auto widget = new W_PushButton();
    widget->fl_widget = w;
    // "selection" (pressed) colour
    auto cx = fl_contrast(0, Widget::entry_bg);
    auto cp = fl_color_average(Widget::entry_bg, cx, 0.9);
    w->color(Widget::entry_bg, cp);
    w->callback([](Fl_Widget* w, void* ud) {
        (void) ud;
        string* dw{Widget::get_widget_name(w)};
        // or string dw{static_cast<Widget*>(ud)->widget_name()};
        //auto ww = static_cast<Fl_Button*>(w);
        Callback0(*dw);
    });
    return widget;
}

void W_PushButton::handle_method(
    string_view method, MList* paramlist)
{
    string label;
    if (method == "COLOUR") {
        string clr;
        if (paramlist->get_string(1, clr)) {
            auto c = get_colour(clr);
            auto cx = fl_contrast(0, c);
            auto cp = fl_color_average(c, cx, 0.9);
            fl_widget->color(c, cp);
        }
    } else {
        W_Label::handle_method(method, paramlist);
    }
}

//static
W_Checkbox* W_Checkbox::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Round_Button(0, 0, 0, Widget::line_height);
    auto widget = new W_Checkbox();
    widget->fl_widget = w;
    w->callback([](Fl_Widget* w, void* ud) {
        (void) ud;
        string* dw{Widget::get_widget_name(w)};
        // or string dw{static_cast<Widget*>(ud)->widget_name()};
        auto ww = static_cast<Fl_Round_Button*>(w);
        string val{};
        if (ww->value() != 0)
            val = "1";
        Callback1(*dw, val);
    });
    return widget;
}

//static
W_List* W_List::make(
    MMap* props)
{
    (void) props;
    auto w = new Fl_Select_Browser(0, 0, 0, 0);
    auto widget = new W_List();
    widget->fl_widget = w;
    w->color(Widget::entry_bg);
    w->callback([](Fl_Widget* w, void* ud) {
        (void) ud;
        string* dw{Widget::get_widget_name(w)};
        // or string dw{static_cast<Widget*>(ud)->widget_name()};
        auto ww = static_cast<Fl_Select_Browser*>(w);
        auto i = ww->value();
        // Callback only for actual items (1-based indexing)
        if (i > 0) {
            Callback2(*dw, to_string(i - 1), ww->text(i));
        }
    });
    return widget;
}

void W_List::handle_method(
    string_view method, MList* paramlist)
{
    string item;
    if (method == "SET") {
        auto e1 = static_cast<Fl_Select_Browser*>(fl_widget);
        e1->clear();
        int n = paramlist->size();
        for (int i = 1; i < n; ++i) {
            //TODO? add can have a second argument (void *), which can
            // refer to data ...
            paramlist->get_string(i, item);
            e1->add(item.c_str());
        }
    } else {
        Widget::handle_method(method, paramlist);
    }
}
