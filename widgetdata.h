#ifndef WIDGETDATA_H
#define WIDGETDATA_H

#include "minion.h"
#include <FL/Fl_Widget.H>
#include <functional>
#include <string>
#include <unordered_map>

const Fl_Color ENTRY_BG = 0xffffc800;
const Fl_Color PENDING_BG = 0xffe0e000;

using method_handler = std::function<void(Fl_Widget *, std::string_view, minion::MinionList)>;

void widget_method(Fl_Widget *w, std::string_view c, minion::MinionList m);

class WidgetData : public Fl_Callback_User_Data
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

    WidgetData(std::string_view w_name, method_handler h);

public:
    ~WidgetData() override;

    static void add_widget(std::string_view name, Fl_Widget *w, method_handler h);
    static Fl_Widget *get_widget(std::string_view name);
    static minion::MinionList list_widgets();
    static std::string_view get_widget_name(Fl_Widget *w);

    method_handler handle_method;

    void remove_widget(std::string_view name);

    std::string_view widget_name();
    //int widget_type();
    //std::string_view widget_type_name();
};

#endif // WIDGETDATA_H