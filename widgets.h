#ifndef WIDGETS_H
#define WIDGETS_H

#include "minion.h"
#include "widget.h"
#include <FL/Fl_Widget.H>

// *** non-layout widgets

class W_Box : public Widget
{
public:
    W_Box(minion::MMap* parammap) : Widget{parammap}
    {}

    static W_Box* make(minion::MMap* parammap);
};

class W_Label : public Widget
{
public:
    W_Label(minion::MMap* parammap) : Widget{parammap}
    {}

    static W_Label* make(minion::MMap* parammap);
};

class W_PushButton : public Widget
{
public:
    W_PushButton(minion::MMap* parammap) : Widget{parammap}
    {}

    static W_PushButton* make(minion::MMap* parammap);
};

class W_Checkbox : public Widget
{
public:
    W_Checkbox(minion::MMap* parammap) : Widget{parammap}
    {}

    static W_Checkbox* make(minion::MMap* parammap);
};

class W_Choice : public Widget
{
public:
    W_Choice(minion::MMap* parammap) : Widget{parammap} {}
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Choice* make(minion::MMap* parammap);
};

class W_Input : public Widget
{
public:
    W_Input(minion::MMap* parammap) : Widget{parammap} {}
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_Input* make(minion::MMap* parammap);
};

class W_Output : public W_Input
{
public:
    W_Output(minion::MMap* parammap) : W_Input{parammap} {}
    // handle_method inherited from W_Input?
    static W_Output* make(minion::MMap* parammap);
};

class W_List : public Widget
{
public:
    W_List(minion::MMap* parammap) : Widget{parammap} {}
    void handle_method(std::string_view method, minion::MList* paramlist) override;
    static W_List* make(minion::MMap* parammap);
};

class W_TextLine : public Widget
{
public:
    W_TextLine(minion::MMap* parammap) : Widget{parammap} {}
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_TextLine* make(minion::MMap* parammap);

    bool set(std::string_view newtext);
};

class W_RowTable : public Widget
{
public:
    W_RowTable(minion::MMap* parammap) : Widget{parammap} {}
    virtual void handle_method(std::string_view method, minion::MList* paramlist);
    static W_RowTable* make(minion::MMap* parammap);
};

class W_EditForm : public Widget
{
public:
    W_EditForm(minion::MMap* parammap) : Widget{parammap} {}
    static W_EditForm* make(minion::MMap* parammap);
};

#endif // WIDGETS_H
