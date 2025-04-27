#ifndef EDITFORM_H
#define EDITFORM_H

#include <FL/Fl_Grid.H>
#include <map>
#include <string>
#include <vector>

class EditForm : public Fl_Grid
{
public:
    int entry_height{30}; // TODO: Do I need this?
    int list_title_space{30};

    EditForm();
};

#endif // EDITFORM_H
