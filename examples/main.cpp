//#include "../minion.h"
#include "../iofile.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

string callback(
    string data)
{
    printf("callback got '%s'\n", data.c_str());
    return "{callback_result: Value}";
}

int main()
{
    auto fplist = {
        "../examples/buttons1.minion"
        //
    };

    string guidata;

    for (int count = 0; count < 1; ++count) {
        for (const auto& fp : fplist) {
            if (!readfile(guidata, fp)) {
                printf("File not found: %s\n", fp);
                exit(1);
            }


MinionGui(guidata, callback)

}
    }

    return 0;
}
