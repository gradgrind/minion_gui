#include "backend.h"
#include "iofile.h"
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

/*TODO
const char* callback3(
    const char* data)
{
    auto m = minion::Reader::read(data);
    auto mm = m.m_map();
    string wname;
    (*mm)->get_string("CALLBACK", wname);
    {
        minion::Writer writer(m, 0);
        printf("callback got '%s'\n", writer.dump_c());
    }

    minion::MMap mp;
    if (wname == "EF1") {
        mp = minion::MMap({{"WIDGET", "popup"}, {"DO", {{"SHOW", wname}}}});
    } else {
        mp = minion::MMap({{"WIDGET", "TableTotals"}, {"DO", {{"VALUE", data}}}});
    }
    auto cbr = dump(mp);
    //printf("??? %s\n", cbr);
    //fflush(stdout);
    return cbr;
}
*/

int main()
{
    auto fplist = {
        // These paths are realtive to the directory
        // in which the binary is built.
        //"../../examples/buttons1.minion",
        "../../examples/grid1.minion",
        //"../../examples/various1.minion"
        //
    };

    auto flist = {
        //
        //callback1,
        callback2,
        //callback3
        //
    };

    string guidata;

    for (int count = 0; count < 2; ++count) {
        int i = 0;
        for (const auto& fp : fplist) {
            if (!readfile(guidata, fp)) {
                printf("File not found: %s\n", fp);
                exit(1);
            }
            SetCallbackFunction(flist.begin()[i]);
            ++i;

            Init(guidata.c_str());
        }
    }

    return 0;
}
