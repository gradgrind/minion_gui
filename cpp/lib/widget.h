#ifndef WIDGET_H
#define WIDGET_H

#include "minion.h"
#include <FL/Fl_Widget.H>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

//TODO?
// These are default values. The values used by the widgets are taken from
// static variables in `Widget`, which can be changed before creating
// widgets to make global changes.
const int LINE_HEIGHT = 30;
const Fl_Color NORMAL_FG = 0x40404000;
const Fl_Color NORMAL_BG = 0xf0f0f000;
const Fl_Color ENTRY_BG = 0xffffc800;
const Fl_Color PENDING_BG = 0xffe0e000;
const Fl_Color SELECTION_BG = 0x4c64ff00;

/// Each widget needs additional data, including its name. To make the
// widget accessible to the text-based (MINION) interface, a map is
// built from the widget-names to their management data, which would
// also include a pointer to the FLTK widget itself (as Fl_Widget*, which
// can then be cast to the appropriate sub-class).
// This management data is based on the Widget class, which is a sub-class
// of Fl_Callback_User_Data so that it can be saved as the user-data for
// the actual FLTK widget. Thus the data is also accessible from the FLTK
// widget (which is sometimes necessary for callbacks, etc.).
class Widget : public Fl_Callback_User_Data
{
    struct flagged_widget
    {
        Widget* widget;
        bool floating;
    };

    static std::unordered_map<std::string_view, flagged_widget> widget_map;

    // Widget name, used for look-up, etc.
    std::string w_name;

    // Widget type, which can be used to access a type's member
    // functions, also the name of the type.
    //??? int wtype;

    std::shared_ptr<minion::MMap> properties;
    
    // Substitute for Fl_Widget's user_data
    void *user_data = nullptr;
    bool auto_delete_user_data = false;

protected:
    Fl_Widget* fl_widget;

    Widget() = default;

public:
    ~Widget() override;

    static std::string clear();
    static void new_widget(minion::MList* m);
    static Widget* get_widget(std::string_view name);
    static Fl_Widget* get_fltk_widget(
        std::string_view name)
    {
        return get_widget(name)->fl_widget;
    }
    //TODO? static minion::MList list_widgets();
    static std::string* get_widget_name(
        Fl_Widget* w)
    { //TODO: is this needed?
        auto wd{static_cast<Widget*>(w->user_data())};
        return &wd->w_name;
    }

    inline static int line_height;
    inline static Fl_Color normal_fg;
    inline static Fl_Color normal_bg;
    inline static Fl_Color entry_bg;
    inline static Fl_Color pending_bg;
    inline static Fl_Color selection_bg;

    static void init_settings();
    static void init_foreground(Fl_Color colour);
    static void init_background(Fl_Color colour);
    static void init_background2(Fl_Color colour);
    static void init_selection_background(Fl_Color colour);
    static void init_pending_background(Fl_Color colour);

    Fl_Widget* fltk_widget() { return fl_widget; }

    virtual void handle_method(std::string_view method, minion::MList* mlist);

    void handle_methods(minion::MList* m, size_t start); // handle the calls in a command list

    //TODO: Should this be static? Do I need this at all? What exactly
    // should it do?
    void remove_widget(std::string_view name);

    inline std::string* widget_name() { return &w_name; }
    //int widget_type();
    //std::string_view widget_type_name();

    // properties access
    bool property_int(std::string_view key, int& result)
    {
        if (properties)
            return properties->get_int(key, result);
        return false;
    }
    bool property_string(std::string_view key, std::string& result)
    {
        if (properties)
            return properties->get_string(key, result);
        return false;
    }
};

using new_function = std::function<Widget* (minion::MMap*)>;
extern const std::map<std::string, new_function> new_function_map;

#endif // WIDGET_H
