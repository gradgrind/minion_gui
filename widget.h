#ifndef WIDGET_H
#define WIDGET_H

#include "minion.h"
#include <FL/Fl_Widget.H>
#include <string>
#include <unordered_map>

// Each widget needs additional data, including its name. To make the
// widget accessible to the text-based (MINION) interface, a map is
// built from the widget-names to their management data, which would
// also include a pointer to the widget itself (as Fl_Widget*, which
// can then be cast to the appropriate sub-class).
// This management data is based on the Widget class, which is a sub-class
// of Fl_Callback_User_Data so that it can be saved as the user-data for
// the actual FLTK widget. Thus the data is also accessible from the FLTK
// widget (which is sometimes necessary for callbacks, etc.).
class Widget : public Fl_Callback_User_Data
{
    static std::unordered_map<std::string_view, Fl_Widget *> widget_map;

    // Widget name, used for look-up, etc.
    std::string w_name;

    // Widget type, which can be used to access a type's member
    // functions, also the name of the type.
    //??? int wtype;
    
    // Substitute for Fl_Widget's user_data
    void *user_data = nullptr;
    bool auto_delete_user_data = false;

public:
    //Widget(minion::MinionMap parammap, std::string_view w_name);
    Widget(minion::MinionMap parammap);
    ~Widget() override;

    //static void add_widget(std::string_view name, Fl_Widget *w, method_handler h);
    static Fl_Widget* get_widget(std::string_view name);
    static Widget* get_widget_data(std::string_view name);
    static minion::MinionList list_widgets();
    static std::string_view get_widget_name(Fl_Widget *w);

    //inline static int line_height{LINE_HEIGHT};
    //inline static Fl_Color entry_bg{ENTRY_BG};
    //inline static Fl_Color pending_bg{PENDING_BG};
    
    Fl_Widget* fltk_widget();

    void widget_method(std::string_view method, minion::MinionList paramlist);

    virtual void handle_method(minion::MinionList);

    void remove_widget(std::string_view name);

    std::string_view widget_name();
    //int widget_type();
    //std::string_view widget_type_name();
};


#endif // WIDGET_H