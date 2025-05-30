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

class W_Input : public Widget
{
public:
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Input* make(minion::MMap* parammap);
};

class W_Output : public W_Input
{
public:
    // handle_method inherited from W_Input?
    static W_Output* make(minion::MMap* parammap);
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
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_TextLine* make(minion::MMap* parammap);

    bool set(std::string_view newtext);
};

class W_RowTable : public Widget
{
public:
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_RowTable* make(minion::MMap* parammap);
};

class W_EditForm : public Widget
{
public:
    static W_EditForm* make(minion::MMap* parammap);
};

#endif // WIDGETS_H
