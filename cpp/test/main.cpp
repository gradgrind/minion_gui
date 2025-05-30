//#include "../minion.h"
#include "../backend.h"
#include "../iofile.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

const char* callback(
    const char* data)
{
    printf("callback got '%s'\n", data);
    return "{callback_result: Value}";
}

int main()
{
    SetCallbackFunction(callback);

    auto fplist = {
        // These paths are realtive to the directory
        // in which the binary is built.
        "../../../examples/buttons1.minion"
        //
    };

    string guidata;

    for (int count = 0; count < 1; ++count) {
        for (const auto& fp : fplist) {
            if (!readfile(guidata, fp)) {
                printf("File not found: %s\n", fp);
                exit(1);
            }
            Init(guidata.c_str());
}
    }

    return 0;
}
