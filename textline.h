#ifndef TEXTLINE_H
#define TEXTLINE_H

#include "minion.h"
#include <FL/Fl_Input.H>

Fl_Widget *NEW_TextLine(minion::MinionMap param);

class TextLine : public Fl_Input
{
    std::string text;
    bool modified;

    int handle(int event) override;

public:
    TextLine();

    bool set(std::string_view newtext);
};

#endif // TEXTLINE_H