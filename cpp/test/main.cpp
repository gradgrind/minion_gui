#include "backend.h"
#include "minion.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

string callback_data;

const char* callback1(
    const char* data)
{
    auto m = minion::Reader::read(data);
    printf("callback got '%s'\n", data);
    if (auto mm = m.m_list()) {
        string wname;
        if ((*mm)->get_string(0, wname)) {
            minion::MList mlist({"WIDGET", "Output_1", {"VALUE", wname}});
            minion::MValue val{mlist};
            minion::Writer writer(val, -1);
            callback_data = writer.dump_c();
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
            minion::MList mlist({"WIDGET", wname, {"TEXT", wname}});
            minion::MValue val{mlist};
            minion::Writer writer(val, -1);
            callback_data = writer.dump_c();
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
            minion::MList mlist;
            if (wname == "EF1") {
                mlist = minion::MList({"WIDGET", "popup", {"SHOW", wname}});
            } else {
                mlist = minion::MList({"WIDGET", "TableTotals", {"VALUE", data}});
            }

            minion::MValue val{mlist};
            minion::Writer writer(val, -1);
            callback_data = writer.dump_c();
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
        "../../examples/buttons1.minion",
        "../../examples/grid1.minion",
        "../../examples/various1.minion"
        //
    };

    auto flist = {
        //
        callback1,
        callback2,
        callback3
        //
    };

    string guidata;

    for (int count = 0; count < 2; ++count) {
        int i = 0;
        for (const auto& fp : fplist) {
            /*
            guidata = readfile(fp);
            if (guidata.empty()) {
                cerr << "Read file failed: " << fp << endl;
                exit(1);
            }
            */

            SetCallbackFunction(flist.begin()[i]);
            ++i;

            string fpm = string{"[[MINION_FILE,"}.append(fp).append(
                "],[WIDGET,MainWindow,[SHOW]],[RUN]]");
            Init(fpm.c_str());
            //Init(guidata.c_str());
        }
    }

    return 0;
}
