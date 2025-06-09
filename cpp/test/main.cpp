#include "backend.h"
#include "minion.h"
#include <cstdio>
#include <cstdlib>
#include <map>

using namespace std;

string callback_data;

map<string, bool> tabs;

const char* callback0(
    const char* data)
{
    auto m = minion::Reader::read(data);
    printf("callback got '%s'\n", data);
    if (auto mm = m.m_list()) {
        string wname;
        string wtab;
        if ((*mm)->get_string(0, wname)) {
            if (wname == "()buttons") {
                callback_data = string{"[[WIDGET_PREFIX, buttons]"};
                wtab = "(buttons)l_MainWindow";
                if (!tabs[wtab]) {
                    callback_data.append( //
                        ",[MINION_FILE, ../../examples/buttons.minion],[WIDGET, ");
                    callback_data.append(wtab);
                    callback_data.append(", [PARENT, ()viewer]]");
                    tabs[wtab] = true;
                }
                callback_data.append( //
                    ",[WIDGET, ()viewer, [SELECT, ");
                callback_data.append(wtab);
                callback_data.append("]]]");

            } else if (wname == "()grid") {
                callback_data = string{"[[WIDGET_PREFIX, grid]"};
                wtab = "(grid)l_MainWindow";
                if (!tabs[wtab]) {
                    callback_data.append( //
                        ",[MINION_FILE, ../../examples/grid.minion],[WIDGET, ");
                    callback_data.append(wtab);
                    callback_data.append(", [PARENT, ()viewer]]");
                    tabs[wtab] = true;
                }
                callback_data.append( //
                    ",[WIDGET, ()viewer, [SELECT, ");
                callback_data.append(wtab);
                callback_data.append("]]]");

            } else if (wname == "()complex") {
                callback_data = string{"[[WIDGET_PREFIX, complex]"};
                wtab = "(complex)l_MainWindow";
                if (!tabs[wtab]) {
                    callback_data.append( //
                        ",[MINION_FILE, ../../examples/complex.minion],[WIDGET, ");
                    callback_data.append(wtab);
                    callback_data.append(", [PARENT, ()viewer]]");
                    tabs[wtab] = true;
                }
                callback_data.append( //
                    ",[WIDGET, ()viewer, [SELECT, ");
                callback_data.append(wtab);
                callback_data.append("]]]");

            } else {
                //TODO???

                callback_data = string{"[[WIDGET, ()viewer, [TEXT, \""} //
                                    .append(wname)
                                    .append("\"]]]");
            }
            printf("??? %s\n", callback_data.c_str());
            fflush(stdout);
            return callback_data.c_str();
        }
    }
    throw "Invalid callback";
}

const char* callback1(
    const char* data)
{
    auto m = minion::Reader::read(data);
    printf("callback got '%s'\n", data);
    if (auto mm = m.m_list()) {
        string wname;
        if ((*mm)->get_string(0, wname)) {
            callback_data = string{"[[WIDGET, Output_1, [VALUE, "} //
                                .append(wname)
                                .append("]]]");
            printf("??? %s\n", callback_data.c_str());
            fflush(stdout);
            return callback_data.c_str();
        }
    }
    throw "Invalid callback";
}

const char* callback2(
    const char* data)
{
    auto m = minion::Reader::read(data);
    printf("callback got '%s'\n", data);
    if (auto mm = m.m_list()) {
        string wname;
        if ((*mm)->get_string(0, wname)) {
            callback_data = string{"[[WIDGET, "} //
                                .append(wname)
                                .append(", [TEXT, \"")
                                .append(wname)
                                .append(" pushed\"]]]");
            printf("??? %s\n", callback_data.c_str());
            fflush(stdout);
            return callback_data.c_str();
        }
    }
    throw "Invalid callback";
}

const char* callback3(
    const char* data)
{
    auto m = minion::Reader::read(data);
    printf("callback got '%s'\n", data);
    if (auto mm = m.m_list()) {
        string wname;
        if ((*mm)->get_string(0, wname)) {
            callback_data = string{"[[WIDGET, TableTotals, [VALUE, "} //
                                .append(minion::Writer::dumpString(data))
                                .append("]]");
            if (wname == "EF1" or wname == "EF4") {
                callback_data.append(", [WIDGET, popup, [SHOW, ");
                callback_data.append(wname);
                callback_data.append("]]");
            }
            callback_data.append("]");
            printf("??? %s\n", callback_data.c_str());
            fflush(stdout);
            return callback_data.c_str();
        }
    }
    throw "Invalid callback";
}

int main()
{
    auto fplist = {
        // These paths are realtive to the directory
        // in which the binary is built.
        "../../examples/demo.minion",
        //"../../examples/buttons1.minion",
        //"../../examples/grid1.minion",
        //"../../examples/various1.minion"
        //
    };

    auto flist = {
        //
        callback0,
        callback1,
        callback2,
        callback3
        //
    };

    string guidata;

    for (int count = 0; count < 1; ++count) {
        int i = 0;
        for (const auto& fp : fplist) {
            tabs = {};

            SetCallbackFunction(flist.begin()[i]);
            ++i;

            string fpm = string{"[[MINION_FILE,"}.append(fp).append(
                "],[WIDGET,MainWindow,[SHOW]],[RUN]]");
            Init(fpm.c_str());
        }
    }

    return 0;
}
