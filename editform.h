#ifndef EDITFORM_H
#define EDITFORM_H

#include <FL/Fl_Grid.H>
#include <map>
#include <string>
#include <vector>

struct EditFormEntry
{
    Fl_Widget *widget;
    std::string name;
    int padabove;
    bool spanning;
    bool growable;
};

class EditForm : public Fl_Grid
{
public:
    std::vector<EditFormEntry> entries;
    int entry_height{30};
    int list_title_space{30};
    //int label_width{0};
    std::map<std::string, Fl_Widget *> entry_map;

    EditForm();
    /* Currently unused
    void add_value(const char *name, const char *label);
    void add_separator();
    void add_list(const char *name, const char *label);
    void do_layout();
    */
};

#endif // EDITFORM_H
