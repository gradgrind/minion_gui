#ifndef EDITFORM_H
#define EDITFORM_H

#include <FL/Fl_Grid.H>
#include <map>
#include <string>
#include <vector>

class EditForm : public Fl_Grid
{
public:
    int list_title_space{30};

    EditForm();
};

#endif // EDITFORM_H
