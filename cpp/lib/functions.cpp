#include "functions.h"
#include "callback.h"
#include "support_functions.h"
#include "widget.h"
#include <FL/Fl.H>
//#include <iostream>
using namespace std;

void f_RUN(
    minion::MMap* m)
{
    (void) m;
    //auto cc = Fl::run();
    //std::cout << "Main Loop ended: " << cc << std::endl;
    Fl::run();
}

void setup_method(
    string_view method, minion::MList* paramlist)
{
    if (method == "BACKGROUND") {
        std::string colour;
        if (paramlist->get_string(1, colour)) {
            Widget::init_background(get_colour(colour));
            return;
        }
        throw string{"Invalid SETUP method, colour expected: "} + dump_value(*paramlist);
    }
    if (method == "BACKGROUND2") {
        std::string colour;
        if (paramlist->get_string(1, colour)) {
            Widget::init_background2(get_colour(colour));
            return;
        }
        throw string{"Invalid SETUP method, colour expected: "} + dump_value(*paramlist);
    }
    if (method == "FOREGROUND") {
        std::string colour;
        if (paramlist->get_string(1, colour)) {
            Widget::init_foreground(get_colour(colour));
            return;
        }
        throw string{"Invalid SETUP method, colour expected: "} + dump_value(*paramlist);
    }
    throw string{"Invalid SETUP method: "} + dump_value(*paramlist);
    //TODO?
    //Fl::box_border_radius_max 	( 	int 	R	);
    //Fl::menu_linespacing 	( 	int 	H	);
    //LINE_HEIGHT?
    //PENDING_COLOUR?
}

void f_SETUP(
    minion::MMap* m)
{
    auto dolist0 = m->get("DO");
    if (!dolist0.is_null()) {
        if (auto dolist = dolist0.m_list()) {
            auto len = (*dolist)->size();
            for (size_t i = 0; i < len; ++i) {
                auto n = (*dolist)->get(i);
                auto mlist = n.m_list();
                if (mlist) {
                    string c;
                    if ((*mlist)->get_string(0, c)) {
                        setup_method(c, mlist->get());
                        continue;
                    }
                }
                throw string{"SETUP function, expected method (list): "} + dump_value(n);
            }
            return;
        }
    }
    minion::MValue m0{*m};
    throw string{"Invalid SETUP function: "} + dump_value(*m);
}

std::unordered_map<std::string, function_handler> function_map{
    //
    {"RUN", f_RUN},
    {"SETUP", f_SETUP}
    //
};
