#ifndef WIDGETS_H
#define WIDGETS_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Widget.H>

// *** non-layout widgets

class W_Box : public Widget
{
public:
    static W_Box* make(minion::MMap* parammap);
};

class W_Hline : public W_Box
{
public:
    static W_Hline* make(minion::MMap* parammap);
};

class W_Vline : public W_Box
{
public:
    static W_Vline* make(minion::MMap* parammap);
};

class W_Label : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Label* make(minion::MMap* parammap);
};

class W_PushButton : public W_Label
{
public:
    static W_PushButton* make(minion::MMap* parammap);
};

class W_Checkbox : public Widget
{
public:
    static W_Checkbox* make(minion::MMap* parammap);
};

class W_Choice : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Choice* make(minion::MMap* parammap);
};

class W_Output : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Output* make(minion::MMap* parammap);
};

class W_PopupEditor : public W_Output
{
public:
    //void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_PopupEditor* make(minion::MMap* parammap);
};

class W_List : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_List* make(minion::MMap* parammap);
};

class W_TextLine : public Widget
{
public:
    //? void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_TextLine* make(minion::MMap* parammap);

    bool set(std::string_view newtext);
};

class W_RowTable : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_RowTable* make(minion::MMap* parammap);
};

class W_EditForm : public Widget
{
public:
    static W_EditForm* make(minion::MMap* parammap);
};

#endif // WIDGETS_H
