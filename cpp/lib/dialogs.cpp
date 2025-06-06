#include "dialogs.h"
#include <FL/Fl_Double_Window.H>

W_Dialog* W_Dialog::make(
    minion::MMap* parammap)
{
    int ww = 300;
    int wh = 200;
    parammap->get_int("WIDTH", ww);
    parammap->get_int("HEIGHT", wh);
    auto w = new Fl_Double_Window(ww, wh);
    w->set_modal();
    //w->callback(callback_close_window);
    Fl_Group::current(0); // disable "auto-grouping"
    auto widget = new W_Dialog();
    widget->fl_widget = w;
    parammap->get_int("MIN_WIDTH", ww);
    parammap->get_int("MIN_HEIGHT", wh);
    w->size_range(ww, wh);
    return widget;
}

void W_Dialog::handle_method(
    std::string_view method, minion::MList* paramlist)
{
    if (method == "SHOW") {
        std::string p;
        if (paramlist->get_string(1, p)) {
            //TODO checks ...
            auto w = get_fltk_widget(p);

            fl_widget->position(w->x(), w->y());
        }
        fl_widget->show();
    } else {
        W_Window::handle_method(method, paramlist);
    }
}
