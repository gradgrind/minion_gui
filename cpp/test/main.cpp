#include "../backend.h"
#include "../iofile.h"
#include "../minion.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

minion::InputBuffer minion_ibuffer;
minion::DumpBuffer minion_obuffer;
const char* dump(
    minion::MValue m)
{
    return minion_obuffer.dump(m, 0);
}

const char* callback1(
    const char* data)
{
    auto m = minion_ibuffer.read(data);
    auto mm = m.m_map();
    string wname;
    (*mm)->get_string("CALLBACK", wname);
    printf("callback got '%s'\n", dump(m));
    minion::MMap mp({{"WIDGET", "Output_1"}, {"DO", {{"VALUE", wname}}}});
    auto cbr = dump(mp);
    //printf("??? %s\n", cbr);
    //fflush(stdout);
    return cbr;
}

int main()
{
    auto fplist = {
        // These paths are realtive to the directory
        // in which the binary is built.
        "../../../examples/buttons1.minion"
        //
    };

    auto flist = {callback1};

    string guidata;

    for (int count = 0; count < 1; ++count) {
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
